/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 *  \file V1/sciclient_irq_rm.c
 *
 *  \brief File containing the J7 Family specific interrupt management data for
 *         RM.
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/sciclient/src/sciclient/sciclient_rm_priv.h>
#include <ti/drv/sciclient/soc/V1/sciclient_irq_rm.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(SOC_J721E)
uint8_t vint_usage_count_NAVSS0_MODSS_INTAGGR_0[64] = {0};
uint8_t vint_usage_count_NAVSS0_MODSS_INTAGGR_1[64] = {0};
uint8_t vint_usage_count_NAVSS0_UDMASS_INTAGGR_0[256] = {0};
uint8_t vint_usage_count_MCU_NAVSS0_INTAGGR_0[256] = {0};
static struct Sciclient_rmIaUsedMapping rom_usage_MCU_NAVSS0_INTAGGR_0[3U] = {
    {
        .event = 16404U,
        .cleared = (bool)false,
    },
    {
        .event = 16405U,
        .cleared = (bool)false,
    },
    {
        .event = 16414U,
        .cleared = (bool)false,
    },
};

struct Sciclient_rmIaInst gRmIaInstances[SCICLIENT_RM_IA_NUM_INST] =
{
    {
        .dev_id             = TISCI_DEV_NAVSS0_MODSS_INTAGGR_0,
        .imap               = 0x30900000,
        .sevt_offset        = 20480u,
        .n_sevt             = 1024u,
        .n_vint             = 64,
        .vint_usage_count   = &vint_usage_count_NAVSS0_MODSS_INTAGGR_0[0],
        .v0_b0_evt          = SCICLIENT_RM_IA_GENERIC_EVT_RESETVAL,
        .rom_usage          = NULL,
        .n_rom_usage        = 0U,
    },
    {
        .dev_id             = TISCI_DEV_NAVSS0_MODSS_INTAGGR_1,
        .imap               = 0x30908000,
        .sevt_offset        = 22528u,
        .n_sevt             = 1024u,
        .n_vint             = 64,
        .vint_usage_count   = &vint_usage_count_NAVSS0_MODSS_INTAGGR_1[0],
        .v0_b0_evt          = SCICLIENT_RM_IA_GENERIC_EVT_RESETVAL,
        .rom_usage          = NULL,
        .n_rom_usage        = 0U,
    },
    {
        .dev_id             = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
        .imap               = 0x30940000,
        .sevt_offset        = 0u,
        .n_sevt             = 4608u,
        .n_vint             = 256,
        .vint_usage_count   = &vint_usage_count_NAVSS0_UDMASS_INTAGGR_0[0],
        .v0_b0_evt          = SCICLIENT_RM_IA_GENERIC_EVT_RESETVAL,
        .rom_usage          = NULL,
        .n_rom_usage        = 0U,
    },
    {
        .dev_id             = TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0,
        .imap               = 0x28560000,
        .sevt_offset        = 16384u,
        .n_sevt             = 1536u,
        .n_vint             = 256,
        .vint_usage_count   = &vint_usage_count_MCU_NAVSS0_INTAGGR_0[0],
        .v0_b0_evt          = SCICLIENT_RM_IA_GENERIC_EVT_RESETVAL,
        .rom_usage          = &rom_usage_MCU_NAVSS0_INTAGGR_0[0U],
        .n_rom_usage        = 3,
    }
};

static struct Sciclient_rmIrUsedMapping rom_usage_MAIN2MCU_LVL_INTRTR0[2U] = {
    {
        .inp_start = 28U,
        .outp_start = 32U,
        .length = 2U,
        .cleared = (bool)false,
        .opCleared = (bool)false,
    },
    {
        .inp_start = 128U,
        .outp_start = 34U,
        .length = 16U,
        .cleared = (bool)false,
        .opCleared = (bool)false,
    },
};
static struct Sciclient_rmIrUsedMapping rom_usage_MCU_NAVSS0_INTR_ROUTER_0[1U] = {
    {
        .inp_start = 1U,
        .outp_start = 0U,
        .length = 2U,
        .cleared = (bool)false,
        .opCleared = (bool)false,
    },
};

struct Sciclient_rmIrInst gRmIrInstances[SCICLIENT_RM_IR_NUM_INST] =
{
    {
        .dev_id         = TISCI_DEV_C66SS0_INTROUTER0,
        .cfg            = 0xac0000,
        .n_inp          = 408U,
        .n_outp         = 97U,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_C66SS1_INTROUTER0,
        .cfg            = 0xad0000,
        .n_inp          = 408U,
        .n_outp         = 97U,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_CMPEVENT_INTRTR0,
        .cfg            = 0xa30000,
        .n_inp          = 96u,
        .n_outp         = 32u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
        .cfg            = 0xa10000,
        .n_inp          = 320u,
        .n_outp         = 64u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = &rom_usage_MAIN2MCU_LVL_INTRTR0[0U],
        .n_rom_usage    = 2,
    },
    {
        .dev_id         = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
        .cfg            = 0xa20000,
        .n_inp          = 112u,
        .n_outp         = 48u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_GPIOMUX_INTRTR0,
        .cfg            = 0xa00000,
        .n_inp          = 304u,
        .n_outp         = 64u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_R5FSS0_INTROUTER0,
        .cfg            = 0xa60000,
        .n_inp          = 344U,
        .n_outp         = 256U,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_R5FSS1_INTROUTER0,
        .cfg            = 0xa70000,
        .n_inp          = 344U,
        .n_outp         = 256U,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_TIMESYNC_INTRTR0,
        .cfg            = 0xa40000,
        .n_inp          = 56u,
        .n_outp         = 48u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
        .cfg            = 0x42200000,
        .n_inp          = 120u,
        .n_outp         = 32u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
        .cfg            = 0x310e0000,
        .n_inp          = 456u,
        .n_outp         = 512u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = NULL,
        .n_rom_usage    = 0U,
    },
    {
        .dev_id         = TISCI_DEV_MCU_NAVSS0_INTR_0,
        .cfg            = 0x28540000,
        .n_inp          = 261u,
        .n_outp         = 64u,
        .inp0_mapping   = SCICLIENT_RM_IR_MAPPING_FREE,
        .rom_usage      = &rom_usage_MCU_NAVSS0_INTR_ROUTER_0[0U],
        .n_rom_usage    = 1,
    },
};

#endif

/* IRQ Tree definition */

#if defined(SOC_J721E)
/* Start of MCU_ADC0 interface definition */
const struct Sciclient_rmIrqIf MCU_ADC0_gen_level_0_0_to_C66SS0_INTROUTER0_in_399_399 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 399,
};
const struct Sciclient_rmIrqIf MCU_ADC0_gen_level_0_0_to_C66SS1_INTROUTER0_in_399_399 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 399,
};
const struct Sciclient_rmIrqIf MCU_ADC0_gen_level_0_0_to_R5FSS0_INTROUTER0_in_116_116 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 116,
};
const struct Sciclient_rmIrqIf MCU_ADC0_gen_level_0_0_to_R5FSS1_INTROUTER0_in_116_116 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 116,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_ADC0[] = {
    &MCU_ADC0_gen_level_0_0_to_C66SS0_INTROUTER0_in_399_399,
    &MCU_ADC0_gen_level_0_0_to_C66SS1_INTROUTER0_in_399_399,
    &MCU_ADC0_gen_level_0_0_to_R5FSS0_INTROUTER0_in_116_116,
    &MCU_ADC0_gen_level_0_0_to_R5FSS1_INTROUTER0_in_116_116,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_ADC0 = {
    .id = TISCI_DEV_MCU_ADC12_16FFC0,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_ADC0[0],
};

/* Start of MCU_ADC1 interface definition */
const struct Sciclient_rmIrqIf MCU_ADC1_gen_level_0_0_to_C66SS0_INTROUTER0_in_400_400 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 400,
};
const struct Sciclient_rmIrqIf MCU_ADC1_gen_level_0_0_to_C66SS1_INTROUTER0_in_400_400 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 400,
};
const struct Sciclient_rmIrqIf MCU_ADC1_gen_level_0_0_to_R5FSS0_INTROUTER0_in_117_117 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 117,
};
const struct Sciclient_rmIrqIf MCU_ADC1_gen_level_0_0_to_R5FSS1_INTROUTER0_in_117_117 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 117,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_ADC1[] = {
    &MCU_ADC1_gen_level_0_0_to_C66SS0_INTROUTER0_in_400_400,
    &MCU_ADC1_gen_level_0_0_to_C66SS1_INTROUTER0_in_400_400,
    &MCU_ADC1_gen_level_0_0_to_R5FSS0_INTROUTER0_in_117_117,
    &MCU_ADC1_gen_level_0_0_to_R5FSS1_INTROUTER0_in_117_117,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_ADC1 = {
    .id = TISCI_DEV_MCU_ADC12_16FFC1,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_ADC1[0],
};

/* Start of compute_cluster_j7es_tb_vdc_main_0 interface definition */
const struct Sciclient_rmIrqIf compute_cluster_j7es_tb_vdc_main_0_gic_output_waker_gic_pwr0_wake_request_0_1_to_R5FSS0_INTROUTER0_in_332_333 = {
    .lbase  = 0,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 332,
};
const struct Sciclient_rmIrqIf compute_cluster_j7es_tb_vdc_main_0_gic_output_waker_gic_pwr0_wake_request_0_1_to_R5FSS1_INTROUTER0_in_332_333 = {
    .lbase  = 0,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 332,
};
const struct Sciclient_rmIrqIf *const tisci_if_compute_cluster_j7es_tb_vdc_main_0[] = {
    &compute_cluster_j7es_tb_vdc_main_0_gic_output_waker_gic_pwr0_wake_request_0_1_to_R5FSS0_INTROUTER0_in_332_333,
    &compute_cluster_j7es_tb_vdc_main_0_gic_output_waker_gic_pwr0_wake_request_0_1_to_R5FSS1_INTROUTER0_in_332_333,
};
static const struct Sciclient_rmIrqNode tisci_irq_compute_cluster_j7es_tb_vdc_main_0 = {
    .id = TISCI_DEV_COMPUTE_CLUSTER0,
    .n_if   = 2,
    .p_if   = &tisci_if_compute_cluster_j7es_tb_vdc_main_0[0],
};

/* Start of COMPUTE_CLUSTER0_CLEC interface definition */
const struct Sciclient_rmIrqIf COMPUTE_CLUSTER0_CLEC_soc_events_out_level_16_31_to_C66SS0_INTROUTER0_in_253_268 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 253,
};
const struct Sciclient_rmIrqIf COMPUTE_CLUSTER0_CLEC_soc_events_out_level_16_31_to_C66SS1_INTROUTER0_in_253_268 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 253,
};
const struct Sciclient_rmIrqIf COMPUTE_CLUSTER0_CLEC_soc_events_out_level_0_15_to_R5FSS0_INTROUTER0_in_84_99 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 84,
};
const struct Sciclient_rmIrqIf COMPUTE_CLUSTER0_CLEC_soc_events_out_level_0_15_to_R5FSS1_INTROUTER0_in_84_99 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 84,
};
const struct Sciclient_rmIrqIf *const tisci_if_COMPUTE_CLUSTER0_CLEC[] = {
    &COMPUTE_CLUSTER0_CLEC_soc_events_out_level_16_31_to_C66SS0_INTROUTER0_in_253_268,
    &COMPUTE_CLUSTER0_CLEC_soc_events_out_level_16_31_to_C66SS1_INTROUTER0_in_253_268,
    &COMPUTE_CLUSTER0_CLEC_soc_events_out_level_0_15_to_R5FSS0_INTROUTER0_in_84_99,
    &COMPUTE_CLUSTER0_CLEC_soc_events_out_level_0_15_to_R5FSS1_INTROUTER0_in_84_99,
};
static const struct Sciclient_rmIrqNode tisci_irq_COMPUTE_CLUSTER0_CLEC = {
    .id = TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
    .n_if   = 4,
    .p_if   = &tisci_if_COMPUTE_CLUSTER0_CLEC[0],
};

/* Start of MCU_CPSW0 interface definition */
const struct Sciclient_rmIrqIf MCU_CPSW0_stat_pend_6_6_to_R5FSS0_INTROUTER0_in_118_118 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 118,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_mdio_pend_5_5_to_R5FSS0_INTROUTER0_in_119_119 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 119,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_evnt_pend_4_4_to_R5FSS0_INTROUTER0_in_120_120 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 120,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_stat_pend_6_6_to_R5FSS1_INTROUTER0_in_118_118 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 118,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_mdio_pend_5_5_to_R5FSS1_INTROUTER0_in_119_119 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 119,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_evnt_pend_4_4_to_R5FSS1_INTROUTER0_in_120_120 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 120,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_16_16 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 16,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_cpts_genf1_2_2_to_TIMESYNC_INTRTR0_in_17_17 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 17,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_cpts_sync_3_3_to_TIMESYNC_INTRTR0_in_38_38 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 38,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_stat_pend_6_6_to_C66SS0_INTROUTER0_in_280_280 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 280,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_mdio_pend_5_5_to_C66SS0_INTROUTER0_in_281_281 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 281,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_evnt_pend_4_4_to_C66SS0_INTROUTER0_in_282_282 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 282,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_stat_pend_6_6_to_C66SS1_INTROUTER0_in_280_280 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 280,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_mdio_pend_5_5_to_C66SS1_INTROUTER0_in_281_281 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 281,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_evnt_pend_4_4_to_C66SS1_INTROUTER0_in_282_282 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 282,
};
const struct Sciclient_rmIrqIf MCU_CPSW0_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_10_10 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_CPSW0[] = {
    &MCU_CPSW0_stat_pend_6_6_to_R5FSS0_INTROUTER0_in_118_118,
    &MCU_CPSW0_mdio_pend_5_5_to_R5FSS0_INTROUTER0_in_119_119,
    &MCU_CPSW0_evnt_pend_4_4_to_R5FSS0_INTROUTER0_in_120_120,
    &MCU_CPSW0_stat_pend_6_6_to_R5FSS1_INTROUTER0_in_118_118,
    &MCU_CPSW0_mdio_pend_5_5_to_R5FSS1_INTROUTER0_in_119_119,
    &MCU_CPSW0_evnt_pend_4_4_to_R5FSS1_INTROUTER0_in_120_120,
    &MCU_CPSW0_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_16_16,
    &MCU_CPSW0_cpts_genf1_2_2_to_TIMESYNC_INTRTR0_in_17_17,
    &MCU_CPSW0_cpts_sync_3_3_to_TIMESYNC_INTRTR0_in_38_38,
    &MCU_CPSW0_stat_pend_6_6_to_C66SS0_INTROUTER0_in_280_280,
    &MCU_CPSW0_mdio_pend_5_5_to_C66SS0_INTROUTER0_in_281_281,
    &MCU_CPSW0_evnt_pend_4_4_to_C66SS0_INTROUTER0_in_282_282,
    &MCU_CPSW0_stat_pend_6_6_to_C66SS1_INTROUTER0_in_280_280,
    &MCU_CPSW0_mdio_pend_5_5_to_C66SS1_INTROUTER0_in_281_281,
    &MCU_CPSW0_evnt_pend_4_4_to_C66SS1_INTROUTER0_in_282_282,
    &MCU_CPSW0_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_10_10,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_CPSW0 = {
    .id = TISCI_DEV_MCU_CPSW0,
    .n_if   = 16,
    .p_if   = &tisci_if_MCU_CPSW0[0],
};

/* Start of CPSW0 interface definition */
const struct Sciclient_rmIrqIf CPSW0_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_14_14 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 14,
};
const struct Sciclient_rmIrqIf CPSW0_cpts_genf1_2_2_to_TIMESYNC_INTRTR0_in_15_15 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 15,
};
const struct Sciclient_rmIrqIf CPSW0_cpts_sync_3_3_to_TIMESYNC_INTRTR0_in_37_37 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 37,
};
const struct Sciclient_rmIrqIf CPSW0_stat_pend_6_6_to_C66SS0_INTROUTER0_in_117_117 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 117,
};
const struct Sciclient_rmIrqIf CPSW0_mdio_pend_5_5_to_C66SS0_INTROUTER0_in_118_118 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 118,
};
const struct Sciclient_rmIrqIf CPSW0_evnt_pend_4_4_to_C66SS0_INTROUTER0_in_119_119 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 119,
};
const struct Sciclient_rmIrqIf CPSW0_stat_pend_6_6_to_C66SS1_INTROUTER0_in_117_117 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 117,
};
const struct Sciclient_rmIrqIf CPSW0_mdio_pend_5_5_to_C66SS1_INTROUTER0_in_118_118 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 118,
};
const struct Sciclient_rmIrqIf CPSW0_evnt_pend_4_4_to_C66SS1_INTROUTER0_in_119_119 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 119,
};
const struct Sciclient_rmIrqIf CPSW0_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_9_9 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 9,
};
const struct Sciclient_rmIrqIf CPSW0_stat_pend_6_6_to_MAIN2MCU_LVL_INTRTR0_in_221_221 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 221,
};
const struct Sciclient_rmIrqIf CPSW0_mdio_pend_5_5_to_MAIN2MCU_LVL_INTRTR0_in_222_222 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 222,
};
const struct Sciclient_rmIrqIf CPSW0_evnt_pend_4_4_to_MAIN2MCU_LVL_INTRTR0_in_223_223 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 223,
};
const struct Sciclient_rmIrqIf *const tisci_if_CPSW0[] = {
    &CPSW0_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_14_14,
    &CPSW0_cpts_genf1_2_2_to_TIMESYNC_INTRTR0_in_15_15,
    &CPSW0_cpts_sync_3_3_to_TIMESYNC_INTRTR0_in_37_37,
    &CPSW0_stat_pend_6_6_to_C66SS0_INTROUTER0_in_117_117,
    &CPSW0_mdio_pend_5_5_to_C66SS0_INTROUTER0_in_118_118,
    &CPSW0_evnt_pend_4_4_to_C66SS0_INTROUTER0_in_119_119,
    &CPSW0_stat_pend_6_6_to_C66SS1_INTROUTER0_in_117_117,
    &CPSW0_mdio_pend_5_5_to_C66SS1_INTROUTER0_in_118_118,
    &CPSW0_evnt_pend_4_4_to_C66SS1_INTROUTER0_in_119_119,
    &CPSW0_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_9_9,
    &CPSW0_stat_pend_6_6_to_MAIN2MCU_LVL_INTRTR0_in_221_221,
    &CPSW0_mdio_pend_5_5_to_MAIN2MCU_LVL_INTRTR0_in_222_222,
    &CPSW0_evnt_pend_4_4_to_MAIN2MCU_LVL_INTRTR0_in_223_223,
};
static const struct Sciclient_rmIrqNode tisci_irq_CPSW0 = {
    .id = TISCI_DEV_CPSW0,
    .n_if   = 13,
    .p_if   = &tisci_if_CPSW0[0],
};

/* Start of CSI_RX_IF0 interface definition */
const struct Sciclient_rmIrqIf CSI_RX_IF0_csi_irq_1_1_to_MAIN2MCU_LVL_INTRTR0_in_254_254 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 254,
};
const struct Sciclient_rmIrqIf CSI_RX_IF0_csi_err_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_255_255 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 255,
};
const struct Sciclient_rmIrqIf CSI_RX_IF0_csi_level_2_2_to_MAIN2MCU_LVL_INTRTR0_in_256_256 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf *const tisci_if_CSI_RX_IF0[] = {
    &CSI_RX_IF0_csi_irq_1_1_to_MAIN2MCU_LVL_INTRTR0_in_254_254,
    &CSI_RX_IF0_csi_err_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_255_255,
    &CSI_RX_IF0_csi_level_2_2_to_MAIN2MCU_LVL_INTRTR0_in_256_256,
};
static const struct Sciclient_rmIrqNode tisci_irq_CSI_RX_IF0 = {
    .id = TISCI_DEV_CSI_RX_IF0,
    .n_if   = 3,
    .p_if   = &tisci_if_CSI_RX_IF0[0],
};

/* Start of CSI_RX_IF1 interface definition */
const struct Sciclient_rmIrqIf CSI_RX_IF1_csi_irq_1_1_to_MAIN2MCU_LVL_INTRTR0_in_257_257 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 257,
};
const struct Sciclient_rmIrqIf CSI_RX_IF1_csi_err_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_258_258 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 258,
};
const struct Sciclient_rmIrqIf CSI_RX_IF1_csi_level_2_2_to_MAIN2MCU_LVL_INTRTR0_in_259_259 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 259,
};
const struct Sciclient_rmIrqIf *const tisci_if_CSI_RX_IF1[] = {
    &CSI_RX_IF1_csi_irq_1_1_to_MAIN2MCU_LVL_INTRTR0_in_257_257,
    &CSI_RX_IF1_csi_err_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_258_258,
    &CSI_RX_IF1_csi_level_2_2_to_MAIN2MCU_LVL_INTRTR0_in_259_259,
};
static const struct Sciclient_rmIrqNode tisci_irq_CSI_RX_IF1 = {
    .id = TISCI_DEV_CSI_RX_IF1,
    .n_if   = 3,
    .p_if   = &tisci_if_CSI_RX_IF1[0],
};

/* Start of CSI_TX_IF0 interface definition */
const struct Sciclient_rmIrqIf CSI_TX_IF0_csi_interrupt_0_0_to_MAIN2MCU_LVL_INTRTR0_in_250_250 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 250,
};
const struct Sciclient_rmIrqIf CSI_TX_IF0_csi_level_1_1_to_MAIN2MCU_LVL_INTRTR0_in_251_251 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 251,
};
const struct Sciclient_rmIrqIf CSI_TX_IF0_csi_interrupt_0_0_to_R5FSS0_INTROUTER0_in_268_268 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 268,
};
const struct Sciclient_rmIrqIf CSI_TX_IF0_csi_level_1_1_to_R5FSS0_INTROUTER0_in_269_269 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 269,
};
const struct Sciclient_rmIrqIf CSI_TX_IF0_csi_interrupt_0_0_to_R5FSS1_INTROUTER0_in_268_268 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 268,
};
const struct Sciclient_rmIrqIf CSI_TX_IF0_csi_level_1_1_to_R5FSS1_INTROUTER0_in_269_269 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 269,
};
const struct Sciclient_rmIrqIf *const tisci_if_CSI_TX_IF0[] = {
    &CSI_TX_IF0_csi_interrupt_0_0_to_MAIN2MCU_LVL_INTRTR0_in_250_250,
    &CSI_TX_IF0_csi_level_1_1_to_MAIN2MCU_LVL_INTRTR0_in_251_251,
    &CSI_TX_IF0_csi_interrupt_0_0_to_R5FSS0_INTROUTER0_in_268_268,
    &CSI_TX_IF0_csi_level_1_1_to_R5FSS0_INTROUTER0_in_269_269,
    &CSI_TX_IF0_csi_interrupt_0_0_to_R5FSS1_INTROUTER0_in_268_268,
    &CSI_TX_IF0_csi_level_1_1_to_R5FSS1_INTROUTER0_in_269_269,
};
static const struct Sciclient_rmIrqNode tisci_irq_CSI_TX_IF0 = {
    .id = TISCI_DEV_CSI_TX_IF0,
    .n_if   = 6,
    .p_if   = &tisci_if_CSI_TX_IF0[0],
};

/* Start of DCC0 interface definition */
const struct Sciclient_rmIrqIf DCC0_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_88_88 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 88,
};
const struct Sciclient_rmIrqIf DCC0_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_280_280 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 280,
};
const struct Sciclient_rmIrqIf DCC0_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_280_280 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 280,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC0[] = {
    &DCC0_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_88_88,
    &DCC0_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_280_280,
    &DCC0_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_280_280,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC0 = {
    .id = TISCI_DEV_DCC0,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC0[0],
};

/* Start of DCC1 interface definition */
const struct Sciclient_rmIrqIf DCC1_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_89_89 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 89,
};
const struct Sciclient_rmIrqIf DCC1_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_281_281 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 281,
};
const struct Sciclient_rmIrqIf DCC1_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_281_281 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 281,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC1[] = {
    &DCC1_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_89_89,
    &DCC1_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_281_281,
    &DCC1_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_281_281,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC1 = {
    .id = TISCI_DEV_DCC1,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC1[0],
};

/* Start of DCC2 interface definition */
const struct Sciclient_rmIrqIf DCC2_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_90_90 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 90,
};
const struct Sciclient_rmIrqIf DCC2_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_282_282 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 282,
};
const struct Sciclient_rmIrqIf DCC2_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_282_282 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 282,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC2[] = {
    &DCC2_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_90_90,
    &DCC2_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_282_282,
    &DCC2_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_282_282,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC2 = {
    .id = TISCI_DEV_DCC2,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC2[0],
};

/* Start of DCC3 interface definition */
const struct Sciclient_rmIrqIf DCC3_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_91_91 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 91,
};
const struct Sciclient_rmIrqIf DCC3_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_283_283 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 283,
};
const struct Sciclient_rmIrqIf DCC3_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_283_283 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 283,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC3[] = {
    &DCC3_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_91_91,
    &DCC3_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_283_283,
    &DCC3_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_283_283,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC3 = {
    .id = TISCI_DEV_DCC3,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC3[0],
};

/* Start of DCC4 interface definition */
const struct Sciclient_rmIrqIf DCC4_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_92_92 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 92,
};
const struct Sciclient_rmIrqIf DCC4_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_284_284 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 284,
};
const struct Sciclient_rmIrqIf DCC4_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_284_284 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 284,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC4[] = {
    &DCC4_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_92_92,
    &DCC4_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_284_284,
    &DCC4_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_284_284,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC4 = {
    .id = TISCI_DEV_DCC4,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC4[0],
};

/* Start of MCU_TIMER0 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER0_intr_pend_0_0_to_C66SS0_INTROUTER0_in_284_284 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 284,
};
const struct Sciclient_rmIrqIf MCU_TIMER0_intr_pend_0_0_to_C66SS1_INTROUTER0_in_284_284 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 284,
};
const struct Sciclient_rmIrqIf MCU_TIMER0_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_124_124 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 124,
};
const struct Sciclient_rmIrqIf MCU_TIMER0_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_124_124 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 124,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER0[] = {
    &MCU_TIMER0_intr_pend_0_0_to_C66SS0_INTROUTER0_in_284_284,
    &MCU_TIMER0_intr_pend_0_0_to_C66SS1_INTROUTER0_in_284_284,
    &MCU_TIMER0_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_124_124,
    &MCU_TIMER0_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_124_124,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER0 = {
    .id = TISCI_DEV_MCU_TIMER0,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER0[0],
};

/* Start of DCC5 interface definition */
const struct Sciclient_rmIrqIf DCC5_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_93_93 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 93,
};
const struct Sciclient_rmIrqIf DCC5_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_285_285 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 285,
};
const struct Sciclient_rmIrqIf DCC5_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_285_285 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 285,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC5[] = {
    &DCC5_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_93_93,
    &DCC5_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_285_285,
    &DCC5_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_285_285,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC5 = {
    .id = TISCI_DEV_DCC5,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC5[0],
};

/* Start of DCC6 interface definition */
const struct Sciclient_rmIrqIf DCC6_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_94_94 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 94,
};
const struct Sciclient_rmIrqIf DCC6_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_286_286 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 286,
};
const struct Sciclient_rmIrqIf DCC6_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_286_286 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 286,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC6[] = {
    &DCC6_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_94_94,
    &DCC6_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_286_286,
    &DCC6_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_286_286,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC6 = {
    .id = TISCI_DEV_DCC6,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC6[0],
};

/* Start of DCC7 interface definition */
const struct Sciclient_rmIrqIf DCC7_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_95_95 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 95,
};
const struct Sciclient_rmIrqIf DCC7_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_287_287 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 287,
};
const struct Sciclient_rmIrqIf DCC7_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_287_287 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 287,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC7[] = {
    &DCC7_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_95_95,
    &DCC7_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_287_287,
    &DCC7_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_287_287,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC7 = {
    .id = TISCI_DEV_DCC7,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC7[0],
};

/* Start of DCC8 interface definition */
const struct Sciclient_rmIrqIf DCC8_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_208_208 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 208,
};
const struct Sciclient_rmIrqIf DCC8_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_304_304 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 304,
};
const struct Sciclient_rmIrqIf DCC8_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_304_304 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 304,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC8[] = {
    &DCC8_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_208_208,
    &DCC8_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_304_304,
    &DCC8_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_304_304,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC8 = {
    .id = TISCI_DEV_DCC8,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC8[0],
};

/* Start of DCC9 interface definition */
const struct Sciclient_rmIrqIf DCC9_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_209_209 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 209,
};
const struct Sciclient_rmIrqIf DCC9_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_305_305 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 305,
};
const struct Sciclient_rmIrqIf DCC9_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_305_305 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 305,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC9[] = {
    &DCC9_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_209_209,
    &DCC9_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_305_305,
    &DCC9_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_305_305,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC9 = {
    .id = TISCI_DEV_DCC9,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC9[0],
};

/* Start of DCC10 interface definition */
const struct Sciclient_rmIrqIf DCC10_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_210_210 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 210,
};
const struct Sciclient_rmIrqIf DCC10_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_306_306 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 306,
};
const struct Sciclient_rmIrqIf DCC10_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_306_306 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 306,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC10[] = {
    &DCC10_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_210_210,
    &DCC10_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_306_306,
    &DCC10_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_306_306,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC10 = {
    .id = TISCI_DEV_DCC10,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC10[0],
};

/* Start of DCC11 interface definition */
const struct Sciclient_rmIrqIf DCC11_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_211_211 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 211,
};
const struct Sciclient_rmIrqIf DCC11_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_307_307 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 307,
};
const struct Sciclient_rmIrqIf DCC11_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_307_307 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 307,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC11[] = {
    &DCC11_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_211_211,
    &DCC11_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_307_307,
    &DCC11_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_307_307,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC11 = {
    .id = TISCI_DEV_DCC11,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC11[0],
};

/* Start of DCC12 interface definition */
const struct Sciclient_rmIrqIf DCC12_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_212_212 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 212,
};
const struct Sciclient_rmIrqIf DCC12_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_308_308 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 308,
};
const struct Sciclient_rmIrqIf DCC12_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_308_308 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 308,
};
const struct Sciclient_rmIrqIf *const tisci_if_DCC12[] = {
    &DCC12_intr_done_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_212_212,
    &DCC12_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_308_308,
    &DCC12_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_308_308,
};
static const struct Sciclient_rmIrqNode tisci_irq_DCC12 = {
    .id = TISCI_DEV_DCC12,
    .n_if   = 3,
    .p_if   = &tisci_if_DCC12[0],
};

/* Start of MCU_DCC0 interface definition */
const struct Sciclient_rmIrqIf MCU_DCC0_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_121_121 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 121,
};
const struct Sciclient_rmIrqIf MCU_DCC0_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_121_121 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 121,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_DCC0[] = {
    &MCU_DCC0_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_121_121,
    &MCU_DCC0_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_121_121,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_DCC0 = {
    .id = TISCI_DEV_MCU_DCC0,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_DCC0[0],
};

/* Start of MCU_DCC1 interface definition */
const struct Sciclient_rmIrqIf MCU_DCC1_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_122_122 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 122,
};
const struct Sciclient_rmIrqIf MCU_DCC1_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_122_122 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 122,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_DCC1[] = {
    &MCU_DCC1_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_122_122,
    &MCU_DCC1_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_122_122,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_DCC1 = {
    .id = TISCI_DEV_MCU_DCC1,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_DCC1[0],
};

/* Start of MCU_DCC2 interface definition */
const struct Sciclient_rmIrqIf MCU_DCC2_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_123_123 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 123,
};
const struct Sciclient_rmIrqIf MCU_DCC2_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_123_123 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 123,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_DCC2[] = {
    &MCU_DCC2_intr_done_level_0_0_to_R5FSS0_INTROUTER0_in_123_123,
    &MCU_DCC2_intr_done_level_0_0_to_R5FSS1_INTROUTER0_in_123_123,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_DCC2 = {
    .id = TISCI_DEV_MCU_DCC2,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_DCC2[0],
};

/* Start of DDR0 interface definition */
const struct Sciclient_rmIrqIf DDR0_ddrss_pll_freq_change_req_2_2_to_MAIN2MCU_LVL_INTRTR0_in_9_9 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 9,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_controller_0_0_to_MAIN2MCU_LVL_INTRTR0_in_10_10 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_v2a_other_err_lvl_3_3_to_MAIN2MCU_LVL_INTRTR0_in_11_11 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 11,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_hs_phy_global_error_1_1_to_MAIN2MCU_LVL_INTRTR0_in_12_12 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 12,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_controller_0_0_to_R5FSS0_INTROUTER0_in_264_264 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 264,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_v2a_other_err_lvl_3_3_to_R5FSS0_INTROUTER0_in_265_265 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 265,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_hs_phy_global_error_1_1_to_R5FSS0_INTROUTER0_in_266_266 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 266,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_pll_freq_change_req_2_2_to_R5FSS0_INTROUTER0_in_267_267 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 267,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_controller_0_0_to_R5FSS1_INTROUTER0_in_264_264 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 264,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_v2a_other_err_lvl_3_3_to_R5FSS1_INTROUTER0_in_265_265 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 265,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_hs_phy_global_error_1_1_to_R5FSS1_INTROUTER0_in_266_266 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 266,
};
const struct Sciclient_rmIrqIf DDR0_ddrss_pll_freq_change_req_2_2_to_R5FSS1_INTROUTER0_in_267_267 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 267,
};
const struct Sciclient_rmIrqIf *const tisci_if_DDR0[] = {
    &DDR0_ddrss_pll_freq_change_req_2_2_to_MAIN2MCU_LVL_INTRTR0_in_9_9,
    &DDR0_ddrss_controller_0_0_to_MAIN2MCU_LVL_INTRTR0_in_10_10,
    &DDR0_ddrss_v2a_other_err_lvl_3_3_to_MAIN2MCU_LVL_INTRTR0_in_11_11,
    &DDR0_ddrss_hs_phy_global_error_1_1_to_MAIN2MCU_LVL_INTRTR0_in_12_12,
    &DDR0_ddrss_controller_0_0_to_R5FSS0_INTROUTER0_in_264_264,
    &DDR0_ddrss_v2a_other_err_lvl_3_3_to_R5FSS0_INTROUTER0_in_265_265,
    &DDR0_ddrss_hs_phy_global_error_1_1_to_R5FSS0_INTROUTER0_in_266_266,
    &DDR0_ddrss_pll_freq_change_req_2_2_to_R5FSS0_INTROUTER0_in_267_267,
    &DDR0_ddrss_controller_0_0_to_R5FSS1_INTROUTER0_in_264_264,
    &DDR0_ddrss_v2a_other_err_lvl_3_3_to_R5FSS1_INTROUTER0_in_265_265,
    &DDR0_ddrss_hs_phy_global_error_1_1_to_R5FSS1_INTROUTER0_in_266_266,
    &DDR0_ddrss_pll_freq_change_req_2_2_to_R5FSS1_INTROUTER0_in_267_267,
};
static const struct Sciclient_rmIrqNode tisci_irq_DDR0 = {
    .id = TISCI_DEV_DDR0,
    .n_if   = 12,
    .p_if   = &tisci_if_DDR0[0],
};

/* Start of TIMER0 interface definition */
const struct Sciclient_rmIrqIf TIMER0_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_108_108 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 108,
};
const struct Sciclient_rmIrqIf TIMER0_intr_pend_0_0_to_C66SS0_INTROUTER0_in_1_1 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf TIMER0_intr_pend_0_0_to_C66SS1_INTROUTER0_in_1_1 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf TIMER0_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_240_240 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 240,
};
const struct Sciclient_rmIrqIf TIMER0_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_240_240 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 240,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER0[] = {
    &TIMER0_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_108_108,
    &TIMER0_intr_pend_0_0_to_C66SS0_INTROUTER0_in_1_1,
    &TIMER0_intr_pend_0_0_to_C66SS1_INTROUTER0_in_1_1,
    &TIMER0_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_240_240,
    &TIMER0_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_240_240,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER0 = {
    .id = TISCI_DEV_TIMER0,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER0[0],
};

/* Start of TIMER1 interface definition */
const struct Sciclient_rmIrqIf TIMER1_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_109_109 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 109,
};
const struct Sciclient_rmIrqIf TIMER1_intr_pend_0_0_to_C66SS0_INTROUTER0_in_2_2 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf TIMER1_intr_pend_0_0_to_C66SS1_INTROUTER0_in_2_2 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf TIMER1_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_241_241 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 241,
};
const struct Sciclient_rmIrqIf TIMER1_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_241_241 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 241,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER1[] = {
    &TIMER1_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_109_109,
    &TIMER1_intr_pend_0_0_to_C66SS0_INTROUTER0_in_2_2,
    &TIMER1_intr_pend_0_0_to_C66SS1_INTROUTER0_in_2_2,
    &TIMER1_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_241_241,
    &TIMER1_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_241_241,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER1 = {
    .id = TISCI_DEV_TIMER1,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER1[0],
};

/* Start of TIMER2 interface definition */
const struct Sciclient_rmIrqIf TIMER2_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_110_110 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 110,
};
const struct Sciclient_rmIrqIf TIMER2_intr_pend_0_0_to_C66SS0_INTROUTER0_in_3_3 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf TIMER2_intr_pend_0_0_to_C66SS1_INTROUTER0_in_3_3 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf TIMER2_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_242_242 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 242,
};
const struct Sciclient_rmIrqIf TIMER2_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_242_242 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 242,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER2[] = {
    &TIMER2_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_110_110,
    &TIMER2_intr_pend_0_0_to_C66SS0_INTROUTER0_in_3_3,
    &TIMER2_intr_pend_0_0_to_C66SS1_INTROUTER0_in_3_3,
    &TIMER2_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_242_242,
    &TIMER2_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_242_242,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER2 = {
    .id = TISCI_DEV_TIMER2,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER2[0],
};

/* Start of TIMER3 interface definition */
const struct Sciclient_rmIrqIf TIMER3_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_111_111 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 111,
};
const struct Sciclient_rmIrqIf TIMER3_intr_pend_0_0_to_C66SS0_INTROUTER0_in_4_4 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf TIMER3_intr_pend_0_0_to_C66SS1_INTROUTER0_in_4_4 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf TIMER3_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_243_243 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 243,
};
const struct Sciclient_rmIrqIf TIMER3_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_243_243 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 243,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER3[] = {
    &TIMER3_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_111_111,
    &TIMER3_intr_pend_0_0_to_C66SS0_INTROUTER0_in_4_4,
    &TIMER3_intr_pend_0_0_to_C66SS1_INTROUTER0_in_4_4,
    &TIMER3_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_243_243,
    &TIMER3_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_243_243,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER3 = {
    .id = TISCI_DEV_TIMER3,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER3[0],
};

/* Start of TIMER4 interface definition */
const struct Sciclient_rmIrqIf TIMER4_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_112_112 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 112,
};
const struct Sciclient_rmIrqIf TIMER4_intr_pend_0_0_to_C66SS0_INTROUTER0_in_5_5 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf TIMER4_intr_pend_0_0_to_C66SS1_INTROUTER0_in_5_5 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf TIMER4_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_244_244 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 244,
};
const struct Sciclient_rmIrqIf TIMER4_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_244_244 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 244,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER4[] = {
    &TIMER4_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_112_112,
    &TIMER4_intr_pend_0_0_to_C66SS0_INTROUTER0_in_5_5,
    &TIMER4_intr_pend_0_0_to_C66SS1_INTROUTER0_in_5_5,
    &TIMER4_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_244_244,
    &TIMER4_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_244_244,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER4 = {
    .id = TISCI_DEV_TIMER4,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER4[0],
};

/* Start of TIMER5 interface definition */
const struct Sciclient_rmIrqIf TIMER5_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_113_113 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 113,
};
const struct Sciclient_rmIrqIf TIMER5_intr_pend_0_0_to_C66SS0_INTROUTER0_in_6_6 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 6,
};
const struct Sciclient_rmIrqIf TIMER5_intr_pend_0_0_to_C66SS1_INTROUTER0_in_6_6 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 6,
};
const struct Sciclient_rmIrqIf TIMER5_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_245_245 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 245,
};
const struct Sciclient_rmIrqIf TIMER5_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_245_245 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 245,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER5[] = {
    &TIMER5_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_113_113,
    &TIMER5_intr_pend_0_0_to_C66SS0_INTROUTER0_in_6_6,
    &TIMER5_intr_pend_0_0_to_C66SS1_INTROUTER0_in_6_6,
    &TIMER5_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_245_245,
    &TIMER5_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_245_245,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER5 = {
    .id = TISCI_DEV_TIMER5,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER5[0],
};

/* Start of TIMER6 interface definition */
const struct Sciclient_rmIrqIf TIMER6_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_114_114 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 114,
};
const struct Sciclient_rmIrqIf TIMER6_intr_pend_0_0_to_C66SS0_INTROUTER0_in_7_7 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf TIMER6_intr_pend_0_0_to_C66SS1_INTROUTER0_in_7_7 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf TIMER6_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_246_246 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 246,
};
const struct Sciclient_rmIrqIf TIMER6_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_246_246 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 246,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER6[] = {
    &TIMER6_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_114_114,
    &TIMER6_intr_pend_0_0_to_C66SS0_INTROUTER0_in_7_7,
    &TIMER6_intr_pend_0_0_to_C66SS1_INTROUTER0_in_7_7,
    &TIMER6_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_246_246,
    &TIMER6_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_246_246,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER6 = {
    .id = TISCI_DEV_TIMER6,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER6[0],
};

/* Start of TIMER7 interface definition */
const struct Sciclient_rmIrqIf TIMER7_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_115_115 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 115,
};
const struct Sciclient_rmIrqIf TIMER7_intr_pend_0_0_to_C66SS0_INTROUTER0_in_8_8 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 8,
};
const struct Sciclient_rmIrqIf TIMER7_intr_pend_0_0_to_C66SS1_INTROUTER0_in_8_8 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 8,
};
const struct Sciclient_rmIrqIf TIMER7_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_247_247 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 247,
};
const struct Sciclient_rmIrqIf TIMER7_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_247_247 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 247,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER7[] = {
    &TIMER7_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_115_115,
    &TIMER7_intr_pend_0_0_to_C66SS0_INTROUTER0_in_8_8,
    &TIMER7_intr_pend_0_0_to_C66SS1_INTROUTER0_in_8_8,
    &TIMER7_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_247_247,
    &TIMER7_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_247_247,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER7 = {
    .id = TISCI_DEV_TIMER7,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER7[0],
};

/* Start of TIMER8 interface definition */
const struct Sciclient_rmIrqIf TIMER8_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_116_116 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 116,
};
const struct Sciclient_rmIrqIf TIMER8_intr_pend_0_0_to_C66SS0_INTROUTER0_in_9_9 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 9,
};
const struct Sciclient_rmIrqIf TIMER8_intr_pend_0_0_to_C66SS1_INTROUTER0_in_9_9 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 9,
};
const struct Sciclient_rmIrqIf TIMER8_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_248_248 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 248,
};
const struct Sciclient_rmIrqIf TIMER8_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_248_248 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 248,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER8[] = {
    &TIMER8_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_116_116,
    &TIMER8_intr_pend_0_0_to_C66SS0_INTROUTER0_in_9_9,
    &TIMER8_intr_pend_0_0_to_C66SS1_INTROUTER0_in_9_9,
    &TIMER8_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_248_248,
    &TIMER8_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_248_248,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER8 = {
    .id = TISCI_DEV_TIMER8,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER8[0],
};

/* Start of TIMER9 interface definition */
const struct Sciclient_rmIrqIf TIMER9_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_117_117 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 117,
};
const struct Sciclient_rmIrqIf TIMER9_intr_pend_0_0_to_C66SS0_INTROUTER0_in_10_10 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf TIMER9_intr_pend_0_0_to_C66SS1_INTROUTER0_in_10_10 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf TIMER9_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_249_249 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 249,
};
const struct Sciclient_rmIrqIf TIMER9_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_249_249 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 249,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER9[] = {
    &TIMER9_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_117_117,
    &TIMER9_intr_pend_0_0_to_C66SS0_INTROUTER0_in_10_10,
    &TIMER9_intr_pend_0_0_to_C66SS1_INTROUTER0_in_10_10,
    &TIMER9_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_249_249,
    &TIMER9_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_249_249,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER9 = {
    .id = TISCI_DEV_TIMER9,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER9[0],
};

/* Start of TIMER10 interface definition */
const struct Sciclient_rmIrqIf TIMER10_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_118_118 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 118,
};
const struct Sciclient_rmIrqIf TIMER10_intr_pend_0_0_to_C66SS0_INTROUTER0_in_11_11 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 11,
};
const struct Sciclient_rmIrqIf TIMER10_intr_pend_0_0_to_C66SS1_INTROUTER0_in_11_11 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 11,
};
const struct Sciclient_rmIrqIf TIMER10_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_250_250 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 250,
};
const struct Sciclient_rmIrqIf TIMER10_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_250_250 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 250,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER10[] = {
    &TIMER10_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_118_118,
    &TIMER10_intr_pend_0_0_to_C66SS0_INTROUTER0_in_11_11,
    &TIMER10_intr_pend_0_0_to_C66SS1_INTROUTER0_in_11_11,
    &TIMER10_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_250_250,
    &TIMER10_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_250_250,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER10 = {
    .id = TISCI_DEV_TIMER10,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER10[0],
};

/* Start of GTC0 interface definition */
const struct Sciclient_rmIrqIf GTC0_gtc_push_event_0_0_to_TIMESYNC_INTRTR0_in_1_1 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf *const tisci_if_GTC0[] = {
    &GTC0_gtc_push_event_0_0_to_TIMESYNC_INTRTR0_in_1_1,
};
static const struct Sciclient_rmIrqNode tisci_irq_GTC0 = {
    .id = TISCI_DEV_GTC0,
    .n_if   = 1,
    .p_if   = &tisci_if_GTC0[0],
};

/* Start of TIMER11 interface definition */
const struct Sciclient_rmIrqIf TIMER11_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_119_119 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 119,
};
const struct Sciclient_rmIrqIf TIMER11_intr_pend_0_0_to_C66SS0_INTROUTER0_in_12_12 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 12,
};
const struct Sciclient_rmIrqIf TIMER11_intr_pend_0_0_to_C66SS1_INTROUTER0_in_12_12 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 12,
};
const struct Sciclient_rmIrqIf TIMER11_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_251_251 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 251,
};
const struct Sciclient_rmIrqIf TIMER11_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_251_251 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 251,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER11[] = {
    &TIMER11_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_119_119,
    &TIMER11_intr_pend_0_0_to_C66SS0_INTROUTER0_in_12_12,
    &TIMER11_intr_pend_0_0_to_C66SS1_INTROUTER0_in_12_12,
    &TIMER11_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_251_251,
    &TIMER11_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_251_251,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER11 = {
    .id = TISCI_DEV_TIMER11,
    .n_if   = 5,
    .p_if   = &tisci_if_TIMER11[0],
};

/* Start of TIMER12 interface definition */
const struct Sciclient_rmIrqIf TIMER12_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_120_120 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 120,
};
const struct Sciclient_rmIrqIf TIMER12_intr_pend_0_0_to_C66SS0_INTROUTER0_in_304_304 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 304,
};
const struct Sciclient_rmIrqIf TIMER12_intr_pend_0_0_to_C66SS1_INTROUTER0_in_304_304 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 304,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER12[] = {
    &TIMER12_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_120_120,
    &TIMER12_intr_pend_0_0_to_C66SS0_INTROUTER0_in_304_304,
    &TIMER12_intr_pend_0_0_to_C66SS1_INTROUTER0_in_304_304,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER12 = {
    .id = TISCI_DEV_TIMER12,
    .n_if   = 3,
    .p_if   = &tisci_if_TIMER12[0],
};

/* Start of TIMER13 interface definition */
const struct Sciclient_rmIrqIf TIMER13_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_121_121 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 121,
};
const struct Sciclient_rmIrqIf TIMER13_intr_pend_0_0_to_C66SS0_INTROUTER0_in_305_305 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 305,
};
const struct Sciclient_rmIrqIf TIMER13_intr_pend_0_0_to_C66SS1_INTROUTER0_in_305_305 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 305,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER13[] = {
    &TIMER13_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_121_121,
    &TIMER13_intr_pend_0_0_to_C66SS0_INTROUTER0_in_305_305,
    &TIMER13_intr_pend_0_0_to_C66SS1_INTROUTER0_in_305_305,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER13 = {
    .id = TISCI_DEV_TIMER13,
    .n_if   = 3,
    .p_if   = &tisci_if_TIMER13[0],
};

/* Start of TIMER14 interface definition */
const struct Sciclient_rmIrqIf TIMER14_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_122_122 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 122,
};
const struct Sciclient_rmIrqIf TIMER14_intr_pend_0_0_to_C66SS0_INTROUTER0_in_306_306 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 306,
};
const struct Sciclient_rmIrqIf TIMER14_intr_pend_0_0_to_C66SS1_INTROUTER0_in_306_306 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 306,
};
const struct Sciclient_rmIrqIf TIMER14_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_2_2 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER14[] = {
    &TIMER14_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_122_122,
    &TIMER14_intr_pend_0_0_to_C66SS0_INTROUTER0_in_306_306,
    &TIMER14_intr_pend_0_0_to_C66SS1_INTROUTER0_in_306_306,
    &TIMER14_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_2_2,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER14 = {
    .id = TISCI_DEV_TIMER14,
    .n_if   = 4,
    .p_if   = &tisci_if_TIMER14[0],
};

/* Start of TIMER15 interface definition */
const struct Sciclient_rmIrqIf TIMER15_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_123_123 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 123,
};
const struct Sciclient_rmIrqIf TIMER15_intr_pend_0_0_to_C66SS0_INTROUTER0_in_307_307 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 307,
};
const struct Sciclient_rmIrqIf TIMER15_intr_pend_0_0_to_C66SS1_INTROUTER0_in_307_307 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 307,
};
const struct Sciclient_rmIrqIf TIMER15_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_3_3 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER15[] = {
    &TIMER15_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_123_123,
    &TIMER15_intr_pend_0_0_to_C66SS0_INTROUTER0_in_307_307,
    &TIMER15_intr_pend_0_0_to_C66SS1_INTROUTER0_in_307_307,
    &TIMER15_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_3_3,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER15 = {
    .id = TISCI_DEV_TIMER15,
    .n_if   = 4,
    .p_if   = &tisci_if_TIMER15[0],
};

/* Start of TIMER16 interface definition */
const struct Sciclient_rmIrqIf TIMER16_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_124_124 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 124,
};
const struct Sciclient_rmIrqIf TIMER16_intr_pend_0_0_to_C66SS0_INTROUTER0_in_13_13 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 13,
};
const struct Sciclient_rmIrqIf TIMER16_intr_pend_0_0_to_C66SS1_INTROUTER0_in_13_13 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 13,
};
const struct Sciclient_rmIrqIf TIMER16_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_40_40 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 40,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER16[] = {
    &TIMER16_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_124_124,
    &TIMER16_intr_pend_0_0_to_C66SS0_INTROUTER0_in_13_13,
    &TIMER16_intr_pend_0_0_to_C66SS1_INTROUTER0_in_13_13,
    &TIMER16_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_40_40,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER16 = {
    .id = TISCI_DEV_TIMER16,
    .n_if   = 4,
    .p_if   = &tisci_if_TIMER16[0],
};

/* Start of TIMER17 interface definition */
const struct Sciclient_rmIrqIf TIMER17_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_125_125 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 125,
};
const struct Sciclient_rmIrqIf TIMER17_intr_pend_0_0_to_C66SS0_INTROUTER0_in_14_14 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 14,
};
const struct Sciclient_rmIrqIf TIMER17_intr_pend_0_0_to_C66SS1_INTROUTER0_in_14_14 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 14,
};
const struct Sciclient_rmIrqIf TIMER17_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_41_41 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 41,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER17[] = {
    &TIMER17_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_125_125,
    &TIMER17_intr_pend_0_0_to_C66SS0_INTROUTER0_in_14_14,
    &TIMER17_intr_pend_0_0_to_C66SS1_INTROUTER0_in_14_14,
    &TIMER17_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_41_41,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER17 = {
    .id = TISCI_DEV_TIMER17,
    .n_if   = 4,
    .p_if   = &tisci_if_TIMER17[0],
};

/* Start of TIMER18 interface definition */
const struct Sciclient_rmIrqIf TIMER18_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_126_126 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 126,
};
const struct Sciclient_rmIrqIf TIMER18_intr_pend_0_0_to_C66SS0_INTROUTER0_in_15_15 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 15,
};
const struct Sciclient_rmIrqIf TIMER18_intr_pend_0_0_to_C66SS1_INTROUTER0_in_15_15 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 15,
};
const struct Sciclient_rmIrqIf TIMER18_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_42_42 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 42,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER18[] = {
    &TIMER18_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_126_126,
    &TIMER18_intr_pend_0_0_to_C66SS0_INTROUTER0_in_15_15,
    &TIMER18_intr_pend_0_0_to_C66SS1_INTROUTER0_in_15_15,
    &TIMER18_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_42_42,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER18 = {
    .id = TISCI_DEV_TIMER18,
    .n_if   = 4,
    .p_if   = &tisci_if_TIMER18[0],
};

/* Start of TIMER19 interface definition */
const struct Sciclient_rmIrqIf TIMER19_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_127_127 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 127,
};
const struct Sciclient_rmIrqIf TIMER19_intr_pend_0_0_to_C66SS0_INTROUTER0_in_16_16 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 16,
};
const struct Sciclient_rmIrqIf TIMER19_intr_pend_0_0_to_C66SS1_INTROUTER0_in_16_16 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 16,
};
const struct Sciclient_rmIrqIf TIMER19_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_43_43 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 43,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMER19[] = {
    &TIMER19_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_127_127,
    &TIMER19_intr_pend_0_0_to_C66SS0_INTROUTER0_in_16_16,
    &TIMER19_intr_pend_0_0_to_C66SS1_INTROUTER0_in_16_16,
    &TIMER19_timer_pwm_1_1_to_TIMESYNC_INTRTR0_in_43_43,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMER19 = {
    .id = TISCI_DEV_TIMER19,
    .n_if   = 4,
    .p_if   = &tisci_if_TIMER19[0],
};

/* Start of MCU_TIMER1 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER1_intr_pend_0_0_to_C66SS0_INTROUTER0_in_285_285 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 285,
};
const struct Sciclient_rmIrqIf MCU_TIMER1_intr_pend_0_0_to_C66SS1_INTROUTER0_in_285_285 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 285,
};
const struct Sciclient_rmIrqIf MCU_TIMER1_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_125_125 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 125,
};
const struct Sciclient_rmIrqIf MCU_TIMER1_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_125_125 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 125,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER1[] = {
    &MCU_TIMER1_intr_pend_0_0_to_C66SS0_INTROUTER0_in_285_285,
    &MCU_TIMER1_intr_pend_0_0_to_C66SS1_INTROUTER0_in_285_285,
    &MCU_TIMER1_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_125_125,
    &MCU_TIMER1_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_125_125,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER1 = {
    .id = TISCI_DEV_MCU_TIMER1,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER1[0],
};

/* Start of MCU_TIMER2 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER2_intr_pend_0_0_to_C66SS0_INTROUTER0_in_286_286 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 286,
};
const struct Sciclient_rmIrqIf MCU_TIMER2_intr_pend_0_0_to_C66SS1_INTROUTER0_in_286_286 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 286,
};
const struct Sciclient_rmIrqIf MCU_TIMER2_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_126_126 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 126,
};
const struct Sciclient_rmIrqIf MCU_TIMER2_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_126_126 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 126,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER2[] = {
    &MCU_TIMER2_intr_pend_0_0_to_C66SS0_INTROUTER0_in_286_286,
    &MCU_TIMER2_intr_pend_0_0_to_C66SS1_INTROUTER0_in_286_286,
    &MCU_TIMER2_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_126_126,
    &MCU_TIMER2_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_126_126,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER2 = {
    .id = TISCI_DEV_MCU_TIMER2,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER2[0],
};

/* Start of MCU_TIMER3 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER3_intr_pend_0_0_to_C66SS0_INTROUTER0_in_287_287 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 287,
};
const struct Sciclient_rmIrqIf MCU_TIMER3_intr_pend_0_0_to_C66SS1_INTROUTER0_in_287_287 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 287,
};
const struct Sciclient_rmIrqIf MCU_TIMER3_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_127_127 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 127,
};
const struct Sciclient_rmIrqIf MCU_TIMER3_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_127_127 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 127,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER3[] = {
    &MCU_TIMER3_intr_pend_0_0_to_C66SS0_INTROUTER0_in_287_287,
    &MCU_TIMER3_intr_pend_0_0_to_C66SS1_INTROUTER0_in_287_287,
    &MCU_TIMER3_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_127_127,
    &MCU_TIMER3_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_127_127,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER3 = {
    .id = TISCI_DEV_MCU_TIMER3,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER3[0],
};

/* Start of MCU_TIMER4 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER4_intr_pend_0_0_to_C66SS0_INTROUTER0_in_288_288 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 288,
};
const struct Sciclient_rmIrqIf MCU_TIMER4_intr_pend_0_0_to_C66SS1_INTROUTER0_in_288_288 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 288,
};
const struct Sciclient_rmIrqIf MCU_TIMER4_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_128_128 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 128,
};
const struct Sciclient_rmIrqIf MCU_TIMER4_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_128_128 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 128,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER4[] = {
    &MCU_TIMER4_intr_pend_0_0_to_C66SS0_INTROUTER0_in_288_288,
    &MCU_TIMER4_intr_pend_0_0_to_C66SS1_INTROUTER0_in_288_288,
    &MCU_TIMER4_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_128_128,
    &MCU_TIMER4_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_128_128,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER4 = {
    .id = TISCI_DEV_MCU_TIMER4,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER4[0],
};

/* Start of MCU_TIMER5 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER5_intr_pend_0_0_to_C66SS0_INTROUTER0_in_289_289 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 289,
};
const struct Sciclient_rmIrqIf MCU_TIMER5_intr_pend_0_0_to_C66SS1_INTROUTER0_in_289_289 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 289,
};
const struct Sciclient_rmIrqIf MCU_TIMER5_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_129_129 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 129,
};
const struct Sciclient_rmIrqIf MCU_TIMER5_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_129_129 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 129,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER5[] = {
    &MCU_TIMER5_intr_pend_0_0_to_C66SS0_INTROUTER0_in_289_289,
    &MCU_TIMER5_intr_pend_0_0_to_C66SS1_INTROUTER0_in_289_289,
    &MCU_TIMER5_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_129_129,
    &MCU_TIMER5_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_129_129,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER5 = {
    .id = TISCI_DEV_MCU_TIMER5,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER5[0],
};

/* Start of MCU_TIMER6 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER6_intr_pend_0_0_to_C66SS0_INTROUTER0_in_290_290 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 290,
};
const struct Sciclient_rmIrqIf MCU_TIMER6_intr_pend_0_0_to_C66SS1_INTROUTER0_in_290_290 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 290,
};
const struct Sciclient_rmIrqIf MCU_TIMER6_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_130_130 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 130,
};
const struct Sciclient_rmIrqIf MCU_TIMER6_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_130_130 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 130,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER6[] = {
    &MCU_TIMER6_intr_pend_0_0_to_C66SS0_INTROUTER0_in_290_290,
    &MCU_TIMER6_intr_pend_0_0_to_C66SS1_INTROUTER0_in_290_290,
    &MCU_TIMER6_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_130_130,
    &MCU_TIMER6_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_130_130,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER6 = {
    .id = TISCI_DEV_MCU_TIMER6,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER6[0],
};

/* Start of MCU_TIMER7 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER7_intr_pend_0_0_to_C66SS0_INTROUTER0_in_291_291 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 291,
};
const struct Sciclient_rmIrqIf MCU_TIMER7_intr_pend_0_0_to_C66SS1_INTROUTER0_in_291_291 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 291,
};
const struct Sciclient_rmIrqIf MCU_TIMER7_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_131_131 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 131,
};
const struct Sciclient_rmIrqIf MCU_TIMER7_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_131_131 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 131,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER7[] = {
    &MCU_TIMER7_intr_pend_0_0_to_C66SS0_INTROUTER0_in_291_291,
    &MCU_TIMER7_intr_pend_0_0_to_C66SS1_INTROUTER0_in_291_291,
    &MCU_TIMER7_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_131_131,
    &MCU_TIMER7_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_131_131,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER7 = {
    .id = TISCI_DEV_MCU_TIMER7,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER7[0],
};

/* Start of MCU_TIMER8 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER8_intr_pend_0_0_to_C66SS0_INTROUTER0_in_292_292 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 292,
};
const struct Sciclient_rmIrqIf MCU_TIMER8_intr_pend_0_0_to_C66SS1_INTROUTER0_in_292_292 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 292,
};
const struct Sciclient_rmIrqIf MCU_TIMER8_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_132_132 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 132,
};
const struct Sciclient_rmIrqIf MCU_TIMER8_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_132_132 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 132,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER8[] = {
    &MCU_TIMER8_intr_pend_0_0_to_C66SS0_INTROUTER0_in_292_292,
    &MCU_TIMER8_intr_pend_0_0_to_C66SS1_INTROUTER0_in_292_292,
    &MCU_TIMER8_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_132_132,
    &MCU_TIMER8_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_132_132,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER8 = {
    .id = TISCI_DEV_MCU_TIMER8,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER8[0],
};

/* Start of MCU_TIMER9 interface definition */
const struct Sciclient_rmIrqIf MCU_TIMER9_intr_pend_0_0_to_C66SS0_INTROUTER0_in_293_293 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 293,
};
const struct Sciclient_rmIrqIf MCU_TIMER9_intr_pend_0_0_to_C66SS1_INTROUTER0_in_293_293 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 293,
};
const struct Sciclient_rmIrqIf MCU_TIMER9_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_133_133 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 133,
};
const struct Sciclient_rmIrqIf MCU_TIMER9_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_133_133 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 133,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_TIMER9[] = {
    &MCU_TIMER9_intr_pend_0_0_to_C66SS0_INTROUTER0_in_293_293,
    &MCU_TIMER9_intr_pend_0_0_to_C66SS1_INTROUTER0_in_293_293,
    &MCU_TIMER9_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_133_133,
    &MCU_TIMER9_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_133_133,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_TIMER9 = {
    .id = TISCI_DEV_MCU_TIMER9,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_TIMER9[0],
};

/* Start of ECAP0 interface definition */
const struct Sciclient_rmIrqIf ECAP0_ecap_int_0_0_to_C66SS0_INTROUTER0_in_45_45 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 45,
};
const struct Sciclient_rmIrqIf ECAP0_ecap_int_0_0_to_C66SS1_INTROUTER0_in_45_45 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 45,
};
const struct Sciclient_rmIrqIf ECAP0_ecap_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_17_17 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 17,
};
const struct Sciclient_rmIrqIf ECAP0_ecap_int_0_0_to_R5FSS0_INTROUTER0_in_316_316 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 316,
};
const struct Sciclient_rmIrqIf ECAP0_ecap_int_0_0_to_R5FSS1_INTROUTER0_in_316_316 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 316,
};
const struct Sciclient_rmIrqIf *const tisci_if_ECAP0[] = {
    &ECAP0_ecap_int_0_0_to_C66SS0_INTROUTER0_in_45_45,
    &ECAP0_ecap_int_0_0_to_C66SS1_INTROUTER0_in_45_45,
    &ECAP0_ecap_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_17_17,
    &ECAP0_ecap_int_0_0_to_R5FSS0_INTROUTER0_in_316_316,
    &ECAP0_ecap_int_0_0_to_R5FSS1_INTROUTER0_in_316_316,
};
static const struct Sciclient_rmIrqNode tisci_irq_ECAP0 = {
    .id = TISCI_DEV_ECAP0,
    .n_if   = 5,
    .p_if   = &tisci_if_ECAP0[0],
};

/* Start of ECAP1 interface definition */
const struct Sciclient_rmIrqIf ECAP1_ecap_int_0_0_to_C66SS0_INTROUTER0_in_46_46 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 46,
};
const struct Sciclient_rmIrqIf ECAP1_ecap_int_0_0_to_C66SS1_INTROUTER0_in_46_46 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 46,
};
const struct Sciclient_rmIrqIf ECAP1_ecap_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_18_18 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 18,
};
const struct Sciclient_rmIrqIf ECAP1_ecap_int_0_0_to_R5FSS0_INTROUTER0_in_317_317 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 317,
};
const struct Sciclient_rmIrqIf ECAP1_ecap_int_0_0_to_R5FSS1_INTROUTER0_in_317_317 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 317,
};
const struct Sciclient_rmIrqIf *const tisci_if_ECAP1[] = {
    &ECAP1_ecap_int_0_0_to_C66SS0_INTROUTER0_in_46_46,
    &ECAP1_ecap_int_0_0_to_C66SS1_INTROUTER0_in_46_46,
    &ECAP1_ecap_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_18_18,
    &ECAP1_ecap_int_0_0_to_R5FSS0_INTROUTER0_in_317_317,
    &ECAP1_ecap_int_0_0_to_R5FSS1_INTROUTER0_in_317_317,
};
static const struct Sciclient_rmIrqNode tisci_irq_ECAP1 = {
    .id = TISCI_DEV_ECAP1,
    .n_if   = 5,
    .p_if   = &tisci_if_ECAP1[0],
};

/* Start of ECAP2 interface definition */
const struct Sciclient_rmIrqIf ECAP2_ecap_int_0_0_to_C66SS0_INTROUTER0_in_47_47 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 47,
};
const struct Sciclient_rmIrqIf ECAP2_ecap_int_0_0_to_C66SS1_INTROUTER0_in_47_47 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 47,
};
const struct Sciclient_rmIrqIf ECAP2_ecap_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_19_19 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 19,
};
const struct Sciclient_rmIrqIf ECAP2_ecap_int_0_0_to_R5FSS0_INTROUTER0_in_318_318 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 318,
};
const struct Sciclient_rmIrqIf ECAP2_ecap_int_0_0_to_R5FSS1_INTROUTER0_in_318_318 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 318,
};
const struct Sciclient_rmIrqIf *const tisci_if_ECAP2[] = {
    &ECAP2_ecap_int_0_0_to_C66SS0_INTROUTER0_in_47_47,
    &ECAP2_ecap_int_0_0_to_C66SS1_INTROUTER0_in_47_47,
    &ECAP2_ecap_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_19_19,
    &ECAP2_ecap_int_0_0_to_R5FSS0_INTROUTER0_in_318_318,
    &ECAP2_ecap_int_0_0_to_R5FSS1_INTROUTER0_in_318_318,
};
static const struct Sciclient_rmIrqNode tisci_irq_ECAP2 = {
    .id = TISCI_DEV_ECAP2,
    .n_if   = 5,
    .p_if   = &tisci_if_ECAP2[0],
};

/* Start of EHRPWM0 interface definition */
const struct Sciclient_rmIrqIf EHRPWM0_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_33_33 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 33,
};
const struct Sciclient_rmIrqIf EHRPWM0_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_39_39 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 39,
};
const struct Sciclient_rmIrqIf EHRPWM0_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_33_33 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 33,
};
const struct Sciclient_rmIrqIf EHRPWM0_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_39_39 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 39,
};
const struct Sciclient_rmIrqIf EHRPWM0_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_2_2 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf EHRPWM0_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_8_8 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 8,
};
const struct Sciclient_rmIrqIf *const tisci_if_EHRPWM0[] = {
    &EHRPWM0_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_33_33,
    &EHRPWM0_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_39_39,
    &EHRPWM0_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_33_33,
    &EHRPWM0_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_39_39,
    &EHRPWM0_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_2_2,
    &EHRPWM0_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_8_8,
};
static const struct Sciclient_rmIrqNode tisci_irq_EHRPWM0 = {
    .id = TISCI_DEV_EHRPWM0,
    .n_if   = 6,
    .p_if   = &tisci_if_EHRPWM0[0],
};

/* Start of EHRPWM1 interface definition */
const struct Sciclient_rmIrqIf EHRPWM1_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_34_34 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 34,
};
const struct Sciclient_rmIrqIf EHRPWM1_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_40_40 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 40,
};
const struct Sciclient_rmIrqIf EHRPWM1_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_34_34 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 34,
};
const struct Sciclient_rmIrqIf EHRPWM1_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_40_40 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 40,
};
const struct Sciclient_rmIrqIf EHRPWM1_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_3_3 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf EHRPWM1_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_9_9 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 9,
};
const struct Sciclient_rmIrqIf *const tisci_if_EHRPWM1[] = {
    &EHRPWM1_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_34_34,
    &EHRPWM1_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_40_40,
    &EHRPWM1_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_34_34,
    &EHRPWM1_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_40_40,
    &EHRPWM1_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_3_3,
    &EHRPWM1_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_9_9,
};
static const struct Sciclient_rmIrqNode tisci_irq_EHRPWM1 = {
    .id = TISCI_DEV_EHRPWM1,
    .n_if   = 6,
    .p_if   = &tisci_if_EHRPWM1[0],
};

/* Start of EHRPWM2 interface definition */
const struct Sciclient_rmIrqIf EHRPWM2_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_35_35 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 35,
};
const struct Sciclient_rmIrqIf EHRPWM2_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_41_41 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 41,
};
const struct Sciclient_rmIrqIf EHRPWM2_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_35_35 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 35,
};
const struct Sciclient_rmIrqIf EHRPWM2_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_41_41 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 41,
};
const struct Sciclient_rmIrqIf EHRPWM2_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_4_4 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf EHRPWM2_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_10_10 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf *const tisci_if_EHRPWM2[] = {
    &EHRPWM2_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_35_35,
    &EHRPWM2_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_41_41,
    &EHRPWM2_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_35_35,
    &EHRPWM2_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_41_41,
    &EHRPWM2_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_4_4,
    &EHRPWM2_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_10_10,
};
static const struct Sciclient_rmIrqNode tisci_irq_EHRPWM2 = {
    .id = TISCI_DEV_EHRPWM2,
    .n_if   = 6,
    .p_if   = &tisci_if_EHRPWM2[0],
};

/* Start of EHRPWM3 interface definition */
const struct Sciclient_rmIrqIf EHRPWM3_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_36_36 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 36,
};
const struct Sciclient_rmIrqIf EHRPWM3_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_42_42 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 42,
};
const struct Sciclient_rmIrqIf EHRPWM3_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_36_36 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 36,
};
const struct Sciclient_rmIrqIf EHRPWM3_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_42_42 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 42,
};
const struct Sciclient_rmIrqIf EHRPWM3_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_5_5 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf EHRPWM3_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_11_11 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 11,
};
const struct Sciclient_rmIrqIf *const tisci_if_EHRPWM3[] = {
    &EHRPWM3_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_36_36,
    &EHRPWM3_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_42_42,
    &EHRPWM3_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_36_36,
    &EHRPWM3_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_42_42,
    &EHRPWM3_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_5_5,
    &EHRPWM3_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_11_11,
};
static const struct Sciclient_rmIrqNode tisci_irq_EHRPWM3 = {
    .id = TISCI_DEV_EHRPWM3,
    .n_if   = 6,
    .p_if   = &tisci_if_EHRPWM3[0],
};

/* Start of EHRPWM4 interface definition */
const struct Sciclient_rmIrqIf EHRPWM4_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_37_37 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 37,
};
const struct Sciclient_rmIrqIf EHRPWM4_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_43_43 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 43,
};
const struct Sciclient_rmIrqIf EHRPWM4_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_37_37 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 37,
};
const struct Sciclient_rmIrqIf EHRPWM4_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_43_43 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 43,
};
const struct Sciclient_rmIrqIf EHRPWM4_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_6_6 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 6,
};
const struct Sciclient_rmIrqIf EHRPWM4_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_12_12 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 12,
};
const struct Sciclient_rmIrqIf *const tisci_if_EHRPWM4[] = {
    &EHRPWM4_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_37_37,
    &EHRPWM4_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_43_43,
    &EHRPWM4_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_37_37,
    &EHRPWM4_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_43_43,
    &EHRPWM4_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_6_6,
    &EHRPWM4_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_12_12,
};
static const struct Sciclient_rmIrqNode tisci_irq_EHRPWM4 = {
    .id = TISCI_DEV_EHRPWM4,
    .n_if   = 6,
    .p_if   = &tisci_if_EHRPWM4[0],
};

/* Start of EHRPWM5 interface definition */
const struct Sciclient_rmIrqIf EHRPWM5_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_38_38 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 38,
};
const struct Sciclient_rmIrqIf EHRPWM5_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_44_44 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 44,
};
const struct Sciclient_rmIrqIf EHRPWM5_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_38_38 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 38,
};
const struct Sciclient_rmIrqIf EHRPWM5_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_44_44 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 44,
};
const struct Sciclient_rmIrqIf EHRPWM5_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_7_7 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf EHRPWM5_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_13_13 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 13,
};
const struct Sciclient_rmIrqIf *const tisci_if_EHRPWM5[] = {
    &EHRPWM5_epwm_etint_0_0_to_C66SS0_INTROUTER0_in_38_38,
    &EHRPWM5_epwm_tripzint_1_1_to_C66SS0_INTROUTER0_in_44_44,
    &EHRPWM5_epwm_etint_0_0_to_C66SS1_INTROUTER0_in_38_38,
    &EHRPWM5_epwm_tripzint_1_1_to_C66SS1_INTROUTER0_in_44_44,
    &EHRPWM5_epwm_etint_0_0_to_MAIN2MCU_PLS_INTRTR0_in_7_7,
    &EHRPWM5_epwm_tripzint_1_1_to_MAIN2MCU_PLS_INTRTR0_in_13_13,
};
static const struct Sciclient_rmIrqNode tisci_irq_EHRPWM5 = {
    .id = TISCI_DEV_EHRPWM5,
    .n_if   = 6,
    .p_if   = &tisci_if_EHRPWM5[0],
};

/* Start of ELM0 interface definition */
const struct Sciclient_rmIrqIf ELM0_elm_porocpsinterrupt_lvl_0_0_to_MAIN2MCU_LVL_INTRTR0_in_7_7 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf ELM0_elm_porocpsinterrupt_lvl_0_0_to_R5FSS0_INTROUTER0_in_230_230 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 230,
};
const struct Sciclient_rmIrqIf ELM0_elm_porocpsinterrupt_lvl_0_0_to_R5FSS1_INTROUTER0_in_230_230 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 230,
};
const struct Sciclient_rmIrqIf *const tisci_if_ELM0[] = {
    &ELM0_elm_porocpsinterrupt_lvl_0_0_to_MAIN2MCU_LVL_INTRTR0_in_7_7,
    &ELM0_elm_porocpsinterrupt_lvl_0_0_to_R5FSS0_INTROUTER0_in_230_230,
    &ELM0_elm_porocpsinterrupt_lvl_0_0_to_R5FSS1_INTROUTER0_in_230_230,
};
static const struct Sciclient_rmIrqNode tisci_irq_ELM0 = {
    .id = TISCI_DEV_ELM0,
    .n_if   = 3,
    .p_if   = &tisci_if_ELM0[0],
};

/* Start of MMCSD0 interface definition */
const struct Sciclient_rmIrqIf MMCSD0_emmcss_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_28_28 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 28,
};
const struct Sciclient_rmIrqIf *const tisci_if_MMCSD0[] = {
    &MMCSD0_emmcss_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_28_28,
};
static const struct Sciclient_rmIrqNode tisci_irq_MMCSD0 = {
    .id = TISCI_DEV_MMCSD0,
    .n_if   = 1,
    .p_if   = &tisci_if_MMCSD0[0],
};

/* Start of MMCSD1 interface definition */
const struct Sciclient_rmIrqIf MMCSD1_emmcsdss_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_29_29 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 29,
};
const struct Sciclient_rmIrqIf MMCSD1_emmcsdss_intr_0_0_to_R5FSS0_INTROUTER0_in_310_310 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 310,
};
const struct Sciclient_rmIrqIf MMCSD1_emmcsdss_intr_0_0_to_R5FSS1_INTROUTER0_in_310_310 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 310,
};
const struct Sciclient_rmIrqIf *const tisci_if_MMCSD1[] = {
    &MMCSD1_emmcsdss_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_29_29,
    &MMCSD1_emmcsdss_intr_0_0_to_R5FSS0_INTROUTER0_in_310_310,
    &MMCSD1_emmcsdss_intr_0_0_to_R5FSS1_INTROUTER0_in_310_310,
};
static const struct Sciclient_rmIrqNode tisci_irq_MMCSD1 = {
    .id = TISCI_DEV_MMCSD1,
    .n_if   = 3,
    .p_if   = &tisci_if_MMCSD1[0],
};

/* Start of MMCSD2 interface definition */
const struct Sciclient_rmIrqIf MMCSD2_emmcsdss_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_30_30 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 30,
};
const struct Sciclient_rmIrqIf MMCSD2_emmcsdss_intr_0_0_to_R5FSS0_INTROUTER0_in_311_311 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 311,
};
const struct Sciclient_rmIrqIf MMCSD2_emmcsdss_intr_0_0_to_R5FSS1_INTROUTER0_in_311_311 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 311,
};
const struct Sciclient_rmIrqIf *const tisci_if_MMCSD2[] = {
    &MMCSD2_emmcsdss_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_30_30,
    &MMCSD2_emmcsdss_intr_0_0_to_R5FSS0_INTROUTER0_in_311_311,
    &MMCSD2_emmcsdss_intr_0_0_to_R5FSS1_INTROUTER0_in_311_311,
};
static const struct Sciclient_rmIrqNode tisci_irq_MMCSD2 = {
    .id = TISCI_DEV_MMCSD2,
    .n_if   = 3,
    .p_if   = &tisci_if_MMCSD2[0],
};

/* Start of EQEP0 interface definition */
const struct Sciclient_rmIrqIf EQEP0_eqep_int_0_0_to_C66SS0_INTROUTER0_in_48_48 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf EQEP0_eqep_int_0_0_to_C66SS1_INTROUTER0_in_48_48 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf EQEP0_eqep_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_14_14 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 14,
};
const struct Sciclient_rmIrqIf *const tisci_if_EQEP0[] = {
    &EQEP0_eqep_int_0_0_to_C66SS0_INTROUTER0_in_48_48,
    &EQEP0_eqep_int_0_0_to_C66SS1_INTROUTER0_in_48_48,
    &EQEP0_eqep_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_14_14,
};
static const struct Sciclient_rmIrqNode tisci_irq_EQEP0 = {
    .id = TISCI_DEV_EQEP0,
    .n_if   = 3,
    .p_if   = &tisci_if_EQEP0[0],
};

/* Start of EQEP1 interface definition */
const struct Sciclient_rmIrqIf EQEP1_eqep_int_0_0_to_C66SS0_INTROUTER0_in_49_49 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 49,
};
const struct Sciclient_rmIrqIf EQEP1_eqep_int_0_0_to_C66SS1_INTROUTER0_in_49_49 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 49,
};
const struct Sciclient_rmIrqIf EQEP1_eqep_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_15_15 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 15,
};
const struct Sciclient_rmIrqIf *const tisci_if_EQEP1[] = {
    &EQEP1_eqep_int_0_0_to_C66SS0_INTROUTER0_in_49_49,
    &EQEP1_eqep_int_0_0_to_C66SS1_INTROUTER0_in_49_49,
    &EQEP1_eqep_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_15_15,
};
static const struct Sciclient_rmIrqNode tisci_irq_EQEP1 = {
    .id = TISCI_DEV_EQEP1,
    .n_if   = 3,
    .p_if   = &tisci_if_EQEP1[0],
};

/* Start of EQEP2 interface definition */
const struct Sciclient_rmIrqIf EQEP2_eqep_int_0_0_to_C66SS0_INTROUTER0_in_50_50 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 50,
};
const struct Sciclient_rmIrqIf EQEP2_eqep_int_0_0_to_C66SS1_INTROUTER0_in_50_50 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 50,
};
const struct Sciclient_rmIrqIf EQEP2_eqep_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_16_16 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 16,
};
const struct Sciclient_rmIrqIf *const tisci_if_EQEP2[] = {
    &EQEP2_eqep_int_0_0_to_C66SS0_INTROUTER0_in_50_50,
    &EQEP2_eqep_int_0_0_to_C66SS1_INTROUTER0_in_50_50,
    &EQEP2_eqep_int_0_0_to_MAIN2MCU_PLS_INTRTR0_in_16_16,
};
static const struct Sciclient_rmIrqNode tisci_irq_EQEP2 = {
    .id = TISCI_DEV_EQEP2,
    .n_if   = 3,
    .p_if   = &tisci_if_EQEP2[0],
};

/* Start of ESM0 interface definition */
const struct Sciclient_rmIrqIf ESM0_esm_int_low_lvl_2_2_to_C66SS0_INTROUTER0_in_123_123 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 123,
};
const struct Sciclient_rmIrqIf ESM0_esm_int_hi_lvl_1_1_to_C66SS0_INTROUTER0_in_124_124 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 124,
};
const struct Sciclient_rmIrqIf ESM0_esm_int_cfg_lvl_0_0_to_C66SS0_INTROUTER0_in_125_125 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 125,
};
const struct Sciclient_rmIrqIf ESM0_esm_int_low_lvl_2_2_to_C66SS1_INTROUTER0_in_122_122 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 122,
};
const struct Sciclient_rmIrqIf ESM0_esm_int_hi_lvl_1_1_to_C66SS1_INTROUTER0_in_123_123 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 123,
};
const struct Sciclient_rmIrqIf ESM0_esm_int_cfg_lvl_0_0_to_C66SS1_INTROUTER0_in_124_124 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 124,
};
const struct Sciclient_rmIrqIf *const tisci_if_ESM0[] = {
    &ESM0_esm_int_low_lvl_2_2_to_C66SS0_INTROUTER0_in_123_123,
    &ESM0_esm_int_hi_lvl_1_1_to_C66SS0_INTROUTER0_in_124_124,
    &ESM0_esm_int_cfg_lvl_0_0_to_C66SS0_INTROUTER0_in_125_125,
    &ESM0_esm_int_low_lvl_2_2_to_C66SS1_INTROUTER0_in_122_122,
    &ESM0_esm_int_hi_lvl_1_1_to_C66SS1_INTROUTER0_in_123_123,
    &ESM0_esm_int_cfg_lvl_0_0_to_C66SS1_INTROUTER0_in_124_124,
};
static const struct Sciclient_rmIrqNode tisci_irq_ESM0 = {
    .id = TISCI_DEV_ESM0,
    .n_if   = 6,
    .p_if   = &tisci_if_ESM0[0],
};

/* Start of MCU_ESM0 interface definition */
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_low_lvl_2_2_to_C66SS0_INTROUTER0_in_296_296 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 296,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_hi_lvl_1_1_to_C66SS0_INTROUTER0_in_297_297 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 297,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_cfg_lvl_0_0_to_C66SS0_INTROUTER0_in_298_298 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 298,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_low_lvl_2_2_to_C66SS1_INTROUTER0_in_296_296 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 296,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_hi_lvl_1_1_to_C66SS1_INTROUTER0_in_297_297 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 297,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_cfg_lvl_0_0_to_C66SS1_INTROUTER0_in_298_298 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 298,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_low_lvl_2_2_to_R5FSS0_INTROUTER0_in_149_149 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 149,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_hi_lvl_1_1_to_R5FSS0_INTROUTER0_in_150_150 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 150,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_cfg_lvl_0_0_to_R5FSS0_INTROUTER0_in_151_151 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 151,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_low_lvl_2_2_to_R5FSS1_INTROUTER0_in_149_149 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 149,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_hi_lvl_1_1_to_R5FSS1_INTROUTER0_in_150_150 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 150,
};
const struct Sciclient_rmIrqIf MCU_ESM0_esm_int_cfg_lvl_0_0_to_R5FSS1_INTROUTER0_in_151_151 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 151,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_ESM0[] = {
    &MCU_ESM0_esm_int_low_lvl_2_2_to_C66SS0_INTROUTER0_in_296_296,
    &MCU_ESM0_esm_int_hi_lvl_1_1_to_C66SS0_INTROUTER0_in_297_297,
    &MCU_ESM0_esm_int_cfg_lvl_0_0_to_C66SS0_INTROUTER0_in_298_298,
    &MCU_ESM0_esm_int_low_lvl_2_2_to_C66SS1_INTROUTER0_in_296_296,
    &MCU_ESM0_esm_int_hi_lvl_1_1_to_C66SS1_INTROUTER0_in_297_297,
    &MCU_ESM0_esm_int_cfg_lvl_0_0_to_C66SS1_INTROUTER0_in_298_298,
    &MCU_ESM0_esm_int_low_lvl_2_2_to_R5FSS0_INTROUTER0_in_149_149,
    &MCU_ESM0_esm_int_hi_lvl_1_1_to_R5FSS0_INTROUTER0_in_150_150,
    &MCU_ESM0_esm_int_cfg_lvl_0_0_to_R5FSS0_INTROUTER0_in_151_151,
    &MCU_ESM0_esm_int_low_lvl_2_2_to_R5FSS1_INTROUTER0_in_149_149,
    &MCU_ESM0_esm_int_hi_lvl_1_1_to_R5FSS1_INTROUTER0_in_150_150,
    &MCU_ESM0_esm_int_cfg_lvl_0_0_to_R5FSS1_INTROUTER0_in_151_151,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_ESM0 = {
    .id = TISCI_DEV_MCU_ESM0,
    .n_if   = 12,
    .p_if   = &tisci_if_MCU_ESM0[0],
};

/* Start of WKUP_ESM0 interface definition */
const struct Sciclient_rmIrqIf WKUP_ESM0_esm_int_low_lvl_2_2_to_C66SS0_INTROUTER0_in_299_299 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 299,
};
const struct Sciclient_rmIrqIf WKUP_ESM0_esm_int_hi_lvl_1_1_to_C66SS0_INTROUTER0_in_300_300 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 300,
};
const struct Sciclient_rmIrqIf WKUP_ESM0_esm_int_cfg_lvl_0_0_to_C66SS0_INTROUTER0_in_301_301 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 301,
};
const struct Sciclient_rmIrqIf WKUP_ESM0_esm_int_low_lvl_2_2_to_C66SS1_INTROUTER0_in_299_299 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 299,
};
const struct Sciclient_rmIrqIf WKUP_ESM0_esm_int_hi_lvl_1_1_to_C66SS1_INTROUTER0_in_300_300 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 300,
};
const struct Sciclient_rmIrqIf WKUP_ESM0_esm_int_cfg_lvl_0_0_to_C66SS1_INTROUTER0_in_301_301 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 301,
};
const struct Sciclient_rmIrqIf *const tisci_if_WKUP_ESM0[] = {
    &WKUP_ESM0_esm_int_low_lvl_2_2_to_C66SS0_INTROUTER0_in_299_299,
    &WKUP_ESM0_esm_int_hi_lvl_1_1_to_C66SS0_INTROUTER0_in_300_300,
    &WKUP_ESM0_esm_int_cfg_lvl_0_0_to_C66SS0_INTROUTER0_in_301_301,
    &WKUP_ESM0_esm_int_low_lvl_2_2_to_C66SS1_INTROUTER0_in_299_299,
    &WKUP_ESM0_esm_int_hi_lvl_1_1_to_C66SS1_INTROUTER0_in_300_300,
    &WKUP_ESM0_esm_int_cfg_lvl_0_0_to_C66SS1_INTROUTER0_in_301_301,
};
static const struct Sciclient_rmIrqNode tisci_irq_WKUP_ESM0 = {
    .id = TISCI_DEV_WKUP_ESM0,
    .n_if   = 6,
    .p_if   = &tisci_if_WKUP_ESM0[0],
};

/* Start of MCU_FSS0_FSAS_0 interface definition */
const struct Sciclient_rmIrqIf MCU_FSS0_FSAS_0_otfa_intr_err_pend_1_1_to_R5FSS0_INTROUTER0_in_145_145 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 145,
};
const struct Sciclient_rmIrqIf MCU_FSS0_FSAS_0_ecc_intr_err_pend_0_0_to_R5FSS0_INTROUTER0_in_146_146 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 146,
};
const struct Sciclient_rmIrqIf MCU_FSS0_FSAS_0_otfa_intr_err_pend_1_1_to_R5FSS1_INTROUTER0_in_145_145 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 145,
};
const struct Sciclient_rmIrqIf MCU_FSS0_FSAS_0_ecc_intr_err_pend_0_0_to_R5FSS1_INTROUTER0_in_146_146 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 146,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_FSS0_FSAS_0[] = {
    &MCU_FSS0_FSAS_0_otfa_intr_err_pend_1_1_to_R5FSS0_INTROUTER0_in_145_145,
    &MCU_FSS0_FSAS_0_ecc_intr_err_pend_0_0_to_R5FSS0_INTROUTER0_in_146_146,
    &MCU_FSS0_FSAS_0_otfa_intr_err_pend_1_1_to_R5FSS1_INTROUTER0_in_145_145,
    &MCU_FSS0_FSAS_0_ecc_intr_err_pend_0_0_to_R5FSS1_INTROUTER0_in_146_146,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_FSS0_FSAS_0 = {
    .id = TISCI_DEV_MCU_FSS0_FSAS_0,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_FSS0_FSAS_0[0],
};

/* Start of MCU_FSS0_HYPERBUS1P0_0 interface definition */
const struct Sciclient_rmIrqIf MCU_FSS0_HYPERBUS1P0_0_hpb_intr_0_0_to_R5FSS0_INTROUTER0_in_144_144 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 144,
};
const struct Sciclient_rmIrqIf MCU_FSS0_HYPERBUS1P0_0_hpb_intr_0_0_to_R5FSS1_INTROUTER0_in_144_144 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 144,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_FSS0_HYPERBUS1P0_0[] = {
    &MCU_FSS0_HYPERBUS1P0_0_hpb_intr_0_0_to_R5FSS0_INTROUTER0_in_144_144,
    &MCU_FSS0_HYPERBUS1P0_0_hpb_intr_0_0_to_R5FSS1_INTROUTER0_in_144_144,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_FSS0_HYPERBUS1P0_0 = {
    .id = TISCI_DEV_MCU_FSS0_HYPERBUS1P0_0,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_FSS0_HYPERBUS1P0_0[0],
};

/* Start of MCU_FSS0_OSPI_0 interface definition */
const struct Sciclient_rmIrqIf MCU_FSS0_OSPI_0_ospi_lvl_intr_0_0_to_R5FSS0_INTROUTER0_in_142_142 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 142,
};
const struct Sciclient_rmIrqIf MCU_FSS0_OSPI_0_ospi_lvl_intr_0_0_to_R5FSS1_INTROUTER0_in_142_142 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 142,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_FSS0_OSPI_0[] = {
    &MCU_FSS0_OSPI_0_ospi_lvl_intr_0_0_to_R5FSS0_INTROUTER0_in_142_142,
    &MCU_FSS0_OSPI_0_ospi_lvl_intr_0_0_to_R5FSS1_INTROUTER0_in_142_142,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_FSS0_OSPI_0 = {
    .id = TISCI_DEV_MCU_FSS0_OSPI_0,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_FSS0_OSPI_0[0],
};

/* Start of MCU_FSS0_OSPI_1 interface definition */
const struct Sciclient_rmIrqIf MCU_FSS0_OSPI_1_ospi_lvl_intr_0_0_to_R5FSS0_INTROUTER0_in_143_143 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 143,
};
const struct Sciclient_rmIrqIf MCU_FSS0_OSPI_1_ospi_lvl_intr_0_0_to_R5FSS1_INTROUTER0_in_143_143 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 143,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_FSS0_OSPI_1[] = {
    &MCU_FSS0_OSPI_1_ospi_lvl_intr_0_0_to_R5FSS0_INTROUTER0_in_143_143,
    &MCU_FSS0_OSPI_1_ospi_lvl_intr_0_0_to_R5FSS1_INTROUTER0_in_143_143,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_FSS0_OSPI_1 = {
    .id = TISCI_DEV_MCU_FSS0_OSPI_1,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_FSS0_OSPI_1[0],
};

/* Start of GPIO0 interface definition */
const struct Sciclient_rmIrqIf GPIO0_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_256_263 = {
    .lbase  = 0,
    .len    = 8,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO0[] = {
    &GPIO0_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_256_263,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO0 = {
    .id = TISCI_DEV_GPIO0,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO0[0],
};

/* Start of GPIO1 interface definition */
const struct Sciclient_rmIrqIf GPIO1_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_288_290 = {
    .lbase  = 0,
    .len    = 3,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 288,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO1[] = {
    &GPIO1_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_288_290,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO1 = {
    .id = TISCI_DEV_GPIO1,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO1[0],
};

/* Start of GPIO2 interface definition */
const struct Sciclient_rmIrqIf GPIO2_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_264_271 = {
    .lbase  = 0,
    .len    = 8,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 264,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO2[] = {
    &GPIO2_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_264_271,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO2 = {
    .id = TISCI_DEV_GPIO2,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO2[0],
};

/* Start of GPIO3 interface definition */
const struct Sciclient_rmIrqIf GPIO3_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_292_294 = {
    .lbase  = 0,
    .len    = 3,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 292,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO3[] = {
    &GPIO3_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_292_294,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO3 = {
    .id = TISCI_DEV_GPIO3,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO3[0],
};

/* Start of GPIO4 interface definition */
const struct Sciclient_rmIrqIf GPIO4_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_272_279 = {
    .lbase  = 0,
    .len    = 8,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 272,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO4[] = {
    &GPIO4_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_272_279,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO4 = {
    .id = TISCI_DEV_GPIO4,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO4[0],
};

/* Start of GPIO5 interface definition */
const struct Sciclient_rmIrqIf GPIO5_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_296_298 = {
    .lbase  = 0,
    .len    = 3,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 296,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO5[] = {
    &GPIO5_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_296_298,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO5 = {
    .id = TISCI_DEV_GPIO5,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO5[0],
};

/* Start of GPIO6 interface definition */
const struct Sciclient_rmIrqIf GPIO6_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_280_287 = {
    .lbase  = 0,
    .len    = 8,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 280,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO6[] = {
    &GPIO6_gpio_bank_0_7_to_GPIOMUX_INTRTR0_in_280_287,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO6 = {
    .id = TISCI_DEV_GPIO6,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO6[0],
};

/* Start of GPIO7 interface definition */
const struct Sciclient_rmIrqIf GPIO7_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_300_302 = {
    .lbase  = 0,
    .len    = 3,
    .rid    = TISCI_DEV_GPIOMUX_INTRTR0,
    .rbase  = 300,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIO7[] = {
    &GPIO7_gpio_bank_0_2_to_GPIOMUX_INTRTR0_in_300_302,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIO7 = {
    .id = TISCI_DEV_GPIO7,
    .n_if   = 1,
    .p_if   = &tisci_if_GPIO7[0],
};

/* Start of WKUP_GPIO0 interface definition */
const struct Sciclient_rmIrqIf WKUP_GPIO0_gpio_bank_0_5_to_WKUP_GPIOMUX_INTRTR0_in_103_108 = {
    .lbase  = 0,
    .len    = 6,
    .rid    = TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
    .rbase  = 103,
};
const struct Sciclient_rmIrqIf *const tisci_if_WKUP_GPIO0[] = {
    &WKUP_GPIO0_gpio_bank_0_5_to_WKUP_GPIOMUX_INTRTR0_in_103_108,
};
static const struct Sciclient_rmIrqNode tisci_irq_WKUP_GPIO0 = {
    .id = TISCI_DEV_WKUP_GPIO0,
    .n_if   = 1,
    .p_if   = &tisci_if_WKUP_GPIO0[0],
};

/* Start of WKUP_GPIO1 interface definition */
const struct Sciclient_rmIrqIf WKUP_GPIO1_gpio_bank_0_5_to_WKUP_GPIOMUX_INTRTR0_in_112_117 = {
    .lbase  = 0,
    .len    = 6,
    .rid    = TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
    .rbase  = 112,
};
const struct Sciclient_rmIrqIf *const tisci_if_WKUP_GPIO1[] = {
    &WKUP_GPIO1_gpio_bank_0_5_to_WKUP_GPIOMUX_INTRTR0_in_112_117,
};
static const struct Sciclient_rmIrqNode tisci_irq_WKUP_GPIO1 = {
    .id = TISCI_DEV_WKUP_GPIO1,
    .n_if   = 1,
    .p_if   = &tisci_if_WKUP_GPIO1[0],
};

/* Start of GPMC0 interface definition */
const struct Sciclient_rmIrqIf GPMC0_gpmc_sinterrupt_0_0_to_MAIN2MCU_LVL_INTRTR0_in_8_8 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 8,
};
const struct Sciclient_rmIrqIf GPMC0_gpmc_sinterrupt_0_0_to_R5FSS0_INTROUTER0_in_229_229 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 229,
};
const struct Sciclient_rmIrqIf GPMC0_gpmc_sinterrupt_0_0_to_R5FSS1_INTROUTER0_in_229_229 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 229,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPMC0[] = {
    &GPMC0_gpmc_sinterrupt_0_0_to_MAIN2MCU_LVL_INTRTR0_in_8_8,
    &GPMC0_gpmc_sinterrupt_0_0_to_R5FSS0_INTROUTER0_in_229_229,
    &GPMC0_gpmc_sinterrupt_0_0_to_R5FSS1_INTROUTER0_in_229_229,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPMC0 = {
    .id = TISCI_DEV_GPMC0,
    .n_if   = 3,
    .p_if   = &tisci_if_GPMC0[0],
};

/* Start of I3C0 interface definition */
const struct Sciclient_rmIrqIf I3C0_i3c__int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_220_220 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 220,
};
const struct Sciclient_rmIrqIf I3C0_i3c__int_0_0_to_C66SS0_INTROUTER0_in_339_339 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 339,
};
const struct Sciclient_rmIrqIf I3C0_i3c__int_0_0_to_C66SS1_INTROUTER0_in_339_339 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 339,
};
const struct Sciclient_rmIrqIf I3C0_i3c__int_0_0_to_R5FSS0_INTROUTER0_in_201_201 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 201,
};
const struct Sciclient_rmIrqIf I3C0_i3c__int_0_0_to_R5FSS1_INTROUTER0_in_201_201 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 201,
};
const struct Sciclient_rmIrqIf *const tisci_if_I3C0[] = {
    &I3C0_i3c__int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_220_220,
    &I3C0_i3c__int_0_0_to_C66SS0_INTROUTER0_in_339_339,
    &I3C0_i3c__int_0_0_to_C66SS1_INTROUTER0_in_339_339,
    &I3C0_i3c__int_0_0_to_R5FSS0_INTROUTER0_in_201_201,
    &I3C0_i3c__int_0_0_to_R5FSS1_INTROUTER0_in_201_201,
};
static const struct Sciclient_rmIrqNode tisci_irq_I3C0 = {
    .id = TISCI_DEV_I3C0,
    .n_if   = 5,
    .p_if   = &tisci_if_I3C0[0],
};

/* Start of MCU_I3C0 interface definition */
const struct Sciclient_rmIrqIf MCU_I3C0_i3c__int_0_0_to_C66SS0_INTROUTER0_in_354_354 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 354,
};
const struct Sciclient_rmIrqIf MCU_I3C0_i3c__int_0_0_to_C66SS1_INTROUTER0_in_354_354 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 354,
};
const struct Sciclient_rmIrqIf MCU_I3C0_i3c__int_0_0_to_R5FSS0_INTROUTER0_in_140_140 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 140,
};
const struct Sciclient_rmIrqIf MCU_I3C0_i3c__int_0_0_to_R5FSS1_INTROUTER0_in_140_140 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 140,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_I3C0[] = {
    &MCU_I3C0_i3c__int_0_0_to_C66SS0_INTROUTER0_in_354_354,
    &MCU_I3C0_i3c__int_0_0_to_C66SS1_INTROUTER0_in_354_354,
    &MCU_I3C0_i3c__int_0_0_to_R5FSS0_INTROUTER0_in_140_140,
    &MCU_I3C0_i3c__int_0_0_to_R5FSS1_INTROUTER0_in_140_140,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_I3C0 = {
    .id = TISCI_DEV_MCU_I3C0,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_I3C0[0],
};

/* Start of MCU_I3C1 interface definition */
const struct Sciclient_rmIrqIf MCU_I3C1_i3c__int_0_0_to_C66SS0_INTROUTER0_in_355_355 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 355,
};
const struct Sciclient_rmIrqIf MCU_I3C1_i3c__int_0_0_to_C66SS1_INTROUTER0_in_355_355 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 355,
};
const struct Sciclient_rmIrqIf MCU_I3C1_i3c__int_0_0_to_R5FSS0_INTROUTER0_in_141_141 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 141,
};
const struct Sciclient_rmIrqIf MCU_I3C1_i3c__int_0_0_to_R5FSS1_INTROUTER0_in_141_141 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 141,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_I3C1[] = {
    &MCU_I3C1_i3c__int_0_0_to_C66SS0_INTROUTER0_in_355_355,
    &MCU_I3C1_i3c__int_0_0_to_C66SS1_INTROUTER0_in_355_355,
    &MCU_I3C1_i3c__int_0_0_to_R5FSS0_INTROUTER0_in_141_141,
    &MCU_I3C1_i3c__int_0_0_to_R5FSS1_INTROUTER0_in_141_141,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_I3C1 = {
    .id = TISCI_DEV_MCU_I3C1,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_I3C1[0],
};

/* Start of PRU_ICSSG0 interface definition */
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_host_intr_pend_4_11_to_MAIN2MCU_LVL_INTRTR0_in_32_39 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 32,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_MAIN2MCU_PLS_INTRTR0_in_20_21 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 20,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_MAIN2MCU_PLS_INTRTR0_in_22_23 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 22,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_host_intr_pend_4_11_to_R5FSS0_INTROUTER0_in_30_37 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 30,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_R5FSS0_INTROUTER0_in_46_47 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 46,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_R5FSS0_INTROUTER0_in_48_49 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_host_intr_pend_4_11_to_R5FSS1_INTROUTER0_in_30_37 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 30,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_R5FSS1_INTROUTER0_in_46_47 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 46,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_R5FSS1_INTROUTER0_in_48_49 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_edc0_sync0_out_0_0_to_TIMESYNC_INTRTR0_in_24_24 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_edc0_sync1_out_1_1_to_TIMESYNC_INTRTR0_in_25_25 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 25,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_edc1_sync0_out_2_2_to_TIMESYNC_INTRTR0_in_26_26 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 26,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_edc1_sync1_out_3_3_to_TIMESYNC_INTRTR0_in_27_27 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 27,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_host_intr_pend_4_11_to_C66SS0_INTROUTER0_in_152_159 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 152,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_C66SS0_INTROUTER0_in_168_169 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 168,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_C66SS0_INTROUTER0_in_170_171 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 170,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_host_intr_pend_4_11_to_C66SS1_INTROUTER0_in_152_159 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 152,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_C66SS1_INTROUTER0_in_168_169 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 168,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_C66SS1_INTROUTER0_in_170_171 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 170,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_host_intr_req_12_19_to_CMPEVENT_INTRTR0_in_16_23 = {
    .lbase  = 12,
    .len    = 8,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 16,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_iep0_cmp_intr_req_20_35_to_CMPEVENT_INTRTR0_in_32_47 = {
    .lbase  = 20,
    .len    = 16,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 32,
};
const struct Sciclient_rmIrqIf PRU_ICSSG0_pr1_iep1_cmp_intr_req_36_51_to_CMPEVENT_INTRTR0_in_48_63 = {
    .lbase  = 36,
    .len    = 16,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf *const tisci_if_PRU_ICSSG0[] = {
    &PRU_ICSSG0_pr1_host_intr_pend_4_11_to_MAIN2MCU_LVL_INTRTR0_in_32_39,
    &PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_MAIN2MCU_PLS_INTRTR0_in_20_21,
    &PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_MAIN2MCU_PLS_INTRTR0_in_22_23,
    &PRU_ICSSG0_pr1_host_intr_pend_4_11_to_R5FSS0_INTROUTER0_in_30_37,
    &PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_R5FSS0_INTROUTER0_in_46_47,
    &PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_R5FSS0_INTROUTER0_in_48_49,
    &PRU_ICSSG0_pr1_host_intr_pend_4_11_to_R5FSS1_INTROUTER0_in_30_37,
    &PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_R5FSS1_INTROUTER0_in_46_47,
    &PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_R5FSS1_INTROUTER0_in_48_49,
    &PRU_ICSSG0_pr1_edc0_sync0_out_0_0_to_TIMESYNC_INTRTR0_in_24_24,
    &PRU_ICSSG0_pr1_edc0_sync1_out_1_1_to_TIMESYNC_INTRTR0_in_25_25,
    &PRU_ICSSG0_pr1_edc1_sync0_out_2_2_to_TIMESYNC_INTRTR0_in_26_26,
    &PRU_ICSSG0_pr1_edc1_sync1_out_3_3_to_TIMESYNC_INTRTR0_in_27_27,
    &PRU_ICSSG0_pr1_host_intr_pend_4_11_to_C66SS0_INTROUTER0_in_152_159,
    &PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_C66SS0_INTROUTER0_in_168_169,
    &PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_C66SS0_INTROUTER0_in_170_171,
    &PRU_ICSSG0_pr1_host_intr_pend_4_11_to_C66SS1_INTROUTER0_in_152_159,
    &PRU_ICSSG0_pr1_tx_sof_intr_req_54_55_to_C66SS1_INTROUTER0_in_168_169,
    &PRU_ICSSG0_pr1_rx_sof_intr_req_52_53_to_C66SS1_INTROUTER0_in_170_171,
    &PRU_ICSSG0_pr1_host_intr_req_12_19_to_CMPEVENT_INTRTR0_in_16_23,
    &PRU_ICSSG0_pr1_iep0_cmp_intr_req_20_35_to_CMPEVENT_INTRTR0_in_32_47,
    &PRU_ICSSG0_pr1_iep1_cmp_intr_req_36_51_to_CMPEVENT_INTRTR0_in_48_63,
};
static const struct Sciclient_rmIrqNode tisci_irq_PRU_ICSSG0 = {
    .id = TISCI_DEV_PRU_ICSSG0,
    .n_if   = 22,
    .p_if   = &tisci_if_PRU_ICSSG0[0],
};

/* Start of PRU_ICSSG1 interface definition */
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_host_intr_pend_4_11_to_MAIN2MCU_LVL_INTRTR0_in_40_47 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 40,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_MAIN2MCU_PLS_INTRTR0_in_24_25 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_MAIN2MCU_PLS_INTRTR0_in_26_27 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 26,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_host_intr_pend_4_11_to_R5FSS0_INTROUTER0_in_38_45 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 38,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_R5FSS0_INTROUTER0_in_50_51 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 50,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_R5FSS0_INTROUTER0_in_52_53 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 52,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_host_intr_pend_4_11_to_R5FSS1_INTROUTER0_in_38_45 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 38,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_R5FSS1_INTROUTER0_in_50_51 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 50,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_R5FSS1_INTROUTER0_in_52_53 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 52,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_edc0_sync0_out_0_0_to_TIMESYNC_INTRTR0_in_28_28 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 28,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_edc0_sync1_out_1_1_to_TIMESYNC_INTRTR0_in_29_29 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 29,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_edc1_sync0_out_2_2_to_TIMESYNC_INTRTR0_in_30_30 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 30,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_edc1_sync1_out_3_3_to_TIMESYNC_INTRTR0_in_31_31 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 31,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_host_intr_pend_4_11_to_C66SS0_INTROUTER0_in_160_167 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 160,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_C66SS0_INTROUTER0_in_172_173 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 172,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_C66SS0_INTROUTER0_in_174_175 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 174,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_host_intr_pend_4_11_to_C66SS1_INTROUTER0_in_160_167 = {
    .lbase  = 4,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 160,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_C66SS1_INTROUTER0_in_172_173 = {
    .lbase  = 54,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 172,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_C66SS1_INTROUTER0_in_174_175 = {
    .lbase  = 52,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 174,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_host_intr_req_12_19_to_CMPEVENT_INTRTR0_in_24_31 = {
    .lbase  = 12,
    .len    = 8,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_iep0_cmp_intr_req_20_35_to_CMPEVENT_INTRTR0_in_64_79 = {
    .lbase  = 20,
    .len    = 16,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf PRU_ICSSG1_pr1_iep1_cmp_intr_req_36_51_to_CMPEVENT_INTRTR0_in_80_95 = {
    .lbase  = 36,
    .len    = 16,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 80,
};
const struct Sciclient_rmIrqIf *const tisci_if_PRU_ICSSG1[] = {
    &PRU_ICSSG1_pr1_host_intr_pend_4_11_to_MAIN2MCU_LVL_INTRTR0_in_40_47,
    &PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_MAIN2MCU_PLS_INTRTR0_in_24_25,
    &PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_MAIN2MCU_PLS_INTRTR0_in_26_27,
    &PRU_ICSSG1_pr1_host_intr_pend_4_11_to_R5FSS0_INTROUTER0_in_38_45,
    &PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_R5FSS0_INTROUTER0_in_50_51,
    &PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_R5FSS0_INTROUTER0_in_52_53,
    &PRU_ICSSG1_pr1_host_intr_pend_4_11_to_R5FSS1_INTROUTER0_in_38_45,
    &PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_R5FSS1_INTROUTER0_in_50_51,
    &PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_R5FSS1_INTROUTER0_in_52_53,
    &PRU_ICSSG1_pr1_edc0_sync0_out_0_0_to_TIMESYNC_INTRTR0_in_28_28,
    &PRU_ICSSG1_pr1_edc0_sync1_out_1_1_to_TIMESYNC_INTRTR0_in_29_29,
    &PRU_ICSSG1_pr1_edc1_sync0_out_2_2_to_TIMESYNC_INTRTR0_in_30_30,
    &PRU_ICSSG1_pr1_edc1_sync1_out_3_3_to_TIMESYNC_INTRTR0_in_31_31,
    &PRU_ICSSG1_pr1_host_intr_pend_4_11_to_C66SS0_INTROUTER0_in_160_167,
    &PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_C66SS0_INTROUTER0_in_172_173,
    &PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_C66SS0_INTROUTER0_in_174_175,
    &PRU_ICSSG1_pr1_host_intr_pend_4_11_to_C66SS1_INTROUTER0_in_160_167,
    &PRU_ICSSG1_pr1_tx_sof_intr_req_54_55_to_C66SS1_INTROUTER0_in_172_173,
    &PRU_ICSSG1_pr1_rx_sof_intr_req_52_53_to_C66SS1_INTROUTER0_in_174_175,
    &PRU_ICSSG1_pr1_host_intr_req_12_19_to_CMPEVENT_INTRTR0_in_24_31,
    &PRU_ICSSG1_pr1_iep0_cmp_intr_req_20_35_to_CMPEVENT_INTRTR0_in_64_79,
    &PRU_ICSSG1_pr1_iep1_cmp_intr_req_36_51_to_CMPEVENT_INTRTR0_in_80_95,
};
static const struct Sciclient_rmIrqNode tisci_irq_PRU_ICSSG1 = {
    .id = TISCI_DEV_PRU_ICSSG1,
    .n_if   = 22,
    .p_if   = &tisci_if_PRU_ICSSG1[0],
};

/* Start of C66SS0_INTROUTER0 interface definition */
const struct Sciclient_rmIrqIf C66SS0_INTROUTER0_outl_0_4_to_C66SS0_CORE0_c66_event_in_sync_4_8 = {
    .lbase  = 0,
    .len    = 5,
    .rid    = TISCI_DEV_C66SS0_CORE0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf C66SS0_INTROUTER0_outl_5_85_to_C66SS0_CORE0_c66_event_in_sync_15_95 = {
    .lbase  = 5,
    .len    = 81,
    .rid    = TISCI_DEV_C66SS0_CORE0,
    .rbase  = 15,
};
const struct Sciclient_rmIrqIf C66SS0_INTROUTER0_outl_86_86_to_C66SS0_CORE0_c66_event_in_sync_99_99 = {
    .lbase  = 86,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_CORE0,
    .rbase  = 99,
};
const struct Sciclient_rmIrqIf C66SS0_INTROUTER0_outl_87_94_to_C66SS0_CORE0_c66_event_in_sync_102_109 = {
    .lbase  = 87,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS0_CORE0,
    .rbase  = 102,
};
const struct Sciclient_rmIrqIf C66SS0_INTROUTER0_outl_95_96_to_C66SS0_CORE0_c66_event_in_sync_114_115 = {
    .lbase  = 95,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_CORE0,
    .rbase  = 114,
};
const struct Sciclient_rmIrqIf *const tisci_if_C66SS0_INTROUTER0[] = {
    &C66SS0_INTROUTER0_outl_0_4_to_C66SS0_CORE0_c66_event_in_sync_4_8,
    &C66SS0_INTROUTER0_outl_5_85_to_C66SS0_CORE0_c66_event_in_sync_15_95,
    &C66SS0_INTROUTER0_outl_86_86_to_C66SS0_CORE0_c66_event_in_sync_99_99,
    &C66SS0_INTROUTER0_outl_87_94_to_C66SS0_CORE0_c66_event_in_sync_102_109,
    &C66SS0_INTROUTER0_outl_95_96_to_C66SS0_CORE0_c66_event_in_sync_114_115,
};
static const struct Sciclient_rmIrqNode tisci_irq_C66SS0_INTROUTER0 = {
    .id = TISCI_DEV_C66SS0_INTROUTER0,
    .n_if   = 5,
    .p_if   = &tisci_if_C66SS0_INTROUTER0[0],
};

/* Start of C66SS1_INTROUTER0 interface definition */
const struct Sciclient_rmIrqIf C66SS1_INTROUTER0_outl_0_4_to_C66SS1_CORE0_c66_event_in_sync_4_8 = {
    .lbase  = 0,
    .len    = 5,
    .rid    = TISCI_DEV_C66SS1_CORE0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf C66SS1_INTROUTER0_outl_5_85_to_C66SS1_CORE0_c66_event_in_sync_15_95 = {
    .lbase  = 5,
    .len    = 81,
    .rid    = TISCI_DEV_C66SS1_CORE0,
    .rbase  = 15,
};
const struct Sciclient_rmIrqIf C66SS1_INTROUTER0_outl_86_86_to_C66SS1_CORE0_c66_event_in_sync_99_99 = {
    .lbase  = 86,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_CORE0,
    .rbase  = 99,
};
const struct Sciclient_rmIrqIf C66SS1_INTROUTER0_outl_87_94_to_C66SS1_CORE0_c66_event_in_sync_102_109 = {
    .lbase  = 87,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS1_CORE0,
    .rbase  = 102,
};
const struct Sciclient_rmIrqIf C66SS1_INTROUTER0_outl_95_96_to_C66SS1_CORE0_c66_event_in_sync_114_115 = {
    .lbase  = 95,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_CORE0,
    .rbase  = 114,
};
const struct Sciclient_rmIrqIf *const tisci_if_C66SS1_INTROUTER0[] = {
    &C66SS1_INTROUTER0_outl_0_4_to_C66SS1_CORE0_c66_event_in_sync_4_8,
    &C66SS1_INTROUTER0_outl_5_85_to_C66SS1_CORE0_c66_event_in_sync_15_95,
    &C66SS1_INTROUTER0_outl_86_86_to_C66SS1_CORE0_c66_event_in_sync_99_99,
    &C66SS1_INTROUTER0_outl_87_94_to_C66SS1_CORE0_c66_event_in_sync_102_109,
    &C66SS1_INTROUTER0_outl_95_96_to_C66SS1_CORE0_c66_event_in_sync_114_115,
};
static const struct Sciclient_rmIrqNode tisci_irq_C66SS1_INTROUTER0 = {
    .id = TISCI_DEV_C66SS1_INTROUTER0,
    .n_if   = 5,
    .p_if   = &tisci_if_C66SS1_INTROUTER0[0],
};

/* Start of CMPEVENT_INTRTR0 interface definition */
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_16_23_to_MAIN2MCU_PLS_INTRTR0_in_96_103 = {
    .lbase  = 16,
    .len    = 8,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 96,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_16_31_to_R5FSS0_INTROUTER0_in_100_115 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 100,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_0_15_to_R5FSS0_INTROUTER0_in_288_303 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 288,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_0_15_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_544_559 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
    .rbase  = 544,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_16_31_to_R5FSS1_INTROUTER0_in_100_115 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 100,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_0_15_to_R5FSS1_INTROUTER0_in_288_303 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 288,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_0_15_to_COMPUTE_CLUSTER0_GIC500SS_spi_544_559 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
    .rbase  = 544,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_24_31_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_60_67 = {
    .lbase  = 24,
    .len    = 8,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 60,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_12_15_to_C66SS0_INTROUTER0_in_344_347 = {
    .lbase  = 12,
    .len    = 4,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 344,
};
const struct Sciclient_rmIrqIf CMPEVENT_INTRTR0_outp_12_15_to_C66SS1_INTROUTER0_in_344_347 = {
    .lbase  = 12,
    .len    = 4,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 344,
};
const struct Sciclient_rmIrqIf *const tisci_if_CMPEVENT_INTRTR0[] = {
    &CMPEVENT_INTRTR0_outp_16_23_to_MAIN2MCU_PLS_INTRTR0_in_96_103,
    &CMPEVENT_INTRTR0_outp_16_31_to_R5FSS0_INTROUTER0_in_100_115,
    &CMPEVENT_INTRTR0_outp_0_15_to_R5FSS0_INTROUTER0_in_288_303,
    &CMPEVENT_INTRTR0_outp_0_15_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_544_559,
    &CMPEVENT_INTRTR0_outp_16_31_to_R5FSS1_INTROUTER0_in_100_115,
    &CMPEVENT_INTRTR0_outp_0_15_to_R5FSS1_INTROUTER0_in_288_303,
    &CMPEVENT_INTRTR0_outp_0_15_to_COMPUTE_CLUSTER0_GIC500SS_spi_544_559,
    &CMPEVENT_INTRTR0_outp_24_31_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_60_67,
    &CMPEVENT_INTRTR0_outp_12_15_to_C66SS0_INTROUTER0_in_344_347,
    &CMPEVENT_INTRTR0_outp_12_15_to_C66SS1_INTROUTER0_in_344_347,
};
static const struct Sciclient_rmIrqNode tisci_irq_CMPEVENT_INTRTR0 = {
    .id = TISCI_DEV_CMPEVENT_INTRTR0,
    .n_if   = 10,
    .p_if   = &tisci_if_CMPEVENT_INTRTR0[0],
};

/* Start of MAIN2MCU_LVL_INTRTR0 interface definition */
const struct Sciclient_rmIrqIf MAIN2MCU_LVL_INTRTR0_outl_0_63_to_MCU_R5FSS0_CORE0_intr_160_223 = {
    .lbase  = 0,
    .len    = 64,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE0,
    .rbase  = 160,
};
const struct Sciclient_rmIrqIf MAIN2MCU_LVL_INTRTR0_outl_0_63_to_MCU_R5FSS0_CORE1_intr_160_223 = {
    .lbase  = 0,
    .len    = 64,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE1,
    .rbase  = 160,
};
const struct Sciclient_rmIrqIf *const tisci_if_MAIN2MCU_LVL_INTRTR0[] = {
    &MAIN2MCU_LVL_INTRTR0_outl_0_63_to_MCU_R5FSS0_CORE0_intr_160_223,
    &MAIN2MCU_LVL_INTRTR0_outl_0_63_to_MCU_R5FSS0_CORE1_intr_160_223,
};
static const struct Sciclient_rmIrqNode tisci_irq_MAIN2MCU_LVL_INTRTR0 = {
    .id = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .n_if   = 2,
    .p_if   = &tisci_if_MAIN2MCU_LVL_INTRTR0[0],
};

/* Start of MAIN2MCU_PLS_INTRTR0 interface definition */
const struct Sciclient_rmIrqIf MAIN2MCU_PLS_INTRTR0_outp_0_47_to_MCU_R5FSS0_CORE0_intr_224_271 = {
    .lbase  = 0,
    .len    = 48,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf MAIN2MCU_PLS_INTRTR0_outp_0_47_to_MCU_R5FSS0_CORE1_intr_224_271 = {
    .lbase  = 0,
    .len    = 48,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE1,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf *const tisci_if_MAIN2MCU_PLS_INTRTR0[] = {
    &MAIN2MCU_PLS_INTRTR0_outp_0_47_to_MCU_R5FSS0_CORE0_intr_224_271,
    &MAIN2MCU_PLS_INTRTR0_outp_0_47_to_MCU_R5FSS0_CORE1_intr_224_271,
};
static const struct Sciclient_rmIrqNode tisci_irq_MAIN2MCU_PLS_INTRTR0 = {
    .id = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .n_if   = 2,
    .p_if   = &tisci_if_MAIN2MCU_PLS_INTRTR0[0],
};

/* Start of GPIOMUX_INTRTR0 interface definition */
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_0_7_to_ESM0_esm_pls_event0_632_639 = {
    .lbase  = 0,
    .len    = 8,
    .rid    = TISCI_DEV_ESM0,
    .rbase  = 632,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_0_7_to_ESM0_esm_pls_event1_640_647 = {
    .lbase  = 0,
    .len    = 8,
    .rid    = TISCI_DEV_ESM0,
    .rbase  = 640,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_0_7_to_ESM0_esm_pls_event2_648_655 = {
    .lbase  = 0,
    .len    = 8,
    .rid    = TISCI_DEV_ESM0,
    .rbase  = 648,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_0_31_to_MAIN2MCU_PLS_INTRTR0_in_64_95 = {
    .lbase  = 0,
    .len    = 32,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_0_15_to_R5FSS0_INTROUTER0_in_68_83 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 68,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_8_63_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_392_447 = {
    .lbase  = 8,
    .len    = 56,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
    .rbase  = 392,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_0_15_to_R5FSS1_INTROUTER0_in_68_83 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 68,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_52_57_to_PRU_ICSSG0_pr1_iep0_cap_intr_req_4_9 = {
    .lbase  = 52,
    .len    = 6,
    .rid    = TISCI_DEV_PRU_ICSSG0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_58_63_to_PRU_ICSSG0_pr1_iep1_cap_intr_req_10_15 = {
    .lbase  = 58,
    .len    = 6,
    .rid    = TISCI_DEV_PRU_ICSSG0,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_8_63_to_COMPUTE_CLUSTER0_GIC500SS_spi_392_447 = {
    .lbase  = 8,
    .len    = 56,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
    .rbase  = 392,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_16_31_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_68_83 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 68,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_16_31_to_R5FSS0_CORE0_intr_176_191 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS0_CORE0,
    .rbase  = 176,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_16_31_to_R5FSS0_CORE1_intr_176_191 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS0_CORE1,
    .rbase  = 176,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_16_31_to_R5FSS1_CORE0_intr_176_191 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS1_CORE0,
    .rbase  = 176,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_16_31_to_R5FSS1_CORE1_intr_176_191 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS1_CORE1,
    .rbase  = 176,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_40_63_to_C66SS0_INTROUTER0_in_128_151 = {
    .lbase  = 40,
    .len    = 24,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 128,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_32_39_to_C66SS0_INTROUTER0_in_391_398 = {
    .lbase  = 32,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 391,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_40_63_to_C66SS1_INTROUTER0_in_128_151 = {
    .lbase  = 40,
    .len    = 24,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 128,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_32_39_to_C66SS1_INTROUTER0_in_391_398 = {
    .lbase  = 32,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 391,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_52_57_to_PRU_ICSSG1_pr1_iep0_cap_intr_req_4_9 = {
    .lbase  = 52,
    .len    = 6,
    .rid    = TISCI_DEV_PRU_ICSSG1,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf GPIOMUX_INTRTR0_outp_58_63_to_PRU_ICSSG1_pr1_iep1_cap_intr_req_10_15 = {
    .lbase  = 58,
    .len    = 6,
    .rid    = TISCI_DEV_PRU_ICSSG1,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf *const tisci_if_GPIOMUX_INTRTR0[] = {
    &GPIOMUX_INTRTR0_outp_0_7_to_ESM0_esm_pls_event0_632_639,
    &GPIOMUX_INTRTR0_outp_0_7_to_ESM0_esm_pls_event1_640_647,
    &GPIOMUX_INTRTR0_outp_0_7_to_ESM0_esm_pls_event2_648_655,
    &GPIOMUX_INTRTR0_outp_0_31_to_MAIN2MCU_PLS_INTRTR0_in_64_95,
    &GPIOMUX_INTRTR0_outp_0_15_to_R5FSS0_INTROUTER0_in_68_83,
    &GPIOMUX_INTRTR0_outp_8_63_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_392_447,
    &GPIOMUX_INTRTR0_outp_0_15_to_R5FSS1_INTROUTER0_in_68_83,
    &GPIOMUX_INTRTR0_outp_52_57_to_PRU_ICSSG0_pr1_iep0_cap_intr_req_4_9,
    &GPIOMUX_INTRTR0_outp_58_63_to_PRU_ICSSG0_pr1_iep1_cap_intr_req_10_15,
    &GPIOMUX_INTRTR0_outp_8_63_to_COMPUTE_CLUSTER0_GIC500SS_spi_392_447,
    &GPIOMUX_INTRTR0_outp_16_31_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_68_83,
    &GPIOMUX_INTRTR0_outp_16_31_to_R5FSS0_CORE0_intr_176_191,
    &GPIOMUX_INTRTR0_outp_16_31_to_R5FSS0_CORE1_intr_176_191,
    &GPIOMUX_INTRTR0_outp_16_31_to_R5FSS1_CORE0_intr_176_191,
    &GPIOMUX_INTRTR0_outp_16_31_to_R5FSS1_CORE1_intr_176_191,
    &GPIOMUX_INTRTR0_outp_40_63_to_C66SS0_INTROUTER0_in_128_151,
    &GPIOMUX_INTRTR0_outp_32_39_to_C66SS0_INTROUTER0_in_391_398,
    &GPIOMUX_INTRTR0_outp_40_63_to_C66SS1_INTROUTER0_in_128_151,
    &GPIOMUX_INTRTR0_outp_32_39_to_C66SS1_INTROUTER0_in_391_398,
    &GPIOMUX_INTRTR0_outp_52_57_to_PRU_ICSSG1_pr1_iep0_cap_intr_req_4_9,
    &GPIOMUX_INTRTR0_outp_58_63_to_PRU_ICSSG1_pr1_iep1_cap_intr_req_10_15,
};
static const struct Sciclient_rmIrqNode tisci_irq_GPIOMUX_INTRTR0 = {
    .id = TISCI_DEV_GPIOMUX_INTRTR0,
    .n_if   = 21,
    .p_if   = &tisci_if_GPIOMUX_INTRTR0[0],
};

/* Start of R5FSS0_INTROUTER0 interface definition */
const struct Sciclient_rmIrqIf R5FSS0_INTROUTER0_outl_0_255_to_R5FSS0_CORE0_intr_256_511 = {
    .lbase  = 0,
    .len    = 256,
    .rid    = TISCI_DEV_R5FSS0_CORE0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf R5FSS0_INTROUTER0_outl_0_255_to_R5FSS0_CORE1_intr_256_511 = {
    .lbase  = 0,
    .len    = 256,
    .rid    = TISCI_DEV_R5FSS0_CORE1,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf *const tisci_if_R5FSS0_INTROUTER0[] = {
    &R5FSS0_INTROUTER0_outl_0_255_to_R5FSS0_CORE0_intr_256_511,
    &R5FSS0_INTROUTER0_outl_0_255_to_R5FSS0_CORE1_intr_256_511,
};
static const struct Sciclient_rmIrqNode tisci_irq_R5FSS0_INTROUTER0 = {
    .id = TISCI_DEV_R5FSS0_INTROUTER0,
    .n_if   = 2,
    .p_if   = &tisci_if_R5FSS0_INTROUTER0[0],
};

/* Start of R5FSS1_INTROUTER0 interface definition */
const struct Sciclient_rmIrqIf R5FSS1_INTROUTER0_outl_0_255_to_R5FSS1_CORE1_intr_256_511 = {
    .lbase  = 0,
    .len    = 256,
    .rid    = TISCI_DEV_R5FSS1_CORE1,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf R5FSS1_INTROUTER0_outl_0_255_to_R5FSS1_CORE0_intr_256_511 = {
    .lbase  = 0,
    .len    = 256,
    .rid    = TISCI_DEV_R5FSS1_CORE0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf *const tisci_if_R5FSS1_INTROUTER0[] = {
    &R5FSS1_INTROUTER0_outl_0_255_to_R5FSS1_CORE1_intr_256_511,
    &R5FSS1_INTROUTER0_outl_0_255_to_R5FSS1_CORE0_intr_256_511,
};
static const struct Sciclient_rmIrqNode tisci_irq_R5FSS1_INTROUTER0 = {
    .id = TISCI_DEV_R5FSS1_INTROUTER0,
    .n_if   = 2,
    .p_if   = &tisci_if_R5FSS1_INTROUTER0[0],
};

/* Start of TIMESYNC_INTRTR0 interface definition */
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_0_0_to_navss512l_main_0_cpts0_hw1_push_0_0 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_1_1_to_navss512l_main_0_cpts0_hw2_push_1_1 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_2_2_to_navss512l_main_0_cpts0_hw3_push_2_2 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_3_3_to_navss512l_main_0_cpts0_hw4_push_3_3 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_4_4_to_navss512l_main_0_cpts0_hw5_push_4_4 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_5_5_to_navss512l_main_0_cpts0_hw6_push_5_5 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_6_6_to_navss512l_main_0_cpts0_hw7_push_6_6 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 6,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_7_7_to_navss512l_main_0_cpts0_hw8_push_7_7 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_40_47_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_52_59 = {
    .lbase  = 40,
    .len    = 8,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 52,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_24_24_to_MCU_CPSW0_cpts_hw3_push_0_0 = {
    .lbase  = 24,
    .len    = 1,
    .rid    = TISCI_DEV_MCU_CPSW0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_25_25_to_MCU_CPSW0_cpts_hw4_push_1_1 = {
    .lbase  = 25,
    .len    = 1,
    .rid    = TISCI_DEV_MCU_CPSW0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_20_20_to_PCIE0_pcie_cpts_hw2_push_0_0 = {
    .lbase  = 20,
    .len    = 1,
    .rid    = TISCI_DEV_PCIE0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_21_21_to_PCIE1_pcie_cpts_hw2_push_0_0 = {
    .lbase  = 21,
    .len    = 1,
    .rid    = TISCI_DEV_PCIE1,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_22_22_to_PCIE2_pcie_cpts_hw2_push_0_0 = {
    .lbase  = 22,
    .len    = 1,
    .rid    = TISCI_DEV_PCIE2,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_23_23_to_PCIE3_pcie_cpts_hw2_push_0_0 = {
    .lbase  = 23,
    .len    = 1,
    .rid    = TISCI_DEV_PCIE3,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_26_26_to_CPSW0_cpts_hw1_push_0_0 = {
    .lbase  = 26,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_27_27_to_CPSW0_cpts_hw2_push_1_1 = {
    .lbase  = 27,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_28_28_to_CPSW0_cpts_hw3_push_2_2 = {
    .lbase  = 28,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_29_29_to_CPSW0_cpts_hw4_push_3_3 = {
    .lbase  = 29,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_30_30_to_CPSW0_cpts_hw5_push_4_4 = {
    .lbase  = 30,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_31_31_to_CPSW0_cpts_hw6_push_5_5 = {
    .lbase  = 31,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_32_32_to_CPSW0_cpts_hw7_push_6_6 = {
    .lbase  = 32,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 6,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_33_33_to_CPSW0_cpts_hw8_push_7_7 = {
    .lbase  = 33,
    .len    = 1,
    .rid    = TISCI_DEV_CPSW0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_8_8_to_PRU_ICSSG0_pr1_edc0_latch0_in_0_0 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_9_9_to_PRU_ICSSG0_pr1_edc0_latch1_in_1_1 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_10_10_to_PRU_ICSSG0_pr1_edc1_latch0_in_2_2 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_11_11_to_PRU_ICSSG0_pr1_edc1_latch1_in_3_3 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG0,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_12_12_to_PRU_ICSSG1_pr1_edc0_latch0_in_0_0 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG1,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_13_13_to_PRU_ICSSG1_pr1_edc0_latch1_in_1_1 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG1,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_14_14_to_PRU_ICSSG1_pr1_edc1_latch0_in_2_2 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG1,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf TIMESYNC_INTRTR0_outl_15_15_to_PRU_ICSSG1_pr1_edc1_latch1_in_3_3 = {
    .lbase  = 15,
    .len    = 1,
    .rid    = TISCI_DEV_PRU_ICSSG1,
    .rbase  = 3,
};
const struct Sciclient_rmIrqIf *const tisci_if_TIMESYNC_INTRTR0[] = {
    &TIMESYNC_INTRTR0_outl_0_0_to_navss512l_main_0_cpts0_hw1_push_0_0,
    &TIMESYNC_INTRTR0_outl_1_1_to_navss512l_main_0_cpts0_hw2_push_1_1,
    &TIMESYNC_INTRTR0_outl_2_2_to_navss512l_main_0_cpts0_hw3_push_2_2,
    &TIMESYNC_INTRTR0_outl_3_3_to_navss512l_main_0_cpts0_hw4_push_3_3,
    &TIMESYNC_INTRTR0_outl_4_4_to_navss512l_main_0_cpts0_hw5_push_4_4,
    &TIMESYNC_INTRTR0_outl_5_5_to_navss512l_main_0_cpts0_hw6_push_5_5,
    &TIMESYNC_INTRTR0_outl_6_6_to_navss512l_main_0_cpts0_hw7_push_6_6,
    &TIMESYNC_INTRTR0_outl_7_7_to_navss512l_main_0_cpts0_hw8_push_7_7,
    &TIMESYNC_INTRTR0_outl_40_47_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_52_59,
    &TIMESYNC_INTRTR0_outl_24_24_to_MCU_CPSW0_cpts_hw3_push_0_0,
    &TIMESYNC_INTRTR0_outl_25_25_to_MCU_CPSW0_cpts_hw4_push_1_1,
    &TIMESYNC_INTRTR0_outl_20_20_to_PCIE0_pcie_cpts_hw2_push_0_0,
    &TIMESYNC_INTRTR0_outl_21_21_to_PCIE1_pcie_cpts_hw2_push_0_0,
    &TIMESYNC_INTRTR0_outl_22_22_to_PCIE2_pcie_cpts_hw2_push_0_0,
    &TIMESYNC_INTRTR0_outl_23_23_to_PCIE3_pcie_cpts_hw2_push_0_0,
    &TIMESYNC_INTRTR0_outl_26_26_to_CPSW0_cpts_hw1_push_0_0,
    &TIMESYNC_INTRTR0_outl_27_27_to_CPSW0_cpts_hw2_push_1_1,
    &TIMESYNC_INTRTR0_outl_28_28_to_CPSW0_cpts_hw3_push_2_2,
    &TIMESYNC_INTRTR0_outl_29_29_to_CPSW0_cpts_hw4_push_3_3,
    &TIMESYNC_INTRTR0_outl_30_30_to_CPSW0_cpts_hw5_push_4_4,
    &TIMESYNC_INTRTR0_outl_31_31_to_CPSW0_cpts_hw6_push_5_5,
    &TIMESYNC_INTRTR0_outl_32_32_to_CPSW0_cpts_hw7_push_6_6,
    &TIMESYNC_INTRTR0_outl_33_33_to_CPSW0_cpts_hw8_push_7_7,
    &TIMESYNC_INTRTR0_outl_8_8_to_PRU_ICSSG0_pr1_edc0_latch0_in_0_0,
    &TIMESYNC_INTRTR0_outl_9_9_to_PRU_ICSSG0_pr1_edc0_latch1_in_1_1,
    &TIMESYNC_INTRTR0_outl_10_10_to_PRU_ICSSG0_pr1_edc1_latch0_in_2_2,
    &TIMESYNC_INTRTR0_outl_11_11_to_PRU_ICSSG0_pr1_edc1_latch1_in_3_3,
    &TIMESYNC_INTRTR0_outl_12_12_to_PRU_ICSSG1_pr1_edc0_latch0_in_0_0,
    &TIMESYNC_INTRTR0_outl_13_13_to_PRU_ICSSG1_pr1_edc0_latch1_in_1_1,
    &TIMESYNC_INTRTR0_outl_14_14_to_PRU_ICSSG1_pr1_edc1_latch0_in_2_2,
    &TIMESYNC_INTRTR0_outl_15_15_to_PRU_ICSSG1_pr1_edc1_latch1_in_3_3,
};
static const struct Sciclient_rmIrqNode tisci_irq_TIMESYNC_INTRTR0 = {
    .id = TISCI_DEV_TIMESYNC_INTRTR0,
    .n_if   = 31,
    .p_if   = &tisci_if_TIMESYNC_INTRTR0[0],
};

/* Start of WKUP_GPIOMUX_INTRTR0 interface definition */
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_8_15_to_WKUP_ESM0_esm_pls_event0_88_95 = {
    .lbase  = 8,
    .len    = 8,
    .rid    = TISCI_DEV_WKUP_ESM0,
    .rbase  = 88,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_8_15_to_WKUP_ESM0_esm_pls_event1_96_103 = {
    .lbase  = 8,
    .len    = 8,
    .rid    = TISCI_DEV_WKUP_ESM0,
    .rbase  = 96,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_8_15_to_WKUP_ESM0_esm_pls_event2_104_111 = {
    .lbase  = 8,
    .len    = 8,
    .rid    = TISCI_DEV_WKUP_ESM0,
    .rbase  = 104,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_16_31_to_R5FSS0_INTROUTER0_in_160_175 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 160,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_16_31_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_960_975 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
    .rbase  = 960,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_16_31_to_R5FSS1_INTROUTER0_in_160_175 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 160,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_12_19_to_MCU_NAVSS0_INTAGGR_0_intaggr_levi_pend_4_11 = {
    .lbase  = 12,
    .len    = 8,
    .rid    = TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_16_31_to_COMPUTE_CLUSTER0_GIC500SS_spi_960_975 = {
    .lbase  = 16,
    .len    = 16,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
    .rbase  = 960,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_0_15_to_MCU_R5FSS0_CORE0_intr_124_139 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE0,
    .rbase  = 124,
};
const struct Sciclient_rmIrqIf WKUP_GPIOMUX_INTRTR0_outp_0_15_to_MCU_R5FSS0_CORE1_intr_124_139 = {
    .lbase  = 0,
    .len    = 16,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE1,
    .rbase  = 124,
};
const struct Sciclient_rmIrqIf *const tisci_if_WKUP_GPIOMUX_INTRTR0[] = {
    &WKUP_GPIOMUX_INTRTR0_outp_8_15_to_WKUP_ESM0_esm_pls_event0_88_95,
    &WKUP_GPIOMUX_INTRTR0_outp_8_15_to_WKUP_ESM0_esm_pls_event1_96_103,
    &WKUP_GPIOMUX_INTRTR0_outp_8_15_to_WKUP_ESM0_esm_pls_event2_104_111,
    &WKUP_GPIOMUX_INTRTR0_outp_16_31_to_R5FSS0_INTROUTER0_in_160_175,
    &WKUP_GPIOMUX_INTRTR0_outp_16_31_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_960_975,
    &WKUP_GPIOMUX_INTRTR0_outp_16_31_to_R5FSS1_INTROUTER0_in_160_175,
    &WKUP_GPIOMUX_INTRTR0_outp_12_19_to_MCU_NAVSS0_INTAGGR_0_intaggr_levi_pend_4_11,
    &WKUP_GPIOMUX_INTRTR0_outp_16_31_to_COMPUTE_CLUSTER0_GIC500SS_spi_960_975,
    &WKUP_GPIOMUX_INTRTR0_outp_0_15_to_MCU_R5FSS0_CORE0_intr_124_139,
    &WKUP_GPIOMUX_INTRTR0_outp_0_15_to_MCU_R5FSS0_CORE1_intr_124_139,
};
static const struct Sciclient_rmIrqNode tisci_irq_WKUP_GPIOMUX_INTRTR0 = {
    .id = TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
    .n_if   = 10,
    .p_if   = &tisci_if_WKUP_GPIOMUX_INTRTR0[0],
};

/* Start of AASRC0 interface definition */
const struct Sciclient_rmIrqIf AASRC0_infifo_level_1_1_to_MAIN2MCU_LVL_INTRTR0_in_200_200 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 200,
};
const struct Sciclient_rmIrqIf AASRC0_ingroup_level_2_2_to_MAIN2MCU_LVL_INTRTR0_in_201_201 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 201,
};
const struct Sciclient_rmIrqIf AASRC0_outfifo_level_3_3_to_MAIN2MCU_LVL_INTRTR0_in_202_202 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 202,
};
const struct Sciclient_rmIrqIf AASRC0_outgroup_level_4_4_to_MAIN2MCU_LVL_INTRTR0_in_203_203 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 203,
};
const struct Sciclient_rmIrqIf AASRC0_err_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_204_204 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 204,
};
const struct Sciclient_rmIrqIf AASRC0_infifo_level_1_1_to_C66SS0_INTROUTER0_in_334_334 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 334,
};
const struct Sciclient_rmIrqIf AASRC0_ingroup_level_2_2_to_C66SS0_INTROUTER0_in_335_335 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 335,
};
const struct Sciclient_rmIrqIf AASRC0_outfifo_level_3_3_to_C66SS0_INTROUTER0_in_336_336 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 336,
};
const struct Sciclient_rmIrqIf AASRC0_outgroup_level_4_4_to_C66SS0_INTROUTER0_in_337_337 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 337,
};
const struct Sciclient_rmIrqIf AASRC0_err_level_0_0_to_C66SS0_INTROUTER0_in_338_338 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 338,
};
const struct Sciclient_rmIrqIf AASRC0_infifo_level_1_1_to_C66SS1_INTROUTER0_in_334_334 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 334,
};
const struct Sciclient_rmIrqIf AASRC0_ingroup_level_2_2_to_C66SS1_INTROUTER0_in_335_335 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 335,
};
const struct Sciclient_rmIrqIf AASRC0_outfifo_level_3_3_to_C66SS1_INTROUTER0_in_336_336 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 336,
};
const struct Sciclient_rmIrqIf AASRC0_outgroup_level_4_4_to_C66SS1_INTROUTER0_in_337_337 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 337,
};
const struct Sciclient_rmIrqIf AASRC0_err_level_0_0_to_C66SS1_INTROUTER0_in_338_338 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 338,
};
const struct Sciclient_rmIrqIf AASRC0_err_level_0_0_to_R5FSS0_INTROUTER0_in_203_203 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 203,
};
const struct Sciclient_rmIrqIf AASRC0_infifo_level_1_1_to_R5FSS0_INTROUTER0_in_204_204 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 204,
};
const struct Sciclient_rmIrqIf AASRC0_ingroup_level_2_2_to_R5FSS0_INTROUTER0_in_205_205 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 205,
};
const struct Sciclient_rmIrqIf AASRC0_outfifo_level_3_3_to_R5FSS0_INTROUTER0_in_206_206 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 206,
};
const struct Sciclient_rmIrqIf AASRC0_outgroup_level_4_4_to_R5FSS0_INTROUTER0_in_207_207 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 207,
};
const struct Sciclient_rmIrqIf AASRC0_err_level_0_0_to_R5FSS1_INTROUTER0_in_203_203 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 203,
};
const struct Sciclient_rmIrqIf AASRC0_infifo_level_1_1_to_R5FSS1_INTROUTER0_in_204_204 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 204,
};
const struct Sciclient_rmIrqIf AASRC0_ingroup_level_2_2_to_R5FSS1_INTROUTER0_in_205_205 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 205,
};
const struct Sciclient_rmIrqIf AASRC0_outfifo_level_3_3_to_R5FSS1_INTROUTER0_in_206_206 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 206,
};
const struct Sciclient_rmIrqIf AASRC0_outgroup_level_4_4_to_R5FSS1_INTROUTER0_in_207_207 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 207,
};
const struct Sciclient_rmIrqIf *const tisci_if_AASRC0[] = {
    &AASRC0_infifo_level_1_1_to_MAIN2MCU_LVL_INTRTR0_in_200_200,
    &AASRC0_ingroup_level_2_2_to_MAIN2MCU_LVL_INTRTR0_in_201_201,
    &AASRC0_outfifo_level_3_3_to_MAIN2MCU_LVL_INTRTR0_in_202_202,
    &AASRC0_outgroup_level_4_4_to_MAIN2MCU_LVL_INTRTR0_in_203_203,
    &AASRC0_err_level_0_0_to_MAIN2MCU_LVL_INTRTR0_in_204_204,
    &AASRC0_infifo_level_1_1_to_C66SS0_INTROUTER0_in_334_334,
    &AASRC0_ingroup_level_2_2_to_C66SS0_INTROUTER0_in_335_335,
    &AASRC0_outfifo_level_3_3_to_C66SS0_INTROUTER0_in_336_336,
    &AASRC0_outgroup_level_4_4_to_C66SS0_INTROUTER0_in_337_337,
    &AASRC0_err_level_0_0_to_C66SS0_INTROUTER0_in_338_338,
    &AASRC0_infifo_level_1_1_to_C66SS1_INTROUTER0_in_334_334,
    &AASRC0_ingroup_level_2_2_to_C66SS1_INTROUTER0_in_335_335,
    &AASRC0_outfifo_level_3_3_to_C66SS1_INTROUTER0_in_336_336,
    &AASRC0_outgroup_level_4_4_to_C66SS1_INTROUTER0_in_337_337,
    &AASRC0_err_level_0_0_to_C66SS1_INTROUTER0_in_338_338,
    &AASRC0_err_level_0_0_to_R5FSS0_INTROUTER0_in_203_203,
    &AASRC0_infifo_level_1_1_to_R5FSS0_INTROUTER0_in_204_204,
    &AASRC0_ingroup_level_2_2_to_R5FSS0_INTROUTER0_in_205_205,
    &AASRC0_outfifo_level_3_3_to_R5FSS0_INTROUTER0_in_206_206,
    &AASRC0_outgroup_level_4_4_to_R5FSS0_INTROUTER0_in_207_207,
    &AASRC0_err_level_0_0_to_R5FSS1_INTROUTER0_in_203_203,
    &AASRC0_infifo_level_1_1_to_R5FSS1_INTROUTER0_in_204_204,
    &AASRC0_ingroup_level_2_2_to_R5FSS1_INTROUTER0_in_205_205,
    &AASRC0_outfifo_level_3_3_to_R5FSS1_INTROUTER0_in_206_206,
    &AASRC0_outgroup_level_4_4_to_R5FSS1_INTROUTER0_in_207_207,
};
static const struct Sciclient_rmIrqNode tisci_irq_AASRC0 = {
    .id = TISCI_DEV_AASRC0,
    .n_if   = 25,
    .p_if   = &tisci_if_AASRC0[0],
};

/* Start of DECODER0 interface definition */
const struct Sciclient_rmIrqIf DECODER0_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_252_252 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 252,
};
const struct Sciclient_rmIrqIf *const tisci_if_DECODER0[] = {
    &DECODER0_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_252_252,
};
static const struct Sciclient_rmIrqNode tisci_irq_DECODER0 = {
    .id = TISCI_DEV_DECODER0,
    .n_if   = 1,
    .p_if   = &tisci_if_DECODER0[0],
};

/* Start of UART0 interface definition */
const struct Sciclient_rmIrqIf UART0_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_96_96 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 96,
};
const struct Sciclient_rmIrqIf UART0_usart_irq_0_0_to_C66SS0_INTROUTER0_in_351_351 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 351,
};
const struct Sciclient_rmIrqIf UART0_usart_irq_0_0_to_C66SS1_INTROUTER0_in_351_351 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 351,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART0[] = {
    &UART0_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_96_96,
    &UART0_usart_irq_0_0_to_C66SS0_INTROUTER0_in_351_351,
    &UART0_usart_irq_0_0_to_C66SS1_INTROUTER0_in_351_351,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART0 = {
    .id = TISCI_DEV_UART0,
    .n_if   = 3,
    .p_if   = &tisci_if_UART0[0],
};

/* Start of MCU_UART0 interface definition */
const struct Sciclient_rmIrqIf MCU_UART0_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_139_139 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 139,
};
const struct Sciclient_rmIrqIf MCU_UART0_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_139_139 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 139,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_UART0[] = {
    &MCU_UART0_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_139_139,
    &MCU_UART0_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_139_139,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_UART0 = {
    .id = TISCI_DEV_MCU_UART0,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_UART0[0],
};

/* Start of DSS_DSI0 interface definition */
const struct Sciclient_rmIrqIf DSS_DSI0_dsi_0_func_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_224_224 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf *const tisci_if_DSS_DSI0[] = {
    &DSS_DSI0_dsi_0_func_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_224_224,
};
static const struct Sciclient_rmIrqNode tisci_irq_DSS_DSI0 = {
    .id = TISCI_DEV_DSS_DSI0,
    .n_if   = 1,
    .p_if   = &tisci_if_DSS_DSI0[0],
};

/* Start of DSS_EDP0 interface definition */
const struct Sciclient_rmIrqIf DSS_EDP0_intr_0_3_to_MAIN2MCU_LVL_INTRTR0_in_238_241 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 238,
};
const struct Sciclient_rmIrqIf *const tisci_if_DSS_EDP0[] = {
    &DSS_EDP0_intr_0_3_to_MAIN2MCU_LVL_INTRTR0_in_238_241,
};
static const struct Sciclient_rmIrqNode tisci_irq_DSS_EDP0 = {
    .id = TISCI_DEV_DSS_EDP0,
    .n_if   = 1,
    .p_if   = &tisci_if_DSS_EDP0[0],
};

/* Start of DSS0 interface definition */
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_func_irq_proc0_0_0_to_MAIN2MCU_LVL_INTRTR0_in_226_226 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 226,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_func_irq_proc1_1_1_to_MAIN2MCU_LVL_INTRTR0_in_227_227 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 227,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_secure_irq_proc0_4_4_to_MAIN2MCU_LVL_INTRTR0_in_228_228 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 228,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_secure_irq_proc1_5_5_to_MAIN2MCU_LVL_INTRTR0_in_229_229 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 229,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_safety_error_irq_proc0_2_2_to_MAIN2MCU_LVL_INTRTR0_in_230_230 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 230,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_safety_error_irq_proc1_3_3_to_MAIN2MCU_LVL_INTRTR0_in_231_231 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 231,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_func_irq_proc0_0_0_to_C66SS0_INTROUTER0_in_105_105 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 105,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_func_irq_proc1_1_1_to_C66SS0_INTROUTER0_in_106_106 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 106,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_secure_irq_proc0_4_4_to_C66SS0_INTROUTER0_in_107_107 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 107,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_secure_irq_proc1_5_5_to_C66SS0_INTROUTER0_in_108_108 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 108,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_safety_error_irq_proc0_2_2_to_C66SS0_INTROUTER0_in_109_109 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 109,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_safety_error_irq_proc1_3_3_to_C66SS0_INTROUTER0_in_110_110 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 110,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_func_irq_proc0_0_0_to_C66SS1_INTROUTER0_in_105_105 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 105,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_func_irq_proc1_1_1_to_C66SS1_INTROUTER0_in_106_106 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 106,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_secure_irq_proc0_4_4_to_C66SS1_INTROUTER0_in_107_107 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 107,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_secure_irq_proc1_5_5_to_C66SS1_INTROUTER0_in_108_108 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 108,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_safety_error_irq_proc0_2_2_to_C66SS1_INTROUTER0_in_109_109 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 109,
};
const struct Sciclient_rmIrqIf DSS0_dss_inst0_dispc_safety_error_irq_proc1_3_3_to_C66SS1_INTROUTER0_in_110_110 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 110,
};
const struct Sciclient_rmIrqIf *const tisci_if_DSS0[] = {
    &DSS0_dss_inst0_dispc_func_irq_proc0_0_0_to_MAIN2MCU_LVL_INTRTR0_in_226_226,
    &DSS0_dss_inst0_dispc_func_irq_proc1_1_1_to_MAIN2MCU_LVL_INTRTR0_in_227_227,
    &DSS0_dss_inst0_dispc_secure_irq_proc0_4_4_to_MAIN2MCU_LVL_INTRTR0_in_228_228,
    &DSS0_dss_inst0_dispc_secure_irq_proc1_5_5_to_MAIN2MCU_LVL_INTRTR0_in_229_229,
    &DSS0_dss_inst0_dispc_safety_error_irq_proc0_2_2_to_MAIN2MCU_LVL_INTRTR0_in_230_230,
    &DSS0_dss_inst0_dispc_safety_error_irq_proc1_3_3_to_MAIN2MCU_LVL_INTRTR0_in_231_231,
    &DSS0_dss_inst0_dispc_func_irq_proc0_0_0_to_C66SS0_INTROUTER0_in_105_105,
    &DSS0_dss_inst0_dispc_func_irq_proc1_1_1_to_C66SS0_INTROUTER0_in_106_106,
    &DSS0_dss_inst0_dispc_secure_irq_proc0_4_4_to_C66SS0_INTROUTER0_in_107_107,
    &DSS0_dss_inst0_dispc_secure_irq_proc1_5_5_to_C66SS0_INTROUTER0_in_108_108,
    &DSS0_dss_inst0_dispc_safety_error_irq_proc0_2_2_to_C66SS0_INTROUTER0_in_109_109,
    &DSS0_dss_inst0_dispc_safety_error_irq_proc1_3_3_to_C66SS0_INTROUTER0_in_110_110,
    &DSS0_dss_inst0_dispc_func_irq_proc0_0_0_to_C66SS1_INTROUTER0_in_105_105,
    &DSS0_dss_inst0_dispc_func_irq_proc1_1_1_to_C66SS1_INTROUTER0_in_106_106,
    &DSS0_dss_inst0_dispc_secure_irq_proc0_4_4_to_C66SS1_INTROUTER0_in_107_107,
    &DSS0_dss_inst0_dispc_secure_irq_proc1_5_5_to_C66SS1_INTROUTER0_in_108_108,
    &DSS0_dss_inst0_dispc_safety_error_irq_proc0_2_2_to_C66SS1_INTROUTER0_in_109_109,
    &DSS0_dss_inst0_dispc_safety_error_irq_proc1_3_3_to_C66SS1_INTROUTER0_in_110_110,
};
static const struct Sciclient_rmIrqNode tisci_irq_DSS0 = {
    .id = TISCI_DEV_DSS0,
    .n_if   = 18,
    .p_if   = &tisci_if_DSS0[0],
};

/* Start of ENCODER0 interface definition */
const struct Sciclient_rmIrqIf ENCODER0_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_253_253 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 253,
};
const struct Sciclient_rmIrqIf *const tisci_if_ENCODER0[] = {
    &ENCODER0_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_253_253,
};
static const struct Sciclient_rmIrqNode tisci_irq_ENCODER0 = {
    .id = TISCI_DEV_ENCODER0,
    .n_if   = 1,
    .p_if   = &tisci_if_ENCODER0[0],
};

/* Start of WKUP_VTM0 interface definition */
const struct Sciclient_rmIrqIf WKUP_VTM0_therm_lvl_gt_th1_intr_0_0_to_R5FSS0_INTROUTER0_in_328_328 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 328,
};
const struct Sciclient_rmIrqIf WKUP_VTM0_therm_lvl_gt_th2_intr_1_1_to_R5FSS0_INTROUTER0_in_329_329 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 329,
};
const struct Sciclient_rmIrqIf WKUP_VTM0_therm_lvl_lt_th0_intr_2_2_to_R5FSS0_INTROUTER0_in_330_330 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 330,
};
const struct Sciclient_rmIrqIf WKUP_VTM0_therm_lvl_gt_th1_intr_0_0_to_R5FSS1_INTROUTER0_in_328_328 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 328,
};
const struct Sciclient_rmIrqIf WKUP_VTM0_therm_lvl_gt_th2_intr_1_1_to_R5FSS1_INTROUTER0_in_329_329 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 329,
};
const struct Sciclient_rmIrqIf WKUP_VTM0_therm_lvl_lt_th0_intr_2_2_to_R5FSS1_INTROUTER0_in_330_330 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 330,
};
const struct Sciclient_rmIrqIf *const tisci_if_WKUP_VTM0[] = {
    &WKUP_VTM0_therm_lvl_gt_th1_intr_0_0_to_R5FSS0_INTROUTER0_in_328_328,
    &WKUP_VTM0_therm_lvl_gt_th2_intr_1_1_to_R5FSS0_INTROUTER0_in_329_329,
    &WKUP_VTM0_therm_lvl_lt_th0_intr_2_2_to_R5FSS0_INTROUTER0_in_330_330,
    &WKUP_VTM0_therm_lvl_gt_th1_intr_0_0_to_R5FSS1_INTROUTER0_in_328_328,
    &WKUP_VTM0_therm_lvl_gt_th2_intr_1_1_to_R5FSS1_INTROUTER0_in_329_329,
    &WKUP_VTM0_therm_lvl_lt_th0_intr_2_2_to_R5FSS1_INTROUTER0_in_330_330,
};
static const struct Sciclient_rmIrqNode tisci_irq_WKUP_VTM0 = {
    .id = TISCI_DEV_WKUP_VTM0,
    .n_if   = 6,
    .p_if   = &tisci_if_WKUP_VTM0[0],
};

/* Start of MCAN0 interface definition */
const struct Sciclient_rmIrqIf MCAN0_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_16_17 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 16,
};
const struct Sciclient_rmIrqIf MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_18_18 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 18,
};
const struct Sciclient_rmIrqIf MCAN0_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_382_383 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 382,
};
const struct Sciclient_rmIrqIf MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_384_384 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 384,
};
const struct Sciclient_rmIrqIf MCAN0_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_382_383 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 382,
};
const struct Sciclient_rmIrqIf MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_384_384 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 384,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN0[] = {
    &MCAN0_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_16_17,
    &MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_18_18,
    &MCAN0_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_382_383,
    &MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_384_384,
    &MCAN0_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_382_383,
    &MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_384_384,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN0 = {
    .id = TISCI_DEV_MCAN0,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN0[0],
};

/* Start of MCAN1 interface definition */
const struct Sciclient_rmIrqIf MCAN1_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_19_20 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 19,
};
const struct Sciclient_rmIrqIf MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_21_21 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 21,
};
const struct Sciclient_rmIrqIf MCAN1_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_385_386 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 385,
};
const struct Sciclient_rmIrqIf MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_387_387 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 387,
};
const struct Sciclient_rmIrqIf MCAN1_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_385_386 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 385,
};
const struct Sciclient_rmIrqIf MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_387_387 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 387,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN1[] = {
    &MCAN1_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_19_20,
    &MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_21_21,
    &MCAN1_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_385_386,
    &MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_387_387,
    &MCAN1_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_385_386,
    &MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_387_387,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN1 = {
    .id = TISCI_DEV_MCAN1,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN1[0],
};

/* Start of MCAN2 interface definition */
const struct Sciclient_rmIrqIf MCAN2_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_22_23 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 22,
};
const struct Sciclient_rmIrqIf MCAN2_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_24_24 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf MCAN2_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_21_22 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 21,
};
const struct Sciclient_rmIrqIf MCAN2_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_23_23 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 23,
};
const struct Sciclient_rmIrqIf MCAN2_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_21_22 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 21,
};
const struct Sciclient_rmIrqIf MCAN2_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_23_23 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 23,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN2[] = {
    &MCAN2_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_22_23,
    &MCAN2_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_24_24,
    &MCAN2_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_21_22,
    &MCAN2_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_23_23,
    &MCAN2_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_21_22,
    &MCAN2_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_23_23,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN2 = {
    .id = TISCI_DEV_MCAN2,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN2[0],
};

/* Start of MCAN3 interface definition */
const struct Sciclient_rmIrqIf MCAN3_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_25_26 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 25,
};
const struct Sciclient_rmIrqIf MCAN3_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_27_27 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 27,
};
const struct Sciclient_rmIrqIf MCAN3_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_24_25 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf MCAN3_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_26_26 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 26,
};
const struct Sciclient_rmIrqIf MCAN3_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_24_25 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf MCAN3_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_26_26 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 26,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN3[] = {
    &MCAN3_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_25_26,
    &MCAN3_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_27_27,
    &MCAN3_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_24_25,
    &MCAN3_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_26_26,
    &MCAN3_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_24_25,
    &MCAN3_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_26_26,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN3 = {
    .id = TISCI_DEV_MCAN3,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN3[0],
};

/* Start of MCAN4 interface definition */
const struct Sciclient_rmIrqIf MCAN4_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_278_279 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 278,
};
const struct Sciclient_rmIrqIf MCAN4_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_280_280 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 280,
};
const struct Sciclient_rmIrqIf MCAN4_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_58_59 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 58,
};
const struct Sciclient_rmIrqIf MCAN4_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_60_60 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 60,
};
const struct Sciclient_rmIrqIf MCAN4_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_58_59 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 58,
};
const struct Sciclient_rmIrqIf MCAN4_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_60_60 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 60,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN4[] = {
    &MCAN4_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_278_279,
    &MCAN4_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_280_280,
    &MCAN4_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_58_59,
    &MCAN4_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_60_60,
    &MCAN4_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_58_59,
    &MCAN4_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_60_60,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN4 = {
    .id = TISCI_DEV_MCAN4,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN4[0],
};

/* Start of MCAN5 interface definition */
const struct Sciclient_rmIrqIf MCAN5_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_281_282 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 281,
};
const struct Sciclient_rmIrqIf MCAN5_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_283_283 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 283,
};
const struct Sciclient_rmIrqIf MCAN5_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_61_62 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 61,
};
const struct Sciclient_rmIrqIf MCAN5_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_63_63 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 63,
};
const struct Sciclient_rmIrqIf MCAN5_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_61_62 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 61,
};
const struct Sciclient_rmIrqIf MCAN5_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_63_63 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 63,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN5[] = {
    &MCAN5_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_281_282,
    &MCAN5_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_283_283,
    &MCAN5_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_61_62,
    &MCAN5_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_63_63,
    &MCAN5_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_61_62,
    &MCAN5_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_63_63,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN5 = {
    .id = TISCI_DEV_MCAN5,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN5[0],
};

/* Start of MCAN6 interface definition */
const struct Sciclient_rmIrqIf MCAN6_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_284_285 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 284,
};
const struct Sciclient_rmIrqIf MCAN6_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_286_286 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 286,
};
const struct Sciclient_rmIrqIf MCAN6_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_64_65 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf MCAN6_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_66_66 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 66,
};
const struct Sciclient_rmIrqIf MCAN6_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_64_65 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf MCAN6_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_66_66 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 66,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN6[] = {
    &MCAN6_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_284_285,
    &MCAN6_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_286_286,
    &MCAN6_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_64_65,
    &MCAN6_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_66_66,
    &MCAN6_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_64_65,
    &MCAN6_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_66_66,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN6 = {
    .id = TISCI_DEV_MCAN6,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN6[0],
};

/* Start of MCAN7 interface definition */
const struct Sciclient_rmIrqIf MCAN7_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_287_288 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 287,
};
const struct Sciclient_rmIrqIf MCAN7_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_289_289 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 289,
};
const struct Sciclient_rmIrqIf MCAN7_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_67_68 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 67,
};
const struct Sciclient_rmIrqIf MCAN7_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_69_69 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 69,
};
const struct Sciclient_rmIrqIf MCAN7_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_67_68 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 67,
};
const struct Sciclient_rmIrqIf MCAN7_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_69_69 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 69,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN7[] = {
    &MCAN7_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_287_288,
    &MCAN7_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_289_289,
    &MCAN7_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_67_68,
    &MCAN7_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_69_69,
    &MCAN7_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_67_68,
    &MCAN7_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_69_69,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN7 = {
    .id = TISCI_DEV_MCAN7,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN7[0],
};

/* Start of MCAN8 interface definition */
const struct Sciclient_rmIrqIf MCAN8_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_290_291 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 290,
};
const struct Sciclient_rmIrqIf MCAN8_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_292_292 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 292,
};
const struct Sciclient_rmIrqIf MCAN8_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_70_71 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 70,
};
const struct Sciclient_rmIrqIf MCAN8_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_72_72 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 72,
};
const struct Sciclient_rmIrqIf MCAN8_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_70_71 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 70,
};
const struct Sciclient_rmIrqIf MCAN8_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_72_72 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 72,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN8[] = {
    &MCAN8_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_290_291,
    &MCAN8_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_292_292,
    &MCAN8_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_70_71,
    &MCAN8_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_72_72,
    &MCAN8_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_70_71,
    &MCAN8_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_72_72,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN8 = {
    .id = TISCI_DEV_MCAN8,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN8[0],
};

/* Start of MCAN9 interface definition */
const struct Sciclient_rmIrqIf MCAN9_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_293_294 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 293,
};
const struct Sciclient_rmIrqIf MCAN9_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_295_295 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 295,
};
const struct Sciclient_rmIrqIf MCAN9_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_73_74 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 73,
};
const struct Sciclient_rmIrqIf MCAN9_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_75_75 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 75,
};
const struct Sciclient_rmIrqIf MCAN9_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_73_74 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 73,
};
const struct Sciclient_rmIrqIf MCAN9_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_75_75 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 75,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN9[] = {
    &MCAN9_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_293_294,
    &MCAN9_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_295_295,
    &MCAN9_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_73_74,
    &MCAN9_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_75_75,
    &MCAN9_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_73_74,
    &MCAN9_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_75_75,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN9 = {
    .id = TISCI_DEV_MCAN9,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN9[0],
};

/* Start of MCAN10 interface definition */
const struct Sciclient_rmIrqIf MCAN10_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_296_297 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 296,
};
const struct Sciclient_rmIrqIf MCAN10_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_298_298 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 298,
};
const struct Sciclient_rmIrqIf MCAN10_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_76_77 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 76,
};
const struct Sciclient_rmIrqIf MCAN10_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_78_78 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 78,
};
const struct Sciclient_rmIrqIf MCAN10_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_76_77 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 76,
};
const struct Sciclient_rmIrqIf MCAN10_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_78_78 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 78,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN10[] = {
    &MCAN10_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_296_297,
    &MCAN10_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_298_298,
    &MCAN10_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_76_77,
    &MCAN10_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_78_78,
    &MCAN10_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_76_77,
    &MCAN10_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_78_78,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN10 = {
    .id = TISCI_DEV_MCAN10,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN10[0],
};

/* Start of MCAN11 interface definition */
const struct Sciclient_rmIrqIf MCAN11_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_299_300 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 299,
};
const struct Sciclient_rmIrqIf MCAN11_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_301_301 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 301,
};
const struct Sciclient_rmIrqIf MCAN11_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_79_80 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 79,
};
const struct Sciclient_rmIrqIf MCAN11_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_81_81 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 81,
};
const struct Sciclient_rmIrqIf MCAN11_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_79_80 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 79,
};
const struct Sciclient_rmIrqIf MCAN11_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_81_81 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 81,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN11[] = {
    &MCAN11_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_299_300,
    &MCAN11_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_301_301,
    &MCAN11_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_79_80,
    &MCAN11_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_81_81,
    &MCAN11_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_79_80,
    &MCAN11_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_81_81,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN11 = {
    .id = TISCI_DEV_MCAN11,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN11[0],
};

/* Start of MCAN12 interface definition */
const struct Sciclient_rmIrqIf MCAN12_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_302_303 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 302,
};
const struct Sciclient_rmIrqIf MCAN12_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_304_304 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 304,
};
const struct Sciclient_rmIrqIf MCAN12_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_82_83 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 82,
};
const struct Sciclient_rmIrqIf MCAN12_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_84_84 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 84,
};
const struct Sciclient_rmIrqIf MCAN12_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_82_83 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 82,
};
const struct Sciclient_rmIrqIf MCAN12_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_84_84 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 84,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN12[] = {
    &MCAN12_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_302_303,
    &MCAN12_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_304_304,
    &MCAN12_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_82_83,
    &MCAN12_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_84_84,
    &MCAN12_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_82_83,
    &MCAN12_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_84_84,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN12 = {
    .id = TISCI_DEV_MCAN12,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN12[0],
};

/* Start of MCAN13 interface definition */
const struct Sciclient_rmIrqIf MCAN13_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_305_306 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 305,
};
const struct Sciclient_rmIrqIf MCAN13_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_307_307 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 307,
};
const struct Sciclient_rmIrqIf MCAN13_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_85_86 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 85,
};
const struct Sciclient_rmIrqIf MCAN13_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_87_87 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 87,
};
const struct Sciclient_rmIrqIf MCAN13_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_85_86 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 85,
};
const struct Sciclient_rmIrqIf MCAN13_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_87_87 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 87,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCAN13[] = {
    &MCAN13_mcanss_mcan_lvl_int_1_2_to_MAIN2MCU_LVL_INTRTR0_in_305_306,
    &MCAN13_mcanss_ext_ts_rollover_lvl_int_0_0_to_MAIN2MCU_LVL_INTRTR0_in_307_307,
    &MCAN13_mcanss_mcan_lvl_int_1_2_to_C66SS0_INTROUTER0_in_85_86,
    &MCAN13_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS0_INTROUTER0_in_87_87,
    &MCAN13_mcanss_mcan_lvl_int_1_2_to_C66SS1_INTROUTER0_in_85_86,
    &MCAN13_mcanss_ext_ts_rollover_lvl_int_0_0_to_C66SS1_INTROUTER0_in_87_87,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCAN13 = {
    .id = TISCI_DEV_MCAN13,
    .n_if   = 6,
    .p_if   = &tisci_if_MCAN13[0],
};

/* Start of MCU_MCAN0 interface definition */
const struct Sciclient_rmIrqIf MCU_MCAN0_mcanss_mcan_lvl_int_1_2_to_R5FSS0_INTROUTER0_in_336_337 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 336,
};
const struct Sciclient_rmIrqIf MCU_MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS0_INTROUTER0_in_338_338 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 338,
};
const struct Sciclient_rmIrqIf MCU_MCAN0_mcanss_mcan_lvl_int_1_2_to_R5FSS1_INTROUTER0_in_336_337 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 336,
};
const struct Sciclient_rmIrqIf MCU_MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS1_INTROUTER0_in_338_338 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 338,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_MCAN0[] = {
    &MCU_MCAN0_mcanss_mcan_lvl_int_1_2_to_R5FSS0_INTROUTER0_in_336_337,
    &MCU_MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS0_INTROUTER0_in_338_338,
    &MCU_MCAN0_mcanss_mcan_lvl_int_1_2_to_R5FSS1_INTROUTER0_in_336_337,
    &MCU_MCAN0_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS1_INTROUTER0_in_338_338,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_MCAN0 = {
    .id = TISCI_DEV_MCU_MCAN0,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_MCAN0[0],
};

/* Start of MCU_MCAN1 interface definition */
const struct Sciclient_rmIrqIf MCU_MCAN1_mcanss_mcan_lvl_int_1_2_to_R5FSS0_INTROUTER0_in_339_340 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 339,
};
const struct Sciclient_rmIrqIf MCU_MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS0_INTROUTER0_in_341_341 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 341,
};
const struct Sciclient_rmIrqIf MCU_MCAN1_mcanss_mcan_lvl_int_1_2_to_R5FSS1_INTROUTER0_in_339_340 = {
    .lbase  = 1,
    .len    = 2,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 339,
};
const struct Sciclient_rmIrqIf MCU_MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS1_INTROUTER0_in_341_341 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 341,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_MCAN1[] = {
    &MCU_MCAN1_mcanss_mcan_lvl_int_1_2_to_R5FSS0_INTROUTER0_in_339_340,
    &MCU_MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS0_INTROUTER0_in_341_341,
    &MCU_MCAN1_mcanss_mcan_lvl_int_1_2_to_R5FSS1_INTROUTER0_in_339_340,
    &MCU_MCAN1_mcanss_ext_ts_rollover_lvl_int_0_0_to_R5FSS1_INTROUTER0_in_341_341,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_MCAN1 = {
    .id = TISCI_DEV_MCU_MCAN1,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_MCAN1[0],
};

/* Start of MCASP0 interface definition */
const struct Sciclient_rmIrqIf MCASP0_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_176_176 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 176,
};
const struct Sciclient_rmIrqIf MCASP0_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_177_177 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 177,
};
const struct Sciclient_rmIrqIf MCASP0_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_310_310 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 310,
};
const struct Sciclient_rmIrqIf MCASP0_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_322_322 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 322,
};
const struct Sciclient_rmIrqIf MCASP0_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_310_310 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 310,
};
const struct Sciclient_rmIrqIf MCASP0_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_322_322 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 322,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP0[] = {
    &MCASP0_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_176_176,
    &MCASP0_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_177_177,
    &MCASP0_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_310_310,
    &MCASP0_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_322_322,
    &MCASP0_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_310_310,
    &MCASP0_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_322_322,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP0 = {
    .id = TISCI_DEV_MCASP0,
    .n_if   = 6,
    .p_if   = &tisci_if_MCASP0[0],
};

/* Start of MCASP1 interface definition */
const struct Sciclient_rmIrqIf MCASP1_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_178_178 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 178,
};
const struct Sciclient_rmIrqIf MCASP1_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_179_179 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 179,
};
const struct Sciclient_rmIrqIf MCASP1_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_311_311 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 311,
};
const struct Sciclient_rmIrqIf MCASP1_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_323_323 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 323,
};
const struct Sciclient_rmIrqIf MCASP1_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_311_311 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 311,
};
const struct Sciclient_rmIrqIf MCASP1_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_323_323 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 323,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP1[] = {
    &MCASP1_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_178_178,
    &MCASP1_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_179_179,
    &MCASP1_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_311_311,
    &MCASP1_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_323_323,
    &MCASP1_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_311_311,
    &MCASP1_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_323_323,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP1 = {
    .id = TISCI_DEV_MCASP1,
    .n_if   = 6,
    .p_if   = &tisci_if_MCASP1[0],
};

/* Start of MCASP2 interface definition */
const struct Sciclient_rmIrqIf MCASP2_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_180_180 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 180,
};
const struct Sciclient_rmIrqIf MCASP2_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_181_181 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 181,
};
const struct Sciclient_rmIrqIf MCASP2_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_312_312 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 312,
};
const struct Sciclient_rmIrqIf MCASP2_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_324_324 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 324,
};
const struct Sciclient_rmIrqIf MCASP2_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_312_312 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 312,
};
const struct Sciclient_rmIrqIf MCASP2_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_324_324 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 324,
};
const struct Sciclient_rmIrqIf MCASP2_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_208_208 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 208,
};
const struct Sciclient_rmIrqIf MCASP2_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_209_209 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 209,
};
const struct Sciclient_rmIrqIf MCASP2_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_208_208 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 208,
};
const struct Sciclient_rmIrqIf MCASP2_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_209_209 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 209,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP2[] = {
    &MCASP2_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_180_180,
    &MCASP2_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_181_181,
    &MCASP2_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_312_312,
    &MCASP2_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_324_324,
    &MCASP2_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_312_312,
    &MCASP2_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_324_324,
    &MCASP2_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_208_208,
    &MCASP2_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_209_209,
    &MCASP2_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_208_208,
    &MCASP2_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_209_209,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP2 = {
    .id = TISCI_DEV_MCASP2,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP2[0],
};

/* Start of MCASP3 interface definition */
const struct Sciclient_rmIrqIf MCASP3_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_182_182 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 182,
};
const struct Sciclient_rmIrqIf MCASP3_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_183_183 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 183,
};
const struct Sciclient_rmIrqIf MCASP3_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_313_313 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 313,
};
const struct Sciclient_rmIrqIf MCASP3_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_325_325 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 325,
};
const struct Sciclient_rmIrqIf MCASP3_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_313_313 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 313,
};
const struct Sciclient_rmIrqIf MCASP3_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_325_325 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 325,
};
const struct Sciclient_rmIrqIf MCASP3_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_210_210 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 210,
};
const struct Sciclient_rmIrqIf MCASP3_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_211_211 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 211,
};
const struct Sciclient_rmIrqIf MCASP3_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_210_210 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 210,
};
const struct Sciclient_rmIrqIf MCASP3_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_211_211 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 211,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP3[] = {
    &MCASP3_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_182_182,
    &MCASP3_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_183_183,
    &MCASP3_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_313_313,
    &MCASP3_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_325_325,
    &MCASP3_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_313_313,
    &MCASP3_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_325_325,
    &MCASP3_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_210_210,
    &MCASP3_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_211_211,
    &MCASP3_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_210_210,
    &MCASP3_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_211_211,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP3 = {
    .id = TISCI_DEV_MCASP3,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP3[0],
};

/* Start of MCASP4 interface definition */
const struct Sciclient_rmIrqIf MCASP4_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_184_184 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 184,
};
const struct Sciclient_rmIrqIf MCASP4_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_185_185 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 185,
};
const struct Sciclient_rmIrqIf MCASP4_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_314_314 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 314,
};
const struct Sciclient_rmIrqIf MCASP4_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_326_326 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 326,
};
const struct Sciclient_rmIrqIf MCASP4_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_314_314 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 314,
};
const struct Sciclient_rmIrqIf MCASP4_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_326_326 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 326,
};
const struct Sciclient_rmIrqIf MCASP4_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_212_212 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 212,
};
const struct Sciclient_rmIrqIf MCASP4_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_213_213 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 213,
};
const struct Sciclient_rmIrqIf MCASP4_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_212_212 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 212,
};
const struct Sciclient_rmIrqIf MCASP4_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_213_213 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 213,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP4[] = {
    &MCASP4_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_184_184,
    &MCASP4_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_185_185,
    &MCASP4_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_314_314,
    &MCASP4_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_326_326,
    &MCASP4_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_314_314,
    &MCASP4_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_326_326,
    &MCASP4_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_212_212,
    &MCASP4_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_213_213,
    &MCASP4_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_212_212,
    &MCASP4_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_213_213,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP4 = {
    .id = TISCI_DEV_MCASP4,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP4[0],
};

/* Start of MCASP5 interface definition */
const struct Sciclient_rmIrqIf MCASP5_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_186_186 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 186,
};
const struct Sciclient_rmIrqIf MCASP5_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_187_187 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 187,
};
const struct Sciclient_rmIrqIf MCASP5_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_315_315 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 315,
};
const struct Sciclient_rmIrqIf MCASP5_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_327_327 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 327,
};
const struct Sciclient_rmIrqIf MCASP5_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_315_315 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 315,
};
const struct Sciclient_rmIrqIf MCASP5_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_327_327 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 327,
};
const struct Sciclient_rmIrqIf MCASP5_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_214_214 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 214,
};
const struct Sciclient_rmIrqIf MCASP5_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_215_215 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 215,
};
const struct Sciclient_rmIrqIf MCASP5_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_214_214 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 214,
};
const struct Sciclient_rmIrqIf MCASP5_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_215_215 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 215,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP5[] = {
    &MCASP5_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_186_186,
    &MCASP5_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_187_187,
    &MCASP5_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_315_315,
    &MCASP5_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_327_327,
    &MCASP5_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_315_315,
    &MCASP5_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_327_327,
    &MCASP5_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_214_214,
    &MCASP5_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_215_215,
    &MCASP5_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_214_214,
    &MCASP5_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_215_215,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP5 = {
    .id = TISCI_DEV_MCASP5,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP5[0],
};

/* Start of MCASP6 interface definition */
const struct Sciclient_rmIrqIf MCASP6_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_188_188 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 188,
};
const struct Sciclient_rmIrqIf MCASP6_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_189_189 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 189,
};
const struct Sciclient_rmIrqIf MCASP6_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_316_316 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 316,
};
const struct Sciclient_rmIrqIf MCASP6_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_328_328 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 328,
};
const struct Sciclient_rmIrqIf MCASP6_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_316_316 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 316,
};
const struct Sciclient_rmIrqIf MCASP6_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_328_328 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 328,
};
const struct Sciclient_rmIrqIf MCASP6_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_216_216 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 216,
};
const struct Sciclient_rmIrqIf MCASP6_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_217_217 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 217,
};
const struct Sciclient_rmIrqIf MCASP6_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_216_216 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 216,
};
const struct Sciclient_rmIrqIf MCASP6_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_217_217 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 217,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP6[] = {
    &MCASP6_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_188_188,
    &MCASP6_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_189_189,
    &MCASP6_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_316_316,
    &MCASP6_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_328_328,
    &MCASP6_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_316_316,
    &MCASP6_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_328_328,
    &MCASP6_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_216_216,
    &MCASP6_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_217_217,
    &MCASP6_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_216_216,
    &MCASP6_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_217_217,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP6 = {
    .id = TISCI_DEV_MCASP6,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP6[0],
};

/* Start of MCASP7 interface definition */
const struct Sciclient_rmIrqIf MCASP7_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_190_190 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 190,
};
const struct Sciclient_rmIrqIf MCASP7_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_191_191 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 191,
};
const struct Sciclient_rmIrqIf MCASP7_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_317_317 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 317,
};
const struct Sciclient_rmIrqIf MCASP7_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_329_329 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 329,
};
const struct Sciclient_rmIrqIf MCASP7_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_317_317 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 317,
};
const struct Sciclient_rmIrqIf MCASP7_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_329_329 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 329,
};
const struct Sciclient_rmIrqIf MCASP7_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_218_218 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 218,
};
const struct Sciclient_rmIrqIf MCASP7_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_219_219 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 219,
};
const struct Sciclient_rmIrqIf MCASP7_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_218_218 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 218,
};
const struct Sciclient_rmIrqIf MCASP7_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_219_219 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 219,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP7[] = {
    &MCASP7_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_190_190,
    &MCASP7_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_191_191,
    &MCASP7_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_317_317,
    &MCASP7_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_329_329,
    &MCASP7_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_317_317,
    &MCASP7_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_329_329,
    &MCASP7_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_218_218,
    &MCASP7_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_219_219,
    &MCASP7_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_218_218,
    &MCASP7_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_219_219,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP7 = {
    .id = TISCI_DEV_MCASP7,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP7[0],
};

/* Start of MCASP8 interface definition */
const struct Sciclient_rmIrqIf MCASP8_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_192_192 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 192,
};
const struct Sciclient_rmIrqIf MCASP8_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_193_193 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 193,
};
const struct Sciclient_rmIrqIf MCASP8_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_318_318 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 318,
};
const struct Sciclient_rmIrqIf MCASP8_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_330_330 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 330,
};
const struct Sciclient_rmIrqIf MCASP8_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_318_318 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 318,
};
const struct Sciclient_rmIrqIf MCASP8_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_330_330 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 330,
};
const struct Sciclient_rmIrqIf MCASP8_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_220_220 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 220,
};
const struct Sciclient_rmIrqIf MCASP8_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_221_221 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 221,
};
const struct Sciclient_rmIrqIf MCASP8_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_220_220 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 220,
};
const struct Sciclient_rmIrqIf MCASP8_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_221_221 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 221,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP8[] = {
    &MCASP8_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_192_192,
    &MCASP8_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_193_193,
    &MCASP8_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_318_318,
    &MCASP8_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_330_330,
    &MCASP8_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_318_318,
    &MCASP8_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_330_330,
    &MCASP8_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_220_220,
    &MCASP8_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_221_221,
    &MCASP8_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_220_220,
    &MCASP8_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_221_221,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP8 = {
    .id = TISCI_DEV_MCASP8,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP8[0],
};

/* Start of MCASP9 interface definition */
const struct Sciclient_rmIrqIf MCASP9_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_194_194 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 194,
};
const struct Sciclient_rmIrqIf MCASP9_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_195_195 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 195,
};
const struct Sciclient_rmIrqIf MCASP9_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_319_319 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 319,
};
const struct Sciclient_rmIrqIf MCASP9_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_331_331 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 331,
};
const struct Sciclient_rmIrqIf MCASP9_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_319_319 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 319,
};
const struct Sciclient_rmIrqIf MCASP9_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_331_331 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 331,
};
const struct Sciclient_rmIrqIf MCASP9_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_222_222 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 222,
};
const struct Sciclient_rmIrqIf MCASP9_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_223_223 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 223,
};
const struct Sciclient_rmIrqIf MCASP9_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_222_222 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 222,
};
const struct Sciclient_rmIrqIf MCASP9_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_223_223 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 223,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP9[] = {
    &MCASP9_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_194_194,
    &MCASP9_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_195_195,
    &MCASP9_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_319_319,
    &MCASP9_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_331_331,
    &MCASP9_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_319_319,
    &MCASP9_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_331_331,
    &MCASP9_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_222_222,
    &MCASP9_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_223_223,
    &MCASP9_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_222_222,
    &MCASP9_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_223_223,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP9 = {
    .id = TISCI_DEV_MCASP9,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP9[0],
};

/* Start of MCASP10 interface definition */
const struct Sciclient_rmIrqIf MCASP10_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_196_196 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 196,
};
const struct Sciclient_rmIrqIf MCASP10_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_197_197 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 197,
};
const struct Sciclient_rmIrqIf MCASP10_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_320_320 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 320,
};
const struct Sciclient_rmIrqIf MCASP10_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_332_332 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 332,
};
const struct Sciclient_rmIrqIf MCASP10_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_320_320 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 320,
};
const struct Sciclient_rmIrqIf MCASP10_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_332_332 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 332,
};
const struct Sciclient_rmIrqIf MCASP10_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_224_224 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf MCASP10_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_225_225 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 225,
};
const struct Sciclient_rmIrqIf MCASP10_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_224_224 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf MCASP10_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_225_225 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 225,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP10[] = {
    &MCASP10_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_196_196,
    &MCASP10_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_197_197,
    &MCASP10_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_320_320,
    &MCASP10_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_332_332,
    &MCASP10_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_320_320,
    &MCASP10_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_332_332,
    &MCASP10_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_224_224,
    &MCASP10_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_225_225,
    &MCASP10_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_224_224,
    &MCASP10_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_225_225,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP10 = {
    .id = TISCI_DEV_MCASP10,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP10[0],
};

/* Start of MCASP11 interface definition */
const struct Sciclient_rmIrqIf MCASP11_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_198_198 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 198,
};
const struct Sciclient_rmIrqIf MCASP11_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_199_199 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 199,
};
const struct Sciclient_rmIrqIf MCASP11_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_321_321 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 321,
};
const struct Sciclient_rmIrqIf MCASP11_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_333_333 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 333,
};
const struct Sciclient_rmIrqIf MCASP11_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_321_321 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 321,
};
const struct Sciclient_rmIrqIf MCASP11_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_333_333 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 333,
};
const struct Sciclient_rmIrqIf MCASP11_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_226_226 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 226,
};
const struct Sciclient_rmIrqIf MCASP11_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_227_227 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 227,
};
const struct Sciclient_rmIrqIf MCASP11_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_226_226 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 226,
};
const struct Sciclient_rmIrqIf MCASP11_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_227_227 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 227,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCASP11[] = {
    &MCASP11_xmit_intr_pend_1_1_to_MAIN2MCU_LVL_INTRTR0_in_198_198,
    &MCASP11_rec_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_199_199,
    &MCASP11_rec_intr_pend_0_0_to_C66SS0_INTROUTER0_in_321_321,
    &MCASP11_xmit_intr_pend_1_1_to_C66SS0_INTROUTER0_in_333_333,
    &MCASP11_rec_intr_pend_0_0_to_C66SS1_INTROUTER0_in_321_321,
    &MCASP11_xmit_intr_pend_1_1_to_C66SS1_INTROUTER0_in_333_333,
    &MCASP11_xmit_intr_pend_1_1_to_R5FSS0_INTROUTER0_in_226_226,
    &MCASP11_rec_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_227_227,
    &MCASP11_xmit_intr_pend_1_1_to_R5FSS1_INTROUTER0_in_226_226,
    &MCASP11_rec_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_227_227,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCASP11 = {
    .id = TISCI_DEV_MCASP11,
    .n_if   = 10,
    .p_if   = &tisci_if_MCASP11[0],
};

/* Start of MLB0 interface definition */
const struct Sciclient_rmIrqIf MLB0_mlbss_mlb_int_2_2_to_MAIN2MCU_LVL_INTRTR0_in_205_205 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 205,
};
const struct Sciclient_rmIrqIf MLB0_mlbss_mlb_ahb_int_0_1_to_MAIN2MCU_LVL_INTRTR0_in_206_207 = {
    .lbase  = 0,
    .len    = 2,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 206,
};
const struct Sciclient_rmIrqIf MLB0_mlbss_mlb_ahb_int_0_1_to_C66SS0_INTROUTER0_in_388_389 = {
    .lbase  = 0,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 388,
};
const struct Sciclient_rmIrqIf MLB0_mlbss_mlb_int_2_2_to_C66SS0_INTROUTER0_in_390_390 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 390,
};
const struct Sciclient_rmIrqIf MLB0_mlbss_mlb_ahb_int_0_1_to_C66SS1_INTROUTER0_in_388_389 = {
    .lbase  = 0,
    .len    = 2,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 388,
};
const struct Sciclient_rmIrqIf MLB0_mlbss_mlb_int_2_2_to_C66SS1_INTROUTER0_in_390_390 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 390,
};
const struct Sciclient_rmIrqIf *const tisci_if_MLB0[] = {
    &MLB0_mlbss_mlb_int_2_2_to_MAIN2MCU_LVL_INTRTR0_in_205_205,
    &MLB0_mlbss_mlb_ahb_int_0_1_to_MAIN2MCU_LVL_INTRTR0_in_206_207,
    &MLB0_mlbss_mlb_ahb_int_0_1_to_C66SS0_INTROUTER0_in_388_389,
    &MLB0_mlbss_mlb_int_2_2_to_C66SS0_INTROUTER0_in_390_390,
    &MLB0_mlbss_mlb_ahb_int_0_1_to_C66SS1_INTROUTER0_in_388_389,
    &MLB0_mlbss_mlb_int_2_2_to_C66SS1_INTROUTER0_in_390_390,
};
static const struct Sciclient_rmIrqNode tisci_irq_MLB0 = {
    .id = TISCI_DEV_MLB0,
    .n_if   = 6,
    .p_if   = &tisci_if_MLB0[0],
};

/* Start of I2C0 interface definition */
const struct Sciclient_rmIrqIf I2C0_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_56_56 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 56,
};
const struct Sciclient_rmIrqIf I2C0_pointrpend_0_0_to_C66SS0_INTROUTER0_in_348_348 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 348,
};
const struct Sciclient_rmIrqIf I2C0_pointrpend_0_0_to_C66SS1_INTROUTER0_in_348_348 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 348,
};
const struct Sciclient_rmIrqIf *const tisci_if_I2C0[] = {
    &I2C0_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_56_56,
    &I2C0_pointrpend_0_0_to_C66SS0_INTROUTER0_in_348_348,
    &I2C0_pointrpend_0_0_to_C66SS1_INTROUTER0_in_348_348,
};
static const struct Sciclient_rmIrqNode tisci_irq_I2C0 = {
    .id = TISCI_DEV_I2C0,
    .n_if   = 3,
    .p_if   = &tisci_if_I2C0[0],
};

/* Start of I2C1 interface definition */
const struct Sciclient_rmIrqIf I2C1_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_57_57 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 57,
};
const struct Sciclient_rmIrqIf I2C1_pointrpend_0_0_to_C66SS0_INTROUTER0_in_349_349 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 349,
};
const struct Sciclient_rmIrqIf I2C1_pointrpend_0_0_to_C66SS1_INTROUTER0_in_349_349 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 349,
};
const struct Sciclient_rmIrqIf *const tisci_if_I2C1[] = {
    &I2C1_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_57_57,
    &I2C1_pointrpend_0_0_to_C66SS0_INTROUTER0_in_349_349,
    &I2C1_pointrpend_0_0_to_C66SS1_INTROUTER0_in_349_349,
};
static const struct Sciclient_rmIrqNode tisci_irq_I2C1 = {
    .id = TISCI_DEV_I2C1,
    .n_if   = 3,
    .p_if   = &tisci_if_I2C1[0],
};

/* Start of I2C2 interface definition */
const struct Sciclient_rmIrqIf I2C2_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_58_58 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 58,
};
const struct Sciclient_rmIrqIf I2C2_pointrpend_0_0_to_C66SS0_INTROUTER0_in_350_350 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 350,
};
const struct Sciclient_rmIrqIf I2C2_pointrpend_0_0_to_C66SS1_INTROUTER0_in_350_350 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 350,
};
const struct Sciclient_rmIrqIf I2C2_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_183_183 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 183,
};
const struct Sciclient_rmIrqIf I2C2_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_183_183 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 183,
};
const struct Sciclient_rmIrqIf *const tisci_if_I2C2[] = {
    &I2C2_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_58_58,
    &I2C2_pointrpend_0_0_to_C66SS0_INTROUTER0_in_350_350,
    &I2C2_pointrpend_0_0_to_C66SS1_INTROUTER0_in_350_350,
    &I2C2_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_183_183,
    &I2C2_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_183_183,
};
static const struct Sciclient_rmIrqNode tisci_irq_I2C2 = {
    .id = TISCI_DEV_I2C2,
    .n_if   = 5,
    .p_if   = &tisci_if_I2C2[0],
};

/* Start of I2C3 interface definition */
const struct Sciclient_rmIrqIf I2C3_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_59_59 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 59,
};
const struct Sciclient_rmIrqIf I2C3_pointrpend_0_0_to_C66SS0_INTROUTER0_in_27_27 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 27,
};
const struct Sciclient_rmIrqIf I2C3_pointrpend_0_0_to_C66SS1_INTROUTER0_in_27_27 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 27,
};
const struct Sciclient_rmIrqIf I2C3_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_184_184 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 184,
};
const struct Sciclient_rmIrqIf I2C3_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_184_184 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 184,
};
const struct Sciclient_rmIrqIf *const tisci_if_I2C3[] = {
    &I2C3_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_59_59,
    &I2C3_pointrpend_0_0_to_C66SS0_INTROUTER0_in_27_27,
    &I2C3_pointrpend_0_0_to_C66SS1_INTROUTER0_in_27_27,
    &I2C3_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_184_184,
    &I2C3_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_184_184,
};
static const struct Sciclient_rmIrqNode tisci_irq_I2C3 = {
    .id = TISCI_DEV_I2C3,
    .n_if   = 5,
    .p_if   = &tisci_if_I2C3[0],
};

/* Start of I2C4 interface definition */
const struct Sciclient_rmIrqIf I2C4_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_60_60 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 60,
};
const struct Sciclient_rmIrqIf I2C4_pointrpend_0_0_to_C66SS0_INTROUTER0_in_28_28 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 28,
};
const struct Sciclient_rmIrqIf I2C4_pointrpend_0_0_to_C66SS1_INTROUTER0_in_28_28 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 28,
};
const struct Sciclient_rmIrqIf I2C4_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_185_185 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 185,
};
const struct Sciclient_rmIrqIf I2C4_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_185_185 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 185,
};
const struct Sciclient_rmIrqIf *const tisci_if_I2C4[] = {
    &I2C4_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_60_60,
    &I2C4_pointrpend_0_0_to_C66SS0_INTROUTER0_in_28_28,
    &I2C4_pointrpend_0_0_to_C66SS1_INTROUTER0_in_28_28,
    &I2C4_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_185_185,
    &I2C4_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_185_185,
};
static const struct Sciclient_rmIrqNode tisci_irq_I2C4 = {
    .id = TISCI_DEV_I2C4,
    .n_if   = 5,
    .p_if   = &tisci_if_I2C4[0],
};

/* Start of I2C5 interface definition */
const struct Sciclient_rmIrqIf I2C5_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_61_61 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 61,
};
const struct Sciclient_rmIrqIf I2C5_pointrpend_0_0_to_C66SS0_INTROUTER0_in_29_29 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 29,
};
const struct Sciclient_rmIrqIf I2C5_pointrpend_0_0_to_C66SS1_INTROUTER0_in_29_29 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 29,
};
const struct Sciclient_rmIrqIf I2C5_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_186_186 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 186,
};
const struct Sciclient_rmIrqIf I2C5_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_186_186 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 186,
};
const struct Sciclient_rmIrqIf *const tisci_if_I2C5[] = {
    &I2C5_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_61_61,
    &I2C5_pointrpend_0_0_to_C66SS0_INTROUTER0_in_29_29,
    &I2C5_pointrpend_0_0_to_C66SS1_INTROUTER0_in_29_29,
    &I2C5_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_186_186,
    &I2C5_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_186_186,
};
static const struct Sciclient_rmIrqNode tisci_irq_I2C5 = {
    .id = TISCI_DEV_I2C5,
    .n_if   = 5,
    .p_if   = &tisci_if_I2C5[0],
};

/* Start of I2C6 interface definition */
const struct Sciclient_rmIrqIf I2C6_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_62_62 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 62,
};
const struct Sciclient_rmIrqIf I2C6_pointrpend_0_0_to_C66SS0_INTROUTER0_in_30_30 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 30,
};
const struct Sciclient_rmIrqIf I2C6_pointrpend_0_0_to_C66SS1_INTROUTER0_in_30_30 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 30,
};
const struct Sciclient_rmIrqIf I2C6_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_187_187 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 187,
};
const struct Sciclient_rmIrqIf I2C6_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_187_187 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 187,
};
const struct Sciclient_rmIrqIf *const tisci_if_I2C6[] = {
    &I2C6_pointrpend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_62_62,
    &I2C6_pointrpend_0_0_to_C66SS0_INTROUTER0_in_30_30,
    &I2C6_pointrpend_0_0_to_C66SS1_INTROUTER0_in_30_30,
    &I2C6_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_187_187,
    &I2C6_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_187_187,
};
static const struct Sciclient_rmIrqNode tisci_irq_I2C6 = {
    .id = TISCI_DEV_I2C6,
    .n_if   = 5,
    .p_if   = &tisci_if_I2C6[0],
};

/* Start of MCU_I2C0 interface definition */
const struct Sciclient_rmIrqIf MCU_I2C0_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_134_134 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 134,
};
const struct Sciclient_rmIrqIf MCU_I2C0_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_134_134 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 134,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_I2C0[] = {
    &MCU_I2C0_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_134_134,
    &MCU_I2C0_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_134_134,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_I2C0 = {
    .id = TISCI_DEV_MCU_I2C0,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_I2C0[0],
};

/* Start of MCU_I2C1 interface definition */
const struct Sciclient_rmIrqIf MCU_I2C1_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_135_135 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 135,
};
const struct Sciclient_rmIrqIf MCU_I2C1_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_135_135 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 135,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_I2C1[] = {
    &MCU_I2C1_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_135_135,
    &MCU_I2C1_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_135_135,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_I2C1 = {
    .id = TISCI_DEV_MCU_I2C1,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_I2C1[0],
};

/* Start of WKUP_I2C0 interface definition */
const struct Sciclient_rmIrqIf WKUP_I2C0_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_158_158 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 158,
};
const struct Sciclient_rmIrqIf WKUP_I2C0_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_158_158 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 158,
};
const struct Sciclient_rmIrqIf *const tisci_if_WKUP_I2C0[] = {
    &WKUP_I2C0_pointrpend_0_0_to_R5FSS0_INTROUTER0_in_158_158,
    &WKUP_I2C0_pointrpend_0_0_to_R5FSS1_INTROUTER0_in_158_158,
};
static const struct Sciclient_rmIrqNode tisci_irq_WKUP_I2C0 = {
    .id = TISCI_DEV_WKUP_I2C0,
    .n_if   = 2,
    .p_if   = &tisci_if_WKUP_I2C0[0],
};

/* Start of navss512l_main_0 interface definition */
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_genf0_1_1_to_TIMESYNC_INTRTR0_in_4_4 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_genf1_2_2_to_TIMESYNC_INTRTR0_in_5_5 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_genf2_3_3_to_TIMESYNC_INTRTR0_in_6_6 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 6,
};
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_genf3_4_4_to_TIMESYNC_INTRTR0_in_7_7 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_genf4_5_5_to_TIMESYNC_INTRTR0_in_8_8 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 8,
};
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_genf5_6_6_to_TIMESYNC_INTRTR0_in_9_9 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 9,
};
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_sync_7_7_to_TIMESYNC_INTRTR0_in_36_36 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 36,
};
const struct Sciclient_rmIrqIf navss512l_main_0_cpts0_comp_0_0_to_CMPEVENT_INTRTR0_in_8_8 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 8,
};
const struct Sciclient_rmIrqIf *const tisci_if_navss512l_main_0[] = {
    &navss512l_main_0_cpts0_genf0_1_1_to_TIMESYNC_INTRTR0_in_4_4,
    &navss512l_main_0_cpts0_genf1_2_2_to_TIMESYNC_INTRTR0_in_5_5,
    &navss512l_main_0_cpts0_genf2_3_3_to_TIMESYNC_INTRTR0_in_6_6,
    &navss512l_main_0_cpts0_genf3_4_4_to_TIMESYNC_INTRTR0_in_7_7,
    &navss512l_main_0_cpts0_genf4_5_5_to_TIMESYNC_INTRTR0_in_8_8,
    &navss512l_main_0_cpts0_genf5_6_6_to_TIMESYNC_INTRTR0_in_9_9,
    &navss512l_main_0_cpts0_sync_7_7_to_TIMESYNC_INTRTR0_in_36_36,
    &navss512l_main_0_cpts0_comp_0_0_to_CMPEVENT_INTRTR0_in_8_8,
};
static const struct Sciclient_rmIrqNode tisci_irq_navss512l_main_0 = {
    .id = TISCI_DEV_NAVSS0,
    .n_if   = 8,
    .p_if   = &tisci_if_navss512l_main_0[0],
};

/* Start of NAVSS0_CPTS_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_CPTS_0_event_pend_intr_0_0_to_NAVSS0_INTR_ROUTER_0_in_intr_391_391 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 391,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_CPTS_0[] = {
    &NAVSS0_CPTS_0_event_pend_intr_0_0_to_NAVSS0_INTR_ROUTER_0_in_intr_391_391,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_CPTS_0 = {
    .id = TISCI_DEV_NAVSS0_CPTS_0,
    .n_if   = 1,
    .p_if   = &tisci_if_NAVSS0_CPTS_0[0],
};

/* Start of NAVSS0_MODSS_INTAGGR_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MODSS_INTAGGR_0_intaggr_vintr_pend_0_63_to_NAVSS0_INTR_ROUTER_0_in_intr_320_383 = {
    .lbase  = 0,
    .len    = 64,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 320,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MODSS_INTAGGR_0[] = {
    &NAVSS0_MODSS_INTAGGR_0_intaggr_vintr_pend_0_63_to_NAVSS0_INTR_ROUTER_0_in_intr_320_383,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MODSS_INTAGGR_0 = {
    .id = TISCI_DEV_NAVSS0_MODSS_INTAGGR_0,
    .n_if   = 1,
    .p_if   = &tisci_if_NAVSS0_MODSS_INTAGGR_0[0],
};

/* Start of NAVSS0_MODSS_INTAGGR_1 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MODSS_INTAGGR_1_intaggr_vintr_pend_0_63_to_NAVSS0_INTR_ROUTER_0_in_intr_256_319 = {
    .lbase  = 0,
    .len    = 64,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MODSS_INTAGGR_1[] = {
    &NAVSS0_MODSS_INTAGGR_1_intaggr_vintr_pend_0_63_to_NAVSS0_INTR_ROUTER_0_in_intr_256_319,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MODSS_INTAGGR_1 = {
    .id = TISCI_DEV_NAVSS0_MODSS_INTAGGR_1,
    .n_if   = 1,
    .p_if   = &tisci_if_NAVSS0_MODSS_INTAGGR_1[0],
};

/* Start of NAVSS0_UDMASS_INTAGGR_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_UDMASS_INTAGGR_0_intaggr_vintr_pend_0_255_to_NAVSS0_INTR_ROUTER_0_in_intr_0_255 = {
    .lbase  = 0,
    .len    = 256,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_UDMASS_INTAGGR_0[] = {
    &NAVSS0_UDMASS_INTAGGR_0_intaggr_vintr_pend_0_255_to_NAVSS0_INTR_ROUTER_0_in_intr_0_255,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_UDMASS_INTAGGR_0 = {
    .id = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .n_if   = 1,
    .p_if   = &tisci_if_NAVSS0_UDMASS_INTAGGR_0[0],
};

/* Start of NAVSS0_INTR_ROUTER_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_0_63_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_64_127 = {
    .lbase  = 0,
    .len    = 64,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_64_127_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_448_511 = {
    .lbase  = 64,
    .len    = 64,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
    .rbase  = 448,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_128_191_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_672_735 = {
    .lbase  = 128,
    .len    = 64,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
    .rbase  = 672,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_384_391_to_PRU_ICSSG0_pr1_slv_intr_46_53 = {
    .lbase  = 384,
    .len    = 8,
    .rid    = TISCI_DEV_PRU_ICSSG0,
    .rbase  = 46,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_0_63_to_COMPUTE_CLUSTER0_GIC500SS_spi_64_127 = {
    .lbase  = 0,
    .len    = 64,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_64_127_to_COMPUTE_CLUSTER0_GIC500SS_spi_448_511 = {
    .lbase  = 64,
    .len    = 64,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
    .rbase  = 448,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_128_191_to_COMPUTE_CLUSTER0_GIC500SS_spi_672_735 = {
    .lbase  = 128,
    .len    = 64,
    .rid    = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
    .rbase  = 672,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_392_399_to_PRU_ICSSG1_pr1_slv_intr_46_53 = {
    .lbase  = 392,
    .len    = 8,
    .rid    = TISCI_DEV_PRU_ICSSG1,
    .rbase  = 46,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_192_223_to_R5FSS0_CORE0_intr_224_255 = {
    .lbase  = 192,
    .len    = 32,
    .rid    = TISCI_DEV_R5FSS0_CORE0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_224_255_to_R5FSS0_CORE1_intr_224_255 = {
    .lbase  = 224,
    .len    = 32,
    .rid    = TISCI_DEV_R5FSS0_CORE1,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_256_287_to_R5FSS1_CORE0_intr_224_255 = {
    .lbase  = 256,
    .len    = 32,
    .rid    = TISCI_DEV_R5FSS1_CORE0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_288_319_to_R5FSS1_CORE1_intr_224_255 = {
    .lbase  = 288,
    .len    = 32,
    .rid    = TISCI_DEV_R5FSS1_CORE1,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_344_351_to_C66SS0_INTROUTER0_in_97_104 = {
    .lbase  = 344,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 97,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_320_343_to_C66SS0_INTROUTER0_in_358_381 = {
    .lbase  = 320,
    .len    = 24,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 358,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_376_383_to_C66SS1_INTROUTER0_in_97_104 = {
    .lbase  = 376,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 97,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_352_375_to_C66SS1_INTROUTER0_in_358_381 = {
    .lbase  = 352,
    .len    = 24,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 358,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_400_407_to_MCU_R5FSS0_CORE1_intr_376_383 = {
    .lbase  = 400,
    .len    = 8,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE1,
    .rbase  = 376,
};
const struct Sciclient_rmIrqIf NAVSS0_INTR_ROUTER_0_outl_intr_400_407_to_MCU_R5FSS0_CORE0_intr_376_383 = {
    .lbase  = 400,
    .len    = 8,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE0,
    .rbase  = 376,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_INTR_ROUTER_0[] = {
    &NAVSS0_INTR_ROUTER_0_outl_intr_0_63_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_64_127,
    &NAVSS0_INTR_ROUTER_0_outl_intr_64_127_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_448_511,
    &NAVSS0_INTR_ROUTER_0_outl_intr_128_191_to_COMPUTE_CLUSTER0_CLEC_soc_events_in_672_735,
    &NAVSS0_INTR_ROUTER_0_outl_intr_384_391_to_PRU_ICSSG0_pr1_slv_intr_46_53,
    &NAVSS0_INTR_ROUTER_0_outl_intr_0_63_to_COMPUTE_CLUSTER0_GIC500SS_spi_64_127,
    &NAVSS0_INTR_ROUTER_0_outl_intr_64_127_to_COMPUTE_CLUSTER0_GIC500SS_spi_448_511,
    &NAVSS0_INTR_ROUTER_0_outl_intr_128_191_to_COMPUTE_CLUSTER0_GIC500SS_spi_672_735,
    &NAVSS0_INTR_ROUTER_0_outl_intr_392_399_to_PRU_ICSSG1_pr1_slv_intr_46_53,
    &NAVSS0_INTR_ROUTER_0_outl_intr_192_223_to_R5FSS0_CORE0_intr_224_255,
    &NAVSS0_INTR_ROUTER_0_outl_intr_224_255_to_R5FSS0_CORE1_intr_224_255,
    &NAVSS0_INTR_ROUTER_0_outl_intr_256_287_to_R5FSS1_CORE0_intr_224_255,
    &NAVSS0_INTR_ROUTER_0_outl_intr_288_319_to_R5FSS1_CORE1_intr_224_255,
    &NAVSS0_INTR_ROUTER_0_outl_intr_344_351_to_C66SS0_INTROUTER0_in_97_104,
    &NAVSS0_INTR_ROUTER_0_outl_intr_320_343_to_C66SS0_INTROUTER0_in_358_381,
    &NAVSS0_INTR_ROUTER_0_outl_intr_376_383_to_C66SS1_INTROUTER0_in_97_104,
    &NAVSS0_INTR_ROUTER_0_outl_intr_352_375_to_C66SS1_INTROUTER0_in_358_381,
    &NAVSS0_INTR_ROUTER_0_outl_intr_400_407_to_MCU_R5FSS0_CORE1_intr_376_383,
    &NAVSS0_INTR_ROUTER_0_outl_intr_400_407_to_MCU_R5FSS0_CORE0_intr_376_383,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_INTR_ROUTER_0 = {
    .id = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .n_if   = 18,
    .p_if   = &tisci_if_NAVSS0_INTR_ROUTER_0[0],
};

/* Start of NAVSS0_MAILBOX_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_0_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_4_7 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_0_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_436_439 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 436,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_0[] = {
    &NAVSS0_MAILBOX_0_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_4_7,
    &NAVSS0_MAILBOX_0_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_436_439,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_0 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_0,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_0[0],
};

/* Start of NAVSS0_MAILBOX_1 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_1_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_8_11 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 8,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_1_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_432_435 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 432,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_1[] = {
    &NAVSS0_MAILBOX_1_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_8_11,
    &NAVSS0_MAILBOX_1_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_432_435,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_1 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_1,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_1[0],
};

/* Start of NAVSS0_MAILBOX_2 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_2_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_12_15 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 12,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_2_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_428_431 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 428,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_2[] = {
    &NAVSS0_MAILBOX_2_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_12_15,
    &NAVSS0_MAILBOX_2_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_428_431,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_2 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_2,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_2[0],
};

/* Start of NAVSS0_MAILBOX_3 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_3_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_16_19 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 16,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_3_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_424_427 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 424,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_3[] = {
    &NAVSS0_MAILBOX_3_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_16_19,
    &NAVSS0_MAILBOX_3_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_424_427,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_3 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_3,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_3[0],
};

/* Start of NAVSS0_MAILBOX_4 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_4_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_20_23 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 20,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_4_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_420_423 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 420,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_4[] = {
    &NAVSS0_MAILBOX_4_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_20_23,
    &NAVSS0_MAILBOX_4_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_420_423,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_4 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_4,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_4[0],
};

/* Start of NAVSS0_MAILBOX_5 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_5_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_24_27 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_5_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_416_419 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 416,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_5[] = {
    &NAVSS0_MAILBOX_5_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_24_27,
    &NAVSS0_MAILBOX_5_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_416_419,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_5 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_5,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_5[0],
};

/* Start of NAVSS0_MAILBOX_6 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_6_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_28_31 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 28,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_6_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_412_415 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 412,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_6[] = {
    &NAVSS0_MAILBOX_6_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_28_31,
    &NAVSS0_MAILBOX_6_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_412_415,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_6 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_6,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_6[0],
};

/* Start of NAVSS0_MAILBOX_7 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_7_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_32_35 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 32,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_7_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_408_411 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 408,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_7[] = {
    &NAVSS0_MAILBOX_7_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_32_35,
    &NAVSS0_MAILBOX_7_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_408_411,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_7 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_7,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_7[0],
};

/* Start of NAVSS0_MAILBOX_8 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_8_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_36_39 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 36,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_8_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_404_407 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 404,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_8[] = {
    &NAVSS0_MAILBOX_8_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_36_39,
    &NAVSS0_MAILBOX_8_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_404_407,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_8 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_8,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_8[0],
};

/* Start of NAVSS0_MAILBOX_9 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_9_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_40_43 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 40,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_9_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_400_403 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 400,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_9[] = {
    &NAVSS0_MAILBOX_9_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_40_43,
    &NAVSS0_MAILBOX_9_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_400_403,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_9 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_9,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_9[0],
};

/* Start of NAVSS0_MAILBOX_10 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_10_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_44_47 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 44,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_10_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_396_399 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 396,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_10[] = {
    &NAVSS0_MAILBOX_10_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_44_47,
    &NAVSS0_MAILBOX_10_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_396_399,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_10 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_10,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_10[0],
};

/* Start of NAVSS0_MAILBOX_11 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_11_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_48_51 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf NAVSS0_MAILBOX_11_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_392_395 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 392,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MAILBOX_11[] = {
    &NAVSS0_MAILBOX_11_pend_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_48_51,
    &NAVSS0_MAILBOX_11_pend_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_392_395,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MAILBOX_11 = {
    .id = TISCI_DEV_NAVSS0_MAILBOX_11,
    .n_if   = 2,
    .p_if   = &tisci_if_NAVSS0_MAILBOX_11[0],
};

/* Start of NAVSS0_MCRC_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_MCRC_0_dma_event_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_0_3 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_UDMASS_INTAGGR_0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf NAVSS0_MCRC_0_dma_event_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_384_387 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 384,
};
const struct Sciclient_rmIrqIf NAVSS0_MCRC_0_intaggr_vintr_pend_4_4_to_NAVSS0_INTR_ROUTER_0_in_intr_388_388 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 388,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_MCRC_0[] = {
    &NAVSS0_MCRC_0_dma_event_intr_0_3_to_NAVSS0_UDMASS_INTAGGR_0_intaggr_levi_pend_0_3,
    &NAVSS0_MCRC_0_dma_event_intr_0_3_to_NAVSS0_INTR_ROUTER_0_in_intr_384_387,
    &NAVSS0_MCRC_0_intaggr_vintr_pend_4_4_to_NAVSS0_INTR_ROUTER_0_in_intr_388_388,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_MCRC_0 = {
    .id = TISCI_DEV_NAVSS0_MCRC_0,
    .n_if   = 3,
    .p_if   = &tisci_if_NAVSS0_MCRC_0[0],
};

/* Start of NAVSS0_TBU_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_TBU_0_io_tbu0_ras_intr_0_0_to_NAVSS0_INTR_ROUTER_0_in_intr_447_447 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 447,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_TBU_0[] = {
    &NAVSS0_TBU_0_io_tbu0_ras_intr_0_0_to_NAVSS0_INTR_ROUTER_0_in_intr_447_447,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_TBU_0 = {
    .id = TISCI_DEV_NAVSS0_TBU_0,
    .n_if   = 1,
    .p_if   = &tisci_if_NAVSS0_TBU_0[0],
};

/* Start of NAVSS0_TCU_0 interface definition */
const struct Sciclient_rmIrqIf NAVSS0_TCU_0_tcu_ras_intr_0_0_to_NAVSS0_INTR_ROUTER_0_in_intr_440_440 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 440,
};
const struct Sciclient_rmIrqIf NAVSS0_TCU_0_tcu_global_ns_intr_1_1_to_NAVSS0_INTR_ROUTER_0_in_intr_441_441 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 441,
};
const struct Sciclient_rmIrqIf NAVSS0_TCU_0_tcu_global_s_intr_2_2_to_NAVSS0_INTR_ROUTER_0_in_intr_442_442 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 442,
};
const struct Sciclient_rmIrqIf NAVSS0_TCU_0_tcu_cmd_sync_ns_intr_3_3_to_NAVSS0_INTR_ROUTER_0_in_intr_443_443 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 443,
};
const struct Sciclient_rmIrqIf NAVSS0_TCU_0_tcu_cmd_sync_s_intr_4_4_to_NAVSS0_INTR_ROUTER_0_in_intr_444_444 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 444,
};
const struct Sciclient_rmIrqIf NAVSS0_TCU_0_tcu_event_q_ns_intr_5_5_to_NAVSS0_INTR_ROUTER_0_in_intr_445_445 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 445,
};
const struct Sciclient_rmIrqIf NAVSS0_TCU_0_tcu_event_q_s_intr_6_6_to_NAVSS0_INTR_ROUTER_0_in_intr_446_446 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_NAVSS0_INTR_ROUTER_0,
    .rbase  = 446,
};
const struct Sciclient_rmIrqIf *const tisci_if_NAVSS0_TCU_0[] = {
    &NAVSS0_TCU_0_tcu_ras_intr_0_0_to_NAVSS0_INTR_ROUTER_0_in_intr_440_440,
    &NAVSS0_TCU_0_tcu_global_ns_intr_1_1_to_NAVSS0_INTR_ROUTER_0_in_intr_441_441,
    &NAVSS0_TCU_0_tcu_global_s_intr_2_2_to_NAVSS0_INTR_ROUTER_0_in_intr_442_442,
    &NAVSS0_TCU_0_tcu_cmd_sync_ns_intr_3_3_to_NAVSS0_INTR_ROUTER_0_in_intr_443_443,
    &NAVSS0_TCU_0_tcu_cmd_sync_s_intr_4_4_to_NAVSS0_INTR_ROUTER_0_in_intr_444_444,
    &NAVSS0_TCU_0_tcu_event_q_ns_intr_5_5_to_NAVSS0_INTR_ROUTER_0_in_intr_445_445,
    &NAVSS0_TCU_0_tcu_event_q_s_intr_6_6_to_NAVSS0_INTR_ROUTER_0_in_intr_446_446,
};
static const struct Sciclient_rmIrqNode tisci_irq_NAVSS0_TCU_0 = {
    .id = TISCI_DEV_NAVSS0_TCU_0,
    .n_if   = 7,
    .p_if   = &tisci_if_NAVSS0_TCU_0[0],
};

/* Start of MCU_NAVSS0_INTAGGR_0 interface definition */
const struct Sciclient_rmIrqIf MCU_NAVSS0_INTAGGR_0_intaggr_vintr_pend_0_255_to_MCU_NAVSS0_INTR_ROUTER_0_in_intr_0_255 = {
    .lbase  = 0,
    .len    = 256,
    .rid    = TISCI_DEV_MCU_NAVSS0_INTR_0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_NAVSS0_INTAGGR_0[] = {
    &MCU_NAVSS0_INTAGGR_0_intaggr_vintr_pend_0_255_to_MCU_NAVSS0_INTR_ROUTER_0_in_intr_0_255,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_NAVSS0_INTAGGR_0 = {
    .id = TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0,
    .n_if   = 1,
    .p_if   = &tisci_if_MCU_NAVSS0_INTAGGR_0[0],
};

/* Start of MCU_NAVSS0_INTR_ROUTER_0 interface definition */
const struct Sciclient_rmIrqIf MCU_NAVSS0_INTR_ROUTER_0_outl_intr_0_31_to_MCU_R5FSS0_CORE0_intr_64_95 = {
    .lbase  = 0,
    .len    = 32,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE0,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf MCU_NAVSS0_INTR_ROUTER_0_outl_intr_32_63_to_MCU_R5FSS0_CORE1_intr_64_95 = {
    .lbase  = 32,
    .len    = 32,
    .rid    = TISCI_DEV_MCU_R5FSS0_CORE1,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_NAVSS0_INTR_ROUTER_0[] = {
    &MCU_NAVSS0_INTR_ROUTER_0_outl_intr_0_31_to_MCU_R5FSS0_CORE0_intr_64_95,
    &MCU_NAVSS0_INTR_ROUTER_0_outl_intr_32_63_to_MCU_R5FSS0_CORE1_intr_64_95,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_NAVSS0_INTR_ROUTER_0 = {
    .id = TISCI_DEV_MCU_NAVSS0_INTR_0,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_NAVSS0_INTR_ROUTER_0[0],
};

/* Start of MCU_NAVSS0_MCRC_0 interface definition */
const struct Sciclient_rmIrqIf MCU_NAVSS0_MCRC_0_dma_event_intr_0_3_to_MCU_NAVSS0_INTAGGR_0_intaggr_levi_pend_0_3 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0,
    .rbase  = 0,
};
const struct Sciclient_rmIrqIf MCU_NAVSS0_MCRC_0_dma_event_intr_0_3_to_MCU_NAVSS0_INTR_ROUTER_0_in_intr_256_259 = {
    .lbase  = 0,
    .len    = 4,
    .rid    = TISCI_DEV_MCU_NAVSS0_INTR_0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf MCU_NAVSS0_MCRC_0_intaggr_vintr_pend_4_4_to_MCU_NAVSS0_INTR_ROUTER_0_in_intr_260_260 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_MCU_NAVSS0_INTR_0,
    .rbase  = 260,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_NAVSS0_MCRC_0[] = {
    &MCU_NAVSS0_MCRC_0_dma_event_intr_0_3_to_MCU_NAVSS0_INTAGGR_0_intaggr_levi_pend_0_3,
    &MCU_NAVSS0_MCRC_0_dma_event_intr_0_3_to_MCU_NAVSS0_INTR_ROUTER_0_in_intr_256_259,
    &MCU_NAVSS0_MCRC_0_intaggr_vintr_pend_4_4_to_MCU_NAVSS0_INTR_ROUTER_0_in_intr_260_260,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_NAVSS0_MCRC_0 = {
    .id = TISCI_DEV_MCU_NAVSS0_MCRC_0,
    .n_if   = 3,
    .p_if   = &tisci_if_MCU_NAVSS0_MCRC_0[0],
};

/* Start of PCIE0 interface definition */
const struct Sciclient_rmIrqIf PCIE0_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_67_67 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 67,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_68_68 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 68,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_69_69 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 69,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_32_32 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 32,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_33_33 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 33,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_34_34 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 34,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_35_35 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 35,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_36_36 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 36,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_37_37 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 37,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_38_38 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 38,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_39_39 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 39,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_10_10 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 10,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_20_20 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 20,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_32_32 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 32,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_203_203 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 203,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_204_204 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 204,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_205_205 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 205,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_206_206 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 206,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_207_207 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 207,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_208_208 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 208,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_209_209 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 209,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_210_210 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 210,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_211_211 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 211,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_212_212 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 212,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_213_213 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 213,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_203_203 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 203,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_204_204 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 204,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_205_205 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 205,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_206_206 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 206,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_207_207 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 207,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_208_208 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 208,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_209_209 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 209,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_210_210 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 210,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_211_211 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 211,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_212_212 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 212,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_213_213 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 213,
};
const struct Sciclient_rmIrqIf PCIE0_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_4_4 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf *const tisci_if_PCIE0[] = {
    &PCIE0_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_67_67,
    &PCIE0_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_68_68,
    &PCIE0_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_69_69,
    &PCIE0_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_32_32,
    &PCIE0_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_33_33,
    &PCIE0_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_34_34,
    &PCIE0_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_35_35,
    &PCIE0_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_36_36,
    &PCIE0_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_37_37,
    &PCIE0_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_38_38,
    &PCIE0_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_39_39,
    &PCIE0_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_10_10,
    &PCIE0_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_20_20,
    &PCIE0_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_32_32,
    &PCIE0_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_203_203,
    &PCIE0_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_204_204,
    &PCIE0_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_205_205,
    &PCIE0_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_206_206,
    &PCIE0_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_207_207,
    &PCIE0_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_208_208,
    &PCIE0_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_209_209,
    &PCIE0_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_210_210,
    &PCIE0_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_211_211,
    &PCIE0_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_212_212,
    &PCIE0_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_213_213,
    &PCIE0_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_203_203,
    &PCIE0_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_204_204,
    &PCIE0_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_205_205,
    &PCIE0_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_206_206,
    &PCIE0_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_207_207,
    &PCIE0_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_208_208,
    &PCIE0_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_209_209,
    &PCIE0_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_210_210,
    &PCIE0_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_211_211,
    &PCIE0_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_212_212,
    &PCIE0_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_213_213,
    &PCIE0_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_4_4,
};
static const struct Sciclient_rmIrqNode tisci_irq_PCIE0 = {
    .id = TISCI_DEV_PCIE0,
    .n_if   = 37,
    .p_if   = &tisci_if_PCIE0[0],
};

/* Start of PCIE1 interface definition */
const struct Sciclient_rmIrqIf PCIE1_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_73_73 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 73,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_74_74 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 74,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_75_75 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 75,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_40_40 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 40,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_41_41 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 41,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_42_42 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 42,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_43_43 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 43,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_44_44 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 44,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_45_45 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 45,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_46_46 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 46,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_47_47 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 47,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_legacy_pulse_9_9_to_R5FSS0_INTROUTER0_in_252_252 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 252,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_downstream_pulse_5_5_to_R5FSS0_INTROUTER0_in_253_253 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 253,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_flr_pulse_7_7_to_R5FSS0_INTROUTER0_in_254_254 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 254,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_phy_level_12_12_to_R5FSS0_INTROUTER0_in_255_255 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 255,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_local_level_11_11_to_R5FSS0_INTROUTER0_in_256_256 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_error_pulse_6_6_to_R5FSS0_INTROUTER0_in_257_257 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 257,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_link_state_pulse_10_10_to_R5FSS0_INTROUTER0_in_258_258 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 258,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_pwr_state_pulse_14_14_to_R5FSS0_INTROUTER0_in_259_259 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 259,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_ptm_valid_pulse_13_13_to_R5FSS0_INTROUTER0_in_260_260 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 260,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_hot_reset_pulse_8_8_to_R5FSS0_INTROUTER0_in_261_261 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 261,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_pend_3_3_to_R5FSS0_INTROUTER0_in_262_262 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 262,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_legacy_pulse_9_9_to_R5FSS1_INTROUTER0_in_252_252 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 252,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_downstream_pulse_5_5_to_R5FSS1_INTROUTER0_in_253_253 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 253,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_flr_pulse_7_7_to_R5FSS1_INTROUTER0_in_254_254 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 254,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_phy_level_12_12_to_R5FSS1_INTROUTER0_in_255_255 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 255,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_local_level_11_11_to_R5FSS1_INTROUTER0_in_256_256 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 256,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_error_pulse_6_6_to_R5FSS1_INTROUTER0_in_257_257 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 257,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_link_state_pulse_10_10_to_R5FSS1_INTROUTER0_in_258_258 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 258,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_pwr_state_pulse_14_14_to_R5FSS1_INTROUTER0_in_259_259 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 259,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_ptm_valid_pulse_13_13_to_R5FSS1_INTROUTER0_in_260_260 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 260,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_hot_reset_pulse_8_8_to_R5FSS1_INTROUTER0_in_261_261 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 261,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_pend_3_3_to_R5FSS1_INTROUTER0_in_262_262 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 262,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_11_11 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 11,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_21_21 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 21,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_33_33 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 33,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_215_215 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 215,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_216_216 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 216,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_217_217 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 217,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_218_218 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 218,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_219_219 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 219,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_220_220 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 220,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_221_221 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 221,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_222_222 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 222,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_223_223 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 223,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_224_224 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_225_225 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 225,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_215_215 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 215,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_216_216 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 216,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_217_217 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 217,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_218_218 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 218,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_219_219 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 219,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_220_220 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 220,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_221_221 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 221,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_222_222 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 222,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_223_223 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 223,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_224_224 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 224,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_225_225 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 225,
};
const struct Sciclient_rmIrqIf PCIE1_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_5_5 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf *const tisci_if_PCIE1[] = {
    &PCIE1_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_73_73,
    &PCIE1_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_74_74,
    &PCIE1_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_75_75,
    &PCIE1_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_40_40,
    &PCIE1_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_41_41,
    &PCIE1_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_42_42,
    &PCIE1_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_43_43,
    &PCIE1_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_44_44,
    &PCIE1_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_45_45,
    &PCIE1_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_46_46,
    &PCIE1_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_47_47,
    &PCIE1_pcie_legacy_pulse_9_9_to_R5FSS0_INTROUTER0_in_252_252,
    &PCIE1_pcie_downstream_pulse_5_5_to_R5FSS0_INTROUTER0_in_253_253,
    &PCIE1_pcie_flr_pulse_7_7_to_R5FSS0_INTROUTER0_in_254_254,
    &PCIE1_pcie_phy_level_12_12_to_R5FSS0_INTROUTER0_in_255_255,
    &PCIE1_pcie_local_level_11_11_to_R5FSS0_INTROUTER0_in_256_256,
    &PCIE1_pcie_error_pulse_6_6_to_R5FSS0_INTROUTER0_in_257_257,
    &PCIE1_pcie_link_state_pulse_10_10_to_R5FSS0_INTROUTER0_in_258_258,
    &PCIE1_pcie_pwr_state_pulse_14_14_to_R5FSS0_INTROUTER0_in_259_259,
    &PCIE1_pcie_ptm_valid_pulse_13_13_to_R5FSS0_INTROUTER0_in_260_260,
    &PCIE1_pcie_hot_reset_pulse_8_8_to_R5FSS0_INTROUTER0_in_261_261,
    &PCIE1_pcie_cpts_pend_3_3_to_R5FSS0_INTROUTER0_in_262_262,
    &PCIE1_pcie_legacy_pulse_9_9_to_R5FSS1_INTROUTER0_in_252_252,
    &PCIE1_pcie_downstream_pulse_5_5_to_R5FSS1_INTROUTER0_in_253_253,
    &PCIE1_pcie_flr_pulse_7_7_to_R5FSS1_INTROUTER0_in_254_254,
    &PCIE1_pcie_phy_level_12_12_to_R5FSS1_INTROUTER0_in_255_255,
    &PCIE1_pcie_local_level_11_11_to_R5FSS1_INTROUTER0_in_256_256,
    &PCIE1_pcie_error_pulse_6_6_to_R5FSS1_INTROUTER0_in_257_257,
    &PCIE1_pcie_link_state_pulse_10_10_to_R5FSS1_INTROUTER0_in_258_258,
    &PCIE1_pcie_pwr_state_pulse_14_14_to_R5FSS1_INTROUTER0_in_259_259,
    &PCIE1_pcie_ptm_valid_pulse_13_13_to_R5FSS1_INTROUTER0_in_260_260,
    &PCIE1_pcie_hot_reset_pulse_8_8_to_R5FSS1_INTROUTER0_in_261_261,
    &PCIE1_pcie_cpts_pend_3_3_to_R5FSS1_INTROUTER0_in_262_262,
    &PCIE1_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_11_11,
    &PCIE1_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_21_21,
    &PCIE1_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_33_33,
    &PCIE1_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_215_215,
    &PCIE1_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_216_216,
    &PCIE1_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_217_217,
    &PCIE1_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_218_218,
    &PCIE1_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_219_219,
    &PCIE1_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_220_220,
    &PCIE1_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_221_221,
    &PCIE1_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_222_222,
    &PCIE1_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_223_223,
    &PCIE1_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_224_224,
    &PCIE1_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_225_225,
    &PCIE1_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_215_215,
    &PCIE1_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_216_216,
    &PCIE1_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_217_217,
    &PCIE1_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_218_218,
    &PCIE1_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_219_219,
    &PCIE1_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_220_220,
    &PCIE1_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_221_221,
    &PCIE1_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_222_222,
    &PCIE1_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_223_223,
    &PCIE1_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_224_224,
    &PCIE1_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_225_225,
    &PCIE1_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_5_5,
};
static const struct Sciclient_rmIrqNode tisci_irq_PCIE1 = {
    .id = TISCI_DEV_PCIE1,
    .n_if   = 59,
    .p_if   = &tisci_if_PCIE1[0],
};

/* Start of PCIE2 interface definition */
const struct Sciclient_rmIrqIf PCIE2_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_79_79 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 79,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_80_80 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 80,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_81_81 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 81,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_48_48 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_49_49 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 49,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_50_50 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 50,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_51_51 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 51,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_52_52 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 52,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_53_53 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 53,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_54_54 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 54,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_55_55 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 55,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_legacy_pulse_9_9_to_R5FSS0_INTROUTER0_in_19_19 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 19,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_downstream_pulse_5_5_to_R5FSS0_INTROUTER0_in_20_20 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 20,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_flr_pulse_7_7_to_R5FSS0_INTROUTER0_in_21_21 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 21,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_phy_level_12_12_to_R5FSS0_INTROUTER0_in_22_22 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 22,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_local_level_11_11_to_R5FSS0_INTROUTER0_in_23_23 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 23,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_error_pulse_6_6_to_R5FSS0_INTROUTER0_in_24_24 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_link_state_pulse_10_10_to_R5FSS0_INTROUTER0_in_25_25 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 25,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_pwr_state_pulse_14_14_to_R5FSS0_INTROUTER0_in_26_26 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 26,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_ptm_valid_pulse_13_13_to_R5FSS0_INTROUTER0_in_27_27 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 27,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_hot_reset_pulse_8_8_to_R5FSS0_INTROUTER0_in_28_28 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 28,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_pend_3_3_to_R5FSS0_INTROUTER0_in_29_29 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 29,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_legacy_pulse_9_9_to_R5FSS1_INTROUTER0_in_19_19 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 19,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_downstream_pulse_5_5_to_R5FSS1_INTROUTER0_in_20_20 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 20,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_flr_pulse_7_7_to_R5FSS1_INTROUTER0_in_21_21 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 21,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_phy_level_12_12_to_R5FSS1_INTROUTER0_in_22_22 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 22,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_local_level_11_11_to_R5FSS1_INTROUTER0_in_23_23 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 23,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_error_pulse_6_6_to_R5FSS1_INTROUTER0_in_24_24 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 24,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_link_state_pulse_10_10_to_R5FSS1_INTROUTER0_in_25_25 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 25,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_pwr_state_pulse_14_14_to_R5FSS1_INTROUTER0_in_26_26 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 26,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_ptm_valid_pulse_13_13_to_R5FSS1_INTROUTER0_in_27_27 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 27,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_hot_reset_pulse_8_8_to_R5FSS1_INTROUTER0_in_28_28 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 28,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_pend_3_3_to_R5FSS1_INTROUTER0_in_29_29 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 29,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_12_12 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 12,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_22_22 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 22,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_34_34 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 34,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_227_227 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 227,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_228_228 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 228,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_229_229 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 229,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_230_230 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 230,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_231_231 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 231,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_232_232 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 232,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_233_233 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 233,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_234_234 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 234,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_235_235 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 235,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_236_236 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 236,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_237_237 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 237,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_227_227 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 227,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_228_228 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 228,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_229_229 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 229,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_230_230 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 230,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_231_231 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 231,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_232_232 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 232,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_233_233 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 233,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_234_234 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 234,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_235_235 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 235,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_236_236 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 236,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_237_237 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 237,
};
const struct Sciclient_rmIrqIf PCIE2_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_6_6 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 6,
};
const struct Sciclient_rmIrqIf *const tisci_if_PCIE2[] = {
    &PCIE2_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_79_79,
    &PCIE2_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_80_80,
    &PCIE2_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_81_81,
    &PCIE2_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_48_48,
    &PCIE2_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_49_49,
    &PCIE2_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_50_50,
    &PCIE2_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_51_51,
    &PCIE2_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_52_52,
    &PCIE2_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_53_53,
    &PCIE2_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_54_54,
    &PCIE2_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_55_55,
    &PCIE2_pcie_legacy_pulse_9_9_to_R5FSS0_INTROUTER0_in_19_19,
    &PCIE2_pcie_downstream_pulse_5_5_to_R5FSS0_INTROUTER0_in_20_20,
    &PCIE2_pcie_flr_pulse_7_7_to_R5FSS0_INTROUTER0_in_21_21,
    &PCIE2_pcie_phy_level_12_12_to_R5FSS0_INTROUTER0_in_22_22,
    &PCIE2_pcie_local_level_11_11_to_R5FSS0_INTROUTER0_in_23_23,
    &PCIE2_pcie_error_pulse_6_6_to_R5FSS0_INTROUTER0_in_24_24,
    &PCIE2_pcie_link_state_pulse_10_10_to_R5FSS0_INTROUTER0_in_25_25,
    &PCIE2_pcie_pwr_state_pulse_14_14_to_R5FSS0_INTROUTER0_in_26_26,
    &PCIE2_pcie_ptm_valid_pulse_13_13_to_R5FSS0_INTROUTER0_in_27_27,
    &PCIE2_pcie_hot_reset_pulse_8_8_to_R5FSS0_INTROUTER0_in_28_28,
    &PCIE2_pcie_cpts_pend_3_3_to_R5FSS0_INTROUTER0_in_29_29,
    &PCIE2_pcie_legacy_pulse_9_9_to_R5FSS1_INTROUTER0_in_19_19,
    &PCIE2_pcie_downstream_pulse_5_5_to_R5FSS1_INTROUTER0_in_20_20,
    &PCIE2_pcie_flr_pulse_7_7_to_R5FSS1_INTROUTER0_in_21_21,
    &PCIE2_pcie_phy_level_12_12_to_R5FSS1_INTROUTER0_in_22_22,
    &PCIE2_pcie_local_level_11_11_to_R5FSS1_INTROUTER0_in_23_23,
    &PCIE2_pcie_error_pulse_6_6_to_R5FSS1_INTROUTER0_in_24_24,
    &PCIE2_pcie_link_state_pulse_10_10_to_R5FSS1_INTROUTER0_in_25_25,
    &PCIE2_pcie_pwr_state_pulse_14_14_to_R5FSS1_INTROUTER0_in_26_26,
    &PCIE2_pcie_ptm_valid_pulse_13_13_to_R5FSS1_INTROUTER0_in_27_27,
    &PCIE2_pcie_hot_reset_pulse_8_8_to_R5FSS1_INTROUTER0_in_28_28,
    &PCIE2_pcie_cpts_pend_3_3_to_R5FSS1_INTROUTER0_in_29_29,
    &PCIE2_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_12_12,
    &PCIE2_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_22_22,
    &PCIE2_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_34_34,
    &PCIE2_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_227_227,
    &PCIE2_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_228_228,
    &PCIE2_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_229_229,
    &PCIE2_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_230_230,
    &PCIE2_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_231_231,
    &PCIE2_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_232_232,
    &PCIE2_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_233_233,
    &PCIE2_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_234_234,
    &PCIE2_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_235_235,
    &PCIE2_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_236_236,
    &PCIE2_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_237_237,
    &PCIE2_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_227_227,
    &PCIE2_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_228_228,
    &PCIE2_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_229_229,
    &PCIE2_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_230_230,
    &PCIE2_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_231_231,
    &PCIE2_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_232_232,
    &PCIE2_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_233_233,
    &PCIE2_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_234_234,
    &PCIE2_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_235_235,
    &PCIE2_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_236_236,
    &PCIE2_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_237_237,
    &PCIE2_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_6_6,
};
static const struct Sciclient_rmIrqNode tisci_irq_PCIE2 = {
    .id = TISCI_DEV_PCIE2,
    .n_if   = 59,
    .p_if   = &tisci_if_PCIE2[0],
};

/* Start of PCIE3 interface definition */
const struct Sciclient_rmIrqIf PCIE3_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_85_85 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 85,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_86_86 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 86,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_87_87 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 87,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_56_56 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 56,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_57_57 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 57,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_58_58 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 58,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_59_59 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 59,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_60_60 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 60,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_61_61 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 61,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_62_62 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 62,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_63_63 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
    .rbase  = 63,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_legacy_pulse_9_9_to_R5FSS0_INTROUTER0_in_54_54 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 54,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_downstream_pulse_5_5_to_R5FSS0_INTROUTER0_in_55_55 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 55,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_flr_pulse_7_7_to_R5FSS0_INTROUTER0_in_56_56 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 56,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_phy_level_12_12_to_R5FSS0_INTROUTER0_in_57_57 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 57,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_local_level_11_11_to_R5FSS0_INTROUTER0_in_58_58 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 58,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_error_pulse_6_6_to_R5FSS0_INTROUTER0_in_59_59 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 59,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_link_state_pulse_10_10_to_R5FSS0_INTROUTER0_in_60_60 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 60,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_pwr_state_pulse_14_14_to_R5FSS0_INTROUTER0_in_61_61 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 61,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_ptm_valid_pulse_13_13_to_R5FSS0_INTROUTER0_in_62_62 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 62,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_hot_reset_pulse_8_8_to_R5FSS0_INTROUTER0_in_63_63 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 63,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_pend_3_3_to_R5FSS0_INTROUTER0_in_64_64 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_legacy_pulse_9_9_to_R5FSS1_INTROUTER0_in_54_54 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 54,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_downstream_pulse_5_5_to_R5FSS1_INTROUTER0_in_55_55 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 55,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_flr_pulse_7_7_to_R5FSS1_INTROUTER0_in_56_56 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 56,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_phy_level_12_12_to_R5FSS1_INTROUTER0_in_57_57 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 57,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_local_level_11_11_to_R5FSS1_INTROUTER0_in_58_58 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 58,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_error_pulse_6_6_to_R5FSS1_INTROUTER0_in_59_59 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 59,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_link_state_pulse_10_10_to_R5FSS1_INTROUTER0_in_60_60 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 60,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_pwr_state_pulse_14_14_to_R5FSS1_INTROUTER0_in_61_61 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 61,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_ptm_valid_pulse_13_13_to_R5FSS1_INTROUTER0_in_62_62 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 62,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_hot_reset_pulse_8_8_to_R5FSS1_INTROUTER0_in_63_63 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 63,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_pend_3_3_to_R5FSS1_INTROUTER0_in_64_64 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 64,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_13_13 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 13,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_23_23 = {
    .lbase  = 2,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 23,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_35_35 = {
    .lbase  = 4,
    .len    = 1,
    .rid    = TISCI_DEV_TIMESYNC_INTRTR0,
    .rbase  = 35,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_239_239 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 239,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_240_240 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 240,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_241_241 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 241,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_242_242 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 242,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_243_243 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 243,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_244_244 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 244,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_245_245 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 245,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_246_246 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 246,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_247_247 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 247,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_248_248 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 248,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_249_249 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 249,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_239_239 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 239,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_240_240 = {
    .lbase  = 5,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 240,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_241_241 = {
    .lbase  = 7,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 241,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_242_242 = {
    .lbase  = 12,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 242,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_243_243 = {
    .lbase  = 11,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 243,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_244_244 = {
    .lbase  = 6,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 244,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_245_245 = {
    .lbase  = 10,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 245,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_246_246 = {
    .lbase  = 14,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 246,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_247_247 = {
    .lbase  = 13,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 247,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_248_248 = {
    .lbase  = 8,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 248,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_249_249 = {
    .lbase  = 3,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 249,
};
const struct Sciclient_rmIrqIf PCIE3_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_7_7 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_CMPEVENT_INTRTR0,
    .rbase  = 7,
};
const struct Sciclient_rmIrqIf *const tisci_if_PCIE3[] = {
    &PCIE3_pcie_phy_level_12_12_to_MAIN2MCU_LVL_INTRTR0_in_85_85,
    &PCIE3_pcie_local_level_11_11_to_MAIN2MCU_LVL_INTRTR0_in_86_86,
    &PCIE3_pcie_cpts_pend_3_3_to_MAIN2MCU_LVL_INTRTR0_in_87_87,
    &PCIE3_pcie_legacy_pulse_9_9_to_MAIN2MCU_PLS_INTRTR0_in_56_56,
    &PCIE3_pcie_downstream_pulse_5_5_to_MAIN2MCU_PLS_INTRTR0_in_57_57,
    &PCIE3_pcie_flr_pulse_7_7_to_MAIN2MCU_PLS_INTRTR0_in_58_58,
    &PCIE3_pcie_error_pulse_6_6_to_MAIN2MCU_PLS_INTRTR0_in_59_59,
    &PCIE3_pcie_link_state_pulse_10_10_to_MAIN2MCU_PLS_INTRTR0_in_60_60,
    &PCIE3_pcie_pwr_state_pulse_14_14_to_MAIN2MCU_PLS_INTRTR0_in_61_61,
    &PCIE3_pcie_ptm_valid_pulse_13_13_to_MAIN2MCU_PLS_INTRTR0_in_62_62,
    &PCIE3_pcie_hot_reset_pulse_8_8_to_MAIN2MCU_PLS_INTRTR0_in_63_63,
    &PCIE3_pcie_legacy_pulse_9_9_to_R5FSS0_INTROUTER0_in_54_54,
    &PCIE3_pcie_downstream_pulse_5_5_to_R5FSS0_INTROUTER0_in_55_55,
    &PCIE3_pcie_flr_pulse_7_7_to_R5FSS0_INTROUTER0_in_56_56,
    &PCIE3_pcie_phy_level_12_12_to_R5FSS0_INTROUTER0_in_57_57,
    &PCIE3_pcie_local_level_11_11_to_R5FSS0_INTROUTER0_in_58_58,
    &PCIE3_pcie_error_pulse_6_6_to_R5FSS0_INTROUTER0_in_59_59,
    &PCIE3_pcie_link_state_pulse_10_10_to_R5FSS0_INTROUTER0_in_60_60,
    &PCIE3_pcie_pwr_state_pulse_14_14_to_R5FSS0_INTROUTER0_in_61_61,
    &PCIE3_pcie_ptm_valid_pulse_13_13_to_R5FSS0_INTROUTER0_in_62_62,
    &PCIE3_pcie_hot_reset_pulse_8_8_to_R5FSS0_INTROUTER0_in_63_63,
    &PCIE3_pcie_cpts_pend_3_3_to_R5FSS0_INTROUTER0_in_64_64,
    &PCIE3_pcie_legacy_pulse_9_9_to_R5FSS1_INTROUTER0_in_54_54,
    &PCIE3_pcie_downstream_pulse_5_5_to_R5FSS1_INTROUTER0_in_55_55,
    &PCIE3_pcie_flr_pulse_7_7_to_R5FSS1_INTROUTER0_in_56_56,
    &PCIE3_pcie_phy_level_12_12_to_R5FSS1_INTROUTER0_in_57_57,
    &PCIE3_pcie_local_level_11_11_to_R5FSS1_INTROUTER0_in_58_58,
    &PCIE3_pcie_error_pulse_6_6_to_R5FSS1_INTROUTER0_in_59_59,
    &PCIE3_pcie_link_state_pulse_10_10_to_R5FSS1_INTROUTER0_in_60_60,
    &PCIE3_pcie_pwr_state_pulse_14_14_to_R5FSS1_INTROUTER0_in_61_61,
    &PCIE3_pcie_ptm_valid_pulse_13_13_to_R5FSS1_INTROUTER0_in_62_62,
    &PCIE3_pcie_hot_reset_pulse_8_8_to_R5FSS1_INTROUTER0_in_63_63,
    &PCIE3_pcie_cpts_pend_3_3_to_R5FSS1_INTROUTER0_in_64_64,
    &PCIE3_pcie_cpts_genf0_1_1_to_TIMESYNC_INTRTR0_in_13_13,
    &PCIE3_pcie_cpts_hw1_push_2_2_to_TIMESYNC_INTRTR0_in_23_23,
    &PCIE3_pcie_cpts_sync_4_4_to_TIMESYNC_INTRTR0_in_35_35,
    &PCIE3_pcie_legacy_pulse_9_9_to_C66SS0_INTROUTER0_in_239_239,
    &PCIE3_pcie_downstream_pulse_5_5_to_C66SS0_INTROUTER0_in_240_240,
    &PCIE3_pcie_flr_pulse_7_7_to_C66SS0_INTROUTER0_in_241_241,
    &PCIE3_pcie_phy_level_12_12_to_C66SS0_INTROUTER0_in_242_242,
    &PCIE3_pcie_local_level_11_11_to_C66SS0_INTROUTER0_in_243_243,
    &PCIE3_pcie_error_pulse_6_6_to_C66SS0_INTROUTER0_in_244_244,
    &PCIE3_pcie_link_state_pulse_10_10_to_C66SS0_INTROUTER0_in_245_245,
    &PCIE3_pcie_pwr_state_pulse_14_14_to_C66SS0_INTROUTER0_in_246_246,
    &PCIE3_pcie_ptm_valid_pulse_13_13_to_C66SS0_INTROUTER0_in_247_247,
    &PCIE3_pcie_hot_reset_pulse_8_8_to_C66SS0_INTROUTER0_in_248_248,
    &PCIE3_pcie_cpts_pend_3_3_to_C66SS0_INTROUTER0_in_249_249,
    &PCIE3_pcie_legacy_pulse_9_9_to_C66SS1_INTROUTER0_in_239_239,
    &PCIE3_pcie_downstream_pulse_5_5_to_C66SS1_INTROUTER0_in_240_240,
    &PCIE3_pcie_flr_pulse_7_7_to_C66SS1_INTROUTER0_in_241_241,
    &PCIE3_pcie_phy_level_12_12_to_C66SS1_INTROUTER0_in_242_242,
    &PCIE3_pcie_local_level_11_11_to_C66SS1_INTROUTER0_in_243_243,
    &PCIE3_pcie_error_pulse_6_6_to_C66SS1_INTROUTER0_in_244_244,
    &PCIE3_pcie_link_state_pulse_10_10_to_C66SS1_INTROUTER0_in_245_245,
    &PCIE3_pcie_pwr_state_pulse_14_14_to_C66SS1_INTROUTER0_in_246_246,
    &PCIE3_pcie_ptm_valid_pulse_13_13_to_C66SS1_INTROUTER0_in_247_247,
    &PCIE3_pcie_hot_reset_pulse_8_8_to_C66SS1_INTROUTER0_in_248_248,
    &PCIE3_pcie_cpts_pend_3_3_to_C66SS1_INTROUTER0_in_249_249,
    &PCIE3_pcie_cpts_comp_0_0_to_CMPEVENT_INTRTR0_in_7_7,
};
static const struct Sciclient_rmIrqNode tisci_irq_PCIE3 = {
    .id = TISCI_DEV_PCIE3,
    .n_if   = 59,
    .p_if   = &tisci_if_PCIE3[0],
};

/* Start of RTI24 interface definition */
const struct Sciclient_rmIrqIf RTI24_intr_wwd_0_0_to_C66SS0_INTROUTER0_in_308_308 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 308,
};
const struct Sciclient_rmIrqIf *const tisci_if_RTI24[] = {
    &RTI24_intr_wwd_0_0_to_C66SS0_INTROUTER0_in_308_308,
};
static const struct Sciclient_rmIrqNode tisci_irq_RTI24 = {
    .id = TISCI_DEV_RTI24,
    .n_if   = 1,
    .p_if   = &tisci_if_RTI24[0],
};

/* Start of RTI25 interface definition */
const struct Sciclient_rmIrqIf RTI25_intr_wwd_0_0_to_C66SS1_INTROUTER0_in_308_308 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 308,
};
const struct Sciclient_rmIrqIf *const tisci_if_RTI25[] = {
    &RTI25_intr_wwd_0_0_to_C66SS1_INTROUTER0_in_308_308,
};
static const struct Sciclient_rmIrqNode tisci_irq_RTI25 = {
    .id = TISCI_DEV_RTI25,
    .n_if   = 1,
    .p_if   = &tisci_if_RTI25[0],
};

/* Start of SA2_UL0 interface definition */
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_trng_1_1_to_MAIN2MCU_LVL_INTRTR0_in_4_4 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 4,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_pka_0_0_to_MAIN2MCU_LVL_INTRTR0_in_5_5 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 5,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_pka_0_0_to_C66SS0_INTROUTER0_in_121_121 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 121,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_trng_1_1_to_C66SS0_INTROUTER0_in_122_122 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 122,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_pka_0_0_to_C66SS1_INTROUTER0_in_120_120 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 120,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_trng_1_1_to_C66SS1_INTROUTER0_in_121_121 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 121,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_pka_0_0_to_R5FSS0_INTROUTER0_in_314_314 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 314,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_trng_1_1_to_R5FSS0_INTROUTER0_in_315_315 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 315,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_pka_0_0_to_R5FSS1_INTROUTER0_in_314_314 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 314,
};
const struct Sciclient_rmIrqIf SA2_UL0_sa_ul_trng_1_1_to_R5FSS1_INTROUTER0_in_315_315 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 315,
};
const struct Sciclient_rmIrqIf *const tisci_if_SA2_UL0[] = {
    &SA2_UL0_sa_ul_trng_1_1_to_MAIN2MCU_LVL_INTRTR0_in_4_4,
    &SA2_UL0_sa_ul_pka_0_0_to_MAIN2MCU_LVL_INTRTR0_in_5_5,
    &SA2_UL0_sa_ul_pka_0_0_to_C66SS0_INTROUTER0_in_121_121,
    &SA2_UL0_sa_ul_trng_1_1_to_C66SS0_INTROUTER0_in_122_122,
    &SA2_UL0_sa_ul_pka_0_0_to_C66SS1_INTROUTER0_in_120_120,
    &SA2_UL0_sa_ul_trng_1_1_to_C66SS1_INTROUTER0_in_121_121,
    &SA2_UL0_sa_ul_pka_0_0_to_R5FSS0_INTROUTER0_in_314_314,
    &SA2_UL0_sa_ul_trng_1_1_to_R5FSS0_INTROUTER0_in_315_315,
    &SA2_UL0_sa_ul_pka_0_0_to_R5FSS1_INTROUTER0_in_314_314,
    &SA2_UL0_sa_ul_trng_1_1_to_R5FSS1_INTROUTER0_in_315_315,
};
static const struct Sciclient_rmIrqNode tisci_irq_SA2_UL0 = {
    .id = TISCI_DEV_SA2_UL0,
    .n_if   = 10,
    .p_if   = &tisci_if_SA2_UL0[0],
};

/* Start of MCU_SA2_UL0 interface definition */
const struct Sciclient_rmIrqIf MCU_SA2_UL0_sa_ul_pka_0_0_to_R5FSS0_INTROUTER0_in_147_147 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 147,
};
const struct Sciclient_rmIrqIf MCU_SA2_UL0_sa_ul_trng_1_1_to_R5FSS0_INTROUTER0_in_148_148 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 148,
};
const struct Sciclient_rmIrqIf MCU_SA2_UL0_sa_ul_pka_0_0_to_R5FSS1_INTROUTER0_in_147_147 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 147,
};
const struct Sciclient_rmIrqIf MCU_SA2_UL0_sa_ul_trng_1_1_to_R5FSS1_INTROUTER0_in_148_148 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 148,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_SA2_UL0[] = {
    &MCU_SA2_UL0_sa_ul_pka_0_0_to_R5FSS0_INTROUTER0_in_147_147,
    &MCU_SA2_UL0_sa_ul_trng_1_1_to_R5FSS0_INTROUTER0_in_148_148,
    &MCU_SA2_UL0_sa_ul_pka_0_0_to_R5FSS1_INTROUTER0_in_147_147,
    &MCU_SA2_UL0_sa_ul_trng_1_1_to_R5FSS1_INTROUTER0_in_148_148,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_SA2_UL0 = {
    .id = TISCI_DEV_MCU_SA2_UL0,
    .n_if   = 4,
    .p_if   = &tisci_if_MCU_SA2_UL0[0],
};

/* Start of MCSPI0 interface definition */
const struct Sciclient_rmIrqIf MCSPI0_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_48_48 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 48,
};
const struct Sciclient_rmIrqIf MCSPI0_intr_spi_0_0_to_C66SS0_INTROUTER0_in_340_340 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 340,
};
const struct Sciclient_rmIrqIf MCSPI0_intr_spi_0_0_to_C66SS1_INTROUTER0_in_340_340 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 340,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI0[] = {
    &MCSPI0_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_48_48,
    &MCSPI0_intr_spi_0_0_to_C66SS0_INTROUTER0_in_340_340,
    &MCSPI0_intr_spi_0_0_to_C66SS1_INTROUTER0_in_340_340,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI0 = {
    .id = TISCI_DEV_MCSPI0,
    .n_if   = 3,
    .p_if   = &tisci_if_MCSPI0[0],
};

/* Start of MCSPI1 interface definition */
const struct Sciclient_rmIrqIf MCSPI1_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_49_49 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 49,
};
const struct Sciclient_rmIrqIf MCSPI1_intr_spi_0_0_to_C66SS0_INTROUTER0_in_341_341 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 341,
};
const struct Sciclient_rmIrqIf MCSPI1_intr_spi_0_0_to_C66SS1_INTROUTER0_in_341_341 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 341,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI1[] = {
    &MCSPI1_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_49_49,
    &MCSPI1_intr_spi_0_0_to_C66SS0_INTROUTER0_in_341_341,
    &MCSPI1_intr_spi_0_0_to_C66SS1_INTROUTER0_in_341_341,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI1 = {
    .id = TISCI_DEV_MCSPI1,
    .n_if   = 3,
    .p_if   = &tisci_if_MCSPI1[0],
};

/* Start of MCSPI2 interface definition */
const struct Sciclient_rmIrqIf MCSPI2_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_50_50 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 50,
};
const struct Sciclient_rmIrqIf MCSPI2_intr_spi_0_0_to_C66SS0_INTROUTER0_in_342_342 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 342,
};
const struct Sciclient_rmIrqIf MCSPI2_intr_spi_0_0_to_C66SS1_INTROUTER0_in_342_342 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 342,
};
const struct Sciclient_rmIrqIf MCSPI2_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_195_195 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 195,
};
const struct Sciclient_rmIrqIf MCSPI2_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_195_195 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 195,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI2[] = {
    &MCSPI2_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_50_50,
    &MCSPI2_intr_spi_0_0_to_C66SS0_INTROUTER0_in_342_342,
    &MCSPI2_intr_spi_0_0_to_C66SS1_INTROUTER0_in_342_342,
    &MCSPI2_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_195_195,
    &MCSPI2_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_195_195,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI2 = {
    .id = TISCI_DEV_MCSPI2,
    .n_if   = 5,
    .p_if   = &tisci_if_MCSPI2[0],
};

/* Start of MCSPI3 interface definition */
const struct Sciclient_rmIrqIf MCSPI3_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_51_51 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 51,
};
const struct Sciclient_rmIrqIf MCSPI3_intr_spi_0_0_to_C66SS0_INTROUTER0_in_17_17 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 17,
};
const struct Sciclient_rmIrqIf MCSPI3_intr_spi_0_0_to_C66SS1_INTROUTER0_in_17_17 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 17,
};
const struct Sciclient_rmIrqIf MCSPI3_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_196_196 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 196,
};
const struct Sciclient_rmIrqIf MCSPI3_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_196_196 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 196,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI3[] = {
    &MCSPI3_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_51_51,
    &MCSPI3_intr_spi_0_0_to_C66SS0_INTROUTER0_in_17_17,
    &MCSPI3_intr_spi_0_0_to_C66SS1_INTROUTER0_in_17_17,
    &MCSPI3_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_196_196,
    &MCSPI3_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_196_196,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI3 = {
    .id = TISCI_DEV_MCSPI3,
    .n_if   = 5,
    .p_if   = &tisci_if_MCSPI3[0],
};

/* Start of MCSPI4 interface definition */
const struct Sciclient_rmIrqIf MCSPI4_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_52_52 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 52,
};
const struct Sciclient_rmIrqIf MCSPI4_intr_spi_0_0_to_C66SS0_INTROUTER0_in_18_18 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 18,
};
const struct Sciclient_rmIrqIf MCSPI4_intr_spi_0_0_to_C66SS1_INTROUTER0_in_18_18 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 18,
};
const struct Sciclient_rmIrqIf MCSPI4_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_197_197 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 197,
};
const struct Sciclient_rmIrqIf MCSPI4_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_197_197 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 197,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI4[] = {
    &MCSPI4_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_52_52,
    &MCSPI4_intr_spi_0_0_to_C66SS0_INTROUTER0_in_18_18,
    &MCSPI4_intr_spi_0_0_to_C66SS1_INTROUTER0_in_18_18,
    &MCSPI4_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_197_197,
    &MCSPI4_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_197_197,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI4 = {
    .id = TISCI_DEV_MCSPI4,
    .n_if   = 5,
    .p_if   = &tisci_if_MCSPI4[0],
};

/* Start of MCSPI5 interface definition */
const struct Sciclient_rmIrqIf MCSPI5_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_53_53 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 53,
};
const struct Sciclient_rmIrqIf MCSPI5_intr_spi_0_0_to_C66SS0_INTROUTER0_in_19_19 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 19,
};
const struct Sciclient_rmIrqIf MCSPI5_intr_spi_0_0_to_C66SS1_INTROUTER0_in_19_19 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 19,
};
const struct Sciclient_rmIrqIf MCSPI5_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_198_198 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 198,
};
const struct Sciclient_rmIrqIf MCSPI5_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_198_198 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 198,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI5[] = {
    &MCSPI5_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_53_53,
    &MCSPI5_intr_spi_0_0_to_C66SS0_INTROUTER0_in_19_19,
    &MCSPI5_intr_spi_0_0_to_C66SS1_INTROUTER0_in_19_19,
    &MCSPI5_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_198_198,
    &MCSPI5_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_198_198,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI5 = {
    .id = TISCI_DEV_MCSPI5,
    .n_if   = 5,
    .p_if   = &tisci_if_MCSPI5[0],
};

/* Start of MCSPI6 interface definition */
const struct Sciclient_rmIrqIf MCSPI6_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_54_54 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 54,
};
const struct Sciclient_rmIrqIf MCSPI6_intr_spi_0_0_to_C66SS0_INTROUTER0_in_20_20 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 20,
};
const struct Sciclient_rmIrqIf MCSPI6_intr_spi_0_0_to_C66SS1_INTROUTER0_in_20_20 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 20,
};
const struct Sciclient_rmIrqIf MCSPI6_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_199_199 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 199,
};
const struct Sciclient_rmIrqIf MCSPI6_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_199_199 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 199,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI6[] = {
    &MCSPI6_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_54_54,
    &MCSPI6_intr_spi_0_0_to_C66SS0_INTROUTER0_in_20_20,
    &MCSPI6_intr_spi_0_0_to_C66SS1_INTROUTER0_in_20_20,
    &MCSPI6_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_199_199,
    &MCSPI6_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_199_199,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI6 = {
    .id = TISCI_DEV_MCSPI6,
    .n_if   = 5,
    .p_if   = &tisci_if_MCSPI6[0],
};

/* Start of MCSPI7 interface definition */
const struct Sciclient_rmIrqIf MCSPI7_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_55_55 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 55,
};
const struct Sciclient_rmIrqIf MCSPI7_intr_spi_0_0_to_C66SS0_INTROUTER0_in_343_343 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 343,
};
const struct Sciclient_rmIrqIf MCSPI7_intr_spi_0_0_to_C66SS1_INTROUTER0_in_343_343 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 343,
};
const struct Sciclient_rmIrqIf MCSPI7_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_200_200 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 200,
};
const struct Sciclient_rmIrqIf MCSPI7_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_200_200 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 200,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCSPI7[] = {
    &MCSPI7_intr_spi_0_0_to_MAIN2MCU_LVL_INTRTR0_in_55_55,
    &MCSPI7_intr_spi_0_0_to_C66SS0_INTROUTER0_in_343_343,
    &MCSPI7_intr_spi_0_0_to_C66SS1_INTROUTER0_in_343_343,
    &MCSPI7_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_200_200,
    &MCSPI7_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_200_200,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCSPI7 = {
    .id = TISCI_DEV_MCSPI7,
    .n_if   = 5,
    .p_if   = &tisci_if_MCSPI7[0],
};

/* Start of MCU_MCSPI0 interface definition */
const struct Sciclient_rmIrqIf MCU_MCSPI0_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_136_136 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 136,
};
const struct Sciclient_rmIrqIf MCU_MCSPI0_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_136_136 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 136,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_MCSPI0[] = {
    &MCU_MCSPI0_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_136_136,
    &MCU_MCSPI0_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_136_136,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_MCSPI0 = {
    .id = TISCI_DEV_MCU_MCSPI0,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_MCSPI0[0],
};

/* Start of MCU_MCSPI1 interface definition */
const struct Sciclient_rmIrqIf MCU_MCSPI1_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_137_137 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 137,
};
const struct Sciclient_rmIrqIf MCU_MCSPI1_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_137_137 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 137,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_MCSPI1[] = {
    &MCU_MCSPI1_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_137_137,
    &MCU_MCSPI1_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_137_137,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_MCSPI1 = {
    .id = TISCI_DEV_MCU_MCSPI1,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_MCSPI1[0],
};

/* Start of MCU_MCSPI2 interface definition */
const struct Sciclient_rmIrqIf MCU_MCSPI2_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_138_138 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 138,
};
const struct Sciclient_rmIrqIf MCU_MCSPI2_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_138_138 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 138,
};
const struct Sciclient_rmIrqIf *const tisci_if_MCU_MCSPI2[] = {
    &MCU_MCSPI2_intr_spi_0_0_to_R5FSS0_INTROUTER0_in_138_138,
    &MCU_MCSPI2_intr_spi_0_0_to_R5FSS1_INTROUTER0_in_138_138,
};
static const struct Sciclient_rmIrqNode tisci_irq_MCU_MCSPI2 = {
    .id = TISCI_DEV_MCU_MCSPI2,
    .n_if   = 2,
    .p_if   = &tisci_if_MCU_MCSPI2[0],
};

/* Start of UFS0 interface definition */
const struct Sciclient_rmIrqIf UFS0_ufs_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_218_218 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 218,
};
const struct Sciclient_rmIrqIf UFS0_ufs_intr_0_0_to_R5FSS0_INTROUTER0_in_312_312 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 312,
};
const struct Sciclient_rmIrqIf UFS0_ufs_intr_0_0_to_R5FSS1_INTROUTER0_in_312_312 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 312,
};
const struct Sciclient_rmIrqIf *const tisci_if_UFS0[] = {
    &UFS0_ufs_intr_0_0_to_MAIN2MCU_LVL_INTRTR0_in_218_218,
    &UFS0_ufs_intr_0_0_to_R5FSS0_INTROUTER0_in_312_312,
    &UFS0_ufs_intr_0_0_to_R5FSS1_INTROUTER0_in_312_312,
};
static const struct Sciclient_rmIrqNode tisci_irq_UFS0 = {
    .id = TISCI_DEV_UFS0,
    .n_if   = 3,
    .p_if   = &tisci_if_UFS0[0],
};

/* Start of UART1 interface definition */
const struct Sciclient_rmIrqIf UART1_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_97_97 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 97,
};
const struct Sciclient_rmIrqIf UART1_usart_irq_0_0_to_C66SS0_INTROUTER0_in_352_352 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 352,
};
const struct Sciclient_rmIrqIf UART1_usart_irq_0_0_to_C66SS1_INTROUTER0_in_352_352 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 352,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART1[] = {
    &UART1_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_97_97,
    &UART1_usart_irq_0_0_to_C66SS0_INTROUTER0_in_352_352,
    &UART1_usart_irq_0_0_to_C66SS1_INTROUTER0_in_352_352,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART1 = {
    .id = TISCI_DEV_UART1,
    .n_if   = 3,
    .p_if   = &tisci_if_UART1[0],
};

/* Start of UART2 interface definition */
const struct Sciclient_rmIrqIf UART2_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_98_98 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 98,
};
const struct Sciclient_rmIrqIf UART2_usart_irq_0_0_to_C66SS0_INTROUTER0_in_353_353 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 353,
};
const struct Sciclient_rmIrqIf UART2_usart_irq_0_0_to_C66SS1_INTROUTER0_in_353_353 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 353,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART2[] = {
    &UART2_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_98_98,
    &UART2_usart_irq_0_0_to_C66SS0_INTROUTER0_in_353_353,
    &UART2_usart_irq_0_0_to_C66SS1_INTROUTER0_in_353_353,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART2 = {
    .id = TISCI_DEV_UART2,
    .n_if   = 3,
    .p_if   = &tisci_if_UART2[0],
};

/* Start of UART3 interface definition */
const struct Sciclient_rmIrqIf UART3_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_99_99 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 99,
};
const struct Sciclient_rmIrqIf UART3_usart_irq_0_0_to_C66SS0_INTROUTER0_in_51_51 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 51,
};
const struct Sciclient_rmIrqIf UART3_usart_irq_0_0_to_C66SS1_INTROUTER0_in_51_51 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 51,
};
const struct Sciclient_rmIrqIf UART3_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_188_188 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 188,
};
const struct Sciclient_rmIrqIf UART3_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_188_188 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 188,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART3[] = {
    &UART3_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_99_99,
    &UART3_usart_irq_0_0_to_C66SS0_INTROUTER0_in_51_51,
    &UART3_usart_irq_0_0_to_C66SS1_INTROUTER0_in_51_51,
    &UART3_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_188_188,
    &UART3_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_188_188,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART3 = {
    .id = TISCI_DEV_UART3,
    .n_if   = 5,
    .p_if   = &tisci_if_UART3[0],
};

/* Start of UART4 interface definition */
const struct Sciclient_rmIrqIf UART4_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_100_100 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 100,
};
const struct Sciclient_rmIrqIf UART4_usart_irq_0_0_to_C66SS0_INTROUTER0_in_52_52 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 52,
};
const struct Sciclient_rmIrqIf UART4_usart_irq_0_0_to_C66SS1_INTROUTER0_in_52_52 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 52,
};
const struct Sciclient_rmIrqIf UART4_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_189_189 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 189,
};
const struct Sciclient_rmIrqIf UART4_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_189_189 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 189,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART4[] = {
    &UART4_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_100_100,
    &UART4_usart_irq_0_0_to_C66SS0_INTROUTER0_in_52_52,
    &UART4_usart_irq_0_0_to_C66SS1_INTROUTER0_in_52_52,
    &UART4_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_189_189,
    &UART4_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_189_189,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART4 = {
    .id = TISCI_DEV_UART4,
    .n_if   = 5,
    .p_if   = &tisci_if_UART4[0],
};

/* Start of UART5 interface definition */
const struct Sciclient_rmIrqIf UART5_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_101_101 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 101,
};
const struct Sciclient_rmIrqIf UART5_usart_irq_0_0_to_C66SS0_INTROUTER0_in_53_53 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 53,
};
const struct Sciclient_rmIrqIf UART5_usart_irq_0_0_to_C66SS1_INTROUTER0_in_53_53 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 53,
};
const struct Sciclient_rmIrqIf UART5_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_190_190 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 190,
};
const struct Sciclient_rmIrqIf UART5_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_190_190 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 190,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART5[] = {
    &UART5_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_101_101,
    &UART5_usart_irq_0_0_to_C66SS0_INTROUTER0_in_53_53,
    &UART5_usart_irq_0_0_to_C66SS1_INTROUTER0_in_53_53,
    &UART5_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_190_190,
    &UART5_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_190_190,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART5 = {
    .id = TISCI_DEV_UART5,
    .n_if   = 5,
    .p_if   = &tisci_if_UART5[0],
};

/* Start of UART6 interface definition */
const struct Sciclient_rmIrqIf UART6_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_102_102 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 102,
};
const struct Sciclient_rmIrqIf UART6_usart_irq_0_0_to_C66SS0_INTROUTER0_in_54_54 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 54,
};
const struct Sciclient_rmIrqIf UART6_usart_irq_0_0_to_C66SS1_INTROUTER0_in_54_54 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 54,
};
const struct Sciclient_rmIrqIf UART6_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_191_191 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 191,
};
const struct Sciclient_rmIrqIf UART6_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_191_191 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 191,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART6[] = {
    &UART6_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_102_102,
    &UART6_usart_irq_0_0_to_C66SS0_INTROUTER0_in_54_54,
    &UART6_usart_irq_0_0_to_C66SS1_INTROUTER0_in_54_54,
    &UART6_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_191_191,
    &UART6_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_191_191,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART6 = {
    .id = TISCI_DEV_UART6,
    .n_if   = 5,
    .p_if   = &tisci_if_UART6[0],
};

/* Start of UART7 interface definition */
const struct Sciclient_rmIrqIf UART7_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_103_103 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 103,
};
const struct Sciclient_rmIrqIf UART7_usart_irq_0_0_to_C66SS0_INTROUTER0_in_55_55 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 55,
};
const struct Sciclient_rmIrqIf UART7_usart_irq_0_0_to_C66SS1_INTROUTER0_in_55_55 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 55,
};
const struct Sciclient_rmIrqIf UART7_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_192_192 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 192,
};
const struct Sciclient_rmIrqIf UART7_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_192_192 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 192,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART7[] = {
    &UART7_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_103_103,
    &UART7_usart_irq_0_0_to_C66SS0_INTROUTER0_in_55_55,
    &UART7_usart_irq_0_0_to_C66SS1_INTROUTER0_in_55_55,
    &UART7_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_192_192,
    &UART7_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_192_192,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART7 = {
    .id = TISCI_DEV_UART7,
    .n_if   = 5,
    .p_if   = &tisci_if_UART7[0],
};

/* Start of UART8 interface definition */
const struct Sciclient_rmIrqIf UART8_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_104_104 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 104,
};
const struct Sciclient_rmIrqIf UART8_usart_irq_0_0_to_C66SS0_INTROUTER0_in_56_56 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 56,
};
const struct Sciclient_rmIrqIf UART8_usart_irq_0_0_to_C66SS1_INTROUTER0_in_56_56 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 56,
};
const struct Sciclient_rmIrqIf UART8_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_193_193 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 193,
};
const struct Sciclient_rmIrqIf UART8_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_193_193 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 193,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART8[] = {
    &UART8_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_104_104,
    &UART8_usart_irq_0_0_to_C66SS0_INTROUTER0_in_56_56,
    &UART8_usart_irq_0_0_to_C66SS1_INTROUTER0_in_56_56,
    &UART8_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_193_193,
    &UART8_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_193_193,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART8 = {
    .id = TISCI_DEV_UART8,
    .n_if   = 5,
    .p_if   = &tisci_if_UART8[0],
};

/* Start of UART9 interface definition */
const struct Sciclient_rmIrqIf UART9_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_105_105 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 105,
};
const struct Sciclient_rmIrqIf UART9_usart_irq_0_0_to_C66SS0_INTROUTER0_in_57_57 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 57,
};
const struct Sciclient_rmIrqIf UART9_usart_irq_0_0_to_C66SS1_INTROUTER0_in_57_57 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 57,
};
const struct Sciclient_rmIrqIf UART9_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_194_194 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 194,
};
const struct Sciclient_rmIrqIf UART9_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_194_194 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 194,
};
const struct Sciclient_rmIrqIf *const tisci_if_UART9[] = {
    &UART9_usart_irq_0_0_to_MAIN2MCU_LVL_INTRTR0_in_105_105,
    &UART9_usart_irq_0_0_to_C66SS0_INTROUTER0_in_57_57,
    &UART9_usart_irq_0_0_to_C66SS1_INTROUTER0_in_57_57,
    &UART9_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_194_194,
    &UART9_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_194_194,
};
static const struct Sciclient_rmIrqNode tisci_irq_UART9 = {
    .id = TISCI_DEV_UART9,
    .n_if   = 5,
    .p_if   = &tisci_if_UART9[0],
};

/* Start of WKUP_UART0 interface definition */
const struct Sciclient_rmIrqIf WKUP_UART0_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_159_159 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 159,
};
const struct Sciclient_rmIrqIf WKUP_UART0_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_159_159 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 159,
};
const struct Sciclient_rmIrqIf *const tisci_if_WKUP_UART0[] = {
    &WKUP_UART0_usart_irq_0_0_to_R5FSS0_INTROUTER0_in_159_159,
    &WKUP_UART0_usart_irq_0_0_to_R5FSS1_INTROUTER0_in_159_159,
};
static const struct Sciclient_rmIrqNode tisci_irq_WKUP_UART0 = {
    .id = TISCI_DEV_WKUP_UART0,
    .n_if   = 2,
    .p_if   = &tisci_if_WKUP_UART0[0],
};

/* Start of USB0 interface definition */
const struct Sciclient_rmIrqIf USB0_irq_1_8_to_MAIN2MCU_LVL_INTRTR0_in_128_135 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 128,
};
const struct Sciclient_rmIrqIf USB0_otgirq_9_9_to_MAIN2MCU_LVL_INTRTR0_in_152_152 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 152,
};
const struct Sciclient_rmIrqIf USB0_host_system_error_0_0_to_MAIN2MCU_LVL_INTRTR0_in_157_157 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 157,
};
const struct Sciclient_rmIrqIf USB0_irq_1_8_to_C66SS0_INTROUTER0_in_176_183 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 176,
};
const struct Sciclient_rmIrqIf USB0_otgirq_9_9_to_C66SS0_INTROUTER0_in_200_200 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 200,
};
const struct Sciclient_rmIrqIf USB0_host_system_error_0_0_to_C66SS0_INTROUTER0_in_277_277 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 277,
};
const struct Sciclient_rmIrqIf USB0_irq_1_8_to_C66SS1_INTROUTER0_in_176_183 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 176,
};
const struct Sciclient_rmIrqIf USB0_otgirq_9_9_to_C66SS1_INTROUTER0_in_200_200 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 200,
};
const struct Sciclient_rmIrqIf USB0_host_system_error_0_0_to_C66SS1_INTROUTER0_in_277_277 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 277,
};
const struct Sciclient_rmIrqIf USB0_otgirq_9_9_to_R5FSS0_INTROUTER0_in_231_231 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 231,
};
const struct Sciclient_rmIrqIf USB0_irq_1_8_to_R5FSS0_INTROUTER0_in_232_239 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 232,
};
const struct Sciclient_rmIrqIf USB0_host_system_error_0_0_to_R5FSS0_INTROUTER0_in_323_323 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 323,
};
const struct Sciclient_rmIrqIf USB0_otgirq_9_9_to_R5FSS1_INTROUTER0_in_231_231 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 231,
};
const struct Sciclient_rmIrqIf USB0_irq_1_8_to_R5FSS1_INTROUTER0_in_232_239 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 232,
};
const struct Sciclient_rmIrqIf USB0_host_system_error_0_0_to_R5FSS1_INTROUTER0_in_323_323 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 323,
};
const struct Sciclient_rmIrqIf *const tisci_if_USB0[] = {
    &USB0_irq_1_8_to_MAIN2MCU_LVL_INTRTR0_in_128_135,
    &USB0_otgirq_9_9_to_MAIN2MCU_LVL_INTRTR0_in_152_152,
    &USB0_host_system_error_0_0_to_MAIN2MCU_LVL_INTRTR0_in_157_157,
    &USB0_irq_1_8_to_C66SS0_INTROUTER0_in_176_183,
    &USB0_otgirq_9_9_to_C66SS0_INTROUTER0_in_200_200,
    &USB0_host_system_error_0_0_to_C66SS0_INTROUTER0_in_277_277,
    &USB0_irq_1_8_to_C66SS1_INTROUTER0_in_176_183,
    &USB0_otgirq_9_9_to_C66SS1_INTROUTER0_in_200_200,
    &USB0_host_system_error_0_0_to_C66SS1_INTROUTER0_in_277_277,
    &USB0_otgirq_9_9_to_R5FSS0_INTROUTER0_in_231_231,
    &USB0_irq_1_8_to_R5FSS0_INTROUTER0_in_232_239,
    &USB0_host_system_error_0_0_to_R5FSS0_INTROUTER0_in_323_323,
    &USB0_otgirq_9_9_to_R5FSS1_INTROUTER0_in_231_231,
    &USB0_irq_1_8_to_R5FSS1_INTROUTER0_in_232_239,
    &USB0_host_system_error_0_0_to_R5FSS1_INTROUTER0_in_323_323,
};
static const struct Sciclient_rmIrqNode tisci_irq_USB0 = {
    .id = TISCI_DEV_USB0,
    .n_if   = 15,
    .p_if   = &tisci_if_USB0[0],
};

/* Start of USB1 interface definition */
const struct Sciclient_rmIrqIf USB1_irq_1_8_to_MAIN2MCU_LVL_INTRTR0_in_136_143 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 136,
};
const struct Sciclient_rmIrqIf USB1_otgirq_9_9_to_MAIN2MCU_LVL_INTRTR0_in_153_153 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 153,
};
const struct Sciclient_rmIrqIf USB1_host_system_error_0_0_to_MAIN2MCU_LVL_INTRTR0_in_158_158 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 158,
};
const struct Sciclient_rmIrqIf USB1_irq_1_8_to_C66SS0_INTROUTER0_in_184_191 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 184,
};
const struct Sciclient_rmIrqIf USB1_otgirq_9_9_to_C66SS0_INTROUTER0_in_201_201 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 201,
};
const struct Sciclient_rmIrqIf USB1_host_system_error_0_0_to_C66SS0_INTROUTER0_in_278_278 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS0_INTROUTER0,
    .rbase  = 278,
};
const struct Sciclient_rmIrqIf USB1_irq_1_8_to_C66SS1_INTROUTER0_in_184_191 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 184,
};
const struct Sciclient_rmIrqIf USB1_otgirq_9_9_to_C66SS1_INTROUTER0_in_201_201 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 201,
};
const struct Sciclient_rmIrqIf USB1_host_system_error_0_0_to_C66SS1_INTROUTER0_in_278_278 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_C66SS1_INTROUTER0,
    .rbase  = 278,
};
const struct Sciclient_rmIrqIf USB1_otgirq_9_9_to_R5FSS0_INTROUTER0_in_1_1 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf USB1_irq_1_8_to_R5FSS0_INTROUTER0_in_2_9 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf USB1_host_system_error_0_0_to_R5FSS0_INTROUTER0_in_65_65 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 65,
};
const struct Sciclient_rmIrqIf USB1_otgirq_9_9_to_R5FSS1_INTROUTER0_in_1_1 = {
    .lbase  = 9,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 1,
};
const struct Sciclient_rmIrqIf USB1_irq_1_8_to_R5FSS1_INTROUTER0_in_2_9 = {
    .lbase  = 1,
    .len    = 8,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 2,
};
const struct Sciclient_rmIrqIf USB1_host_system_error_0_0_to_R5FSS1_INTROUTER0_in_65_65 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 65,
};
const struct Sciclient_rmIrqIf *const tisci_if_USB1[] = {
    &USB1_irq_1_8_to_MAIN2MCU_LVL_INTRTR0_in_136_143,
    &USB1_otgirq_9_9_to_MAIN2MCU_LVL_INTRTR0_in_153_153,
    &USB1_host_system_error_0_0_to_MAIN2MCU_LVL_INTRTR0_in_158_158,
    &USB1_irq_1_8_to_C66SS0_INTROUTER0_in_184_191,
    &USB1_otgirq_9_9_to_C66SS0_INTROUTER0_in_201_201,
    &USB1_host_system_error_0_0_to_C66SS0_INTROUTER0_in_278_278,
    &USB1_irq_1_8_to_C66SS1_INTROUTER0_in_184_191,
    &USB1_otgirq_9_9_to_C66SS1_INTROUTER0_in_201_201,
    &USB1_host_system_error_0_0_to_C66SS1_INTROUTER0_in_278_278,
    &USB1_otgirq_9_9_to_R5FSS0_INTROUTER0_in_1_1,
    &USB1_irq_1_8_to_R5FSS0_INTROUTER0_in_2_9,
    &USB1_host_system_error_0_0_to_R5FSS0_INTROUTER0_in_65_65,
    &USB1_otgirq_9_9_to_R5FSS1_INTROUTER0_in_1_1,
    &USB1_irq_1_8_to_R5FSS1_INTROUTER0_in_2_9,
    &USB1_host_system_error_0_0_to_R5FSS1_INTROUTER0_in_65_65,
};
static const struct Sciclient_rmIrqNode tisci_irq_USB1 = {
    .id = TISCI_DEV_USB1,
    .n_if   = 15,
    .p_if   = &tisci_if_USB1[0],
};

/* Start of VPFE0 interface definition */
const struct Sciclient_rmIrqIf VPFE0_ccdc_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_276_276 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 276,
};
const struct Sciclient_rmIrqIf VPFE0_rat_exp_intr_1_1_to_MAIN2MCU_LVL_INTRTR0_in_277_277 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
    .rbase  = 277,
};
const struct Sciclient_rmIrqIf VPFE0_ccdc_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_276_276 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 276,
};
const struct Sciclient_rmIrqIf VPFE0_rat_exp_intr_1_1_to_R5FSS0_INTROUTER0_in_277_277 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS0_INTROUTER0,
    .rbase  = 277,
};
const struct Sciclient_rmIrqIf VPFE0_ccdc_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_276_276 = {
    .lbase  = 0,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 276,
};
const struct Sciclient_rmIrqIf VPFE0_rat_exp_intr_1_1_to_R5FSS1_INTROUTER0_in_277_277 = {
    .lbase  = 1,
    .len    = 1,
    .rid    = TISCI_DEV_R5FSS1_INTROUTER0,
    .rbase  = 277,
};
const struct Sciclient_rmIrqIf *const tisci_if_VPFE0[] = {
    &VPFE0_ccdc_intr_pend_0_0_to_MAIN2MCU_LVL_INTRTR0_in_276_276,
    &VPFE0_rat_exp_intr_1_1_to_MAIN2MCU_LVL_INTRTR0_in_277_277,
    &VPFE0_ccdc_intr_pend_0_0_to_R5FSS0_INTROUTER0_in_276_276,
    &VPFE0_rat_exp_intr_1_1_to_R5FSS0_INTROUTER0_in_277_277,
    &VPFE0_ccdc_intr_pend_0_0_to_R5FSS1_INTROUTER0_in_276_276,
    &VPFE0_rat_exp_intr_1_1_to_R5FSS1_INTROUTER0_in_277_277,
};
static const struct Sciclient_rmIrqNode tisci_irq_VPFE0 = {
    .id = TISCI_DEV_VPFE0,
    .n_if   = 6,
    .p_if   = &tisci_if_VPFE0[0],
};


const struct Sciclient_rmIrqNode *const gRmIrqTree[] = {
    &tisci_irq_MCU_ADC0,
    &tisci_irq_MCU_ADC1,
    &tisci_irq_compute_cluster_j7es_tb_vdc_main_0,
    &tisci_irq_COMPUTE_CLUSTER0_CLEC,
    &tisci_irq_MCU_CPSW0,
    &tisci_irq_CPSW0,
    &tisci_irq_CSI_RX_IF0,
    &tisci_irq_CSI_RX_IF1,
    &tisci_irq_CSI_TX_IF0,
    &tisci_irq_DCC0,
    &tisci_irq_DCC1,
    &tisci_irq_DCC2,
    &tisci_irq_DCC3,
    &tisci_irq_DCC4,
    &tisci_irq_MCU_TIMER0,
    &tisci_irq_DCC5,
    &tisci_irq_DCC6,
    &tisci_irq_DCC7,
    &tisci_irq_DCC8,
    &tisci_irq_DCC9,
    &tisci_irq_DCC10,
    &tisci_irq_DCC11,
    &tisci_irq_DCC12,
    &tisci_irq_MCU_DCC0,
    &tisci_irq_MCU_DCC1,
    &tisci_irq_MCU_DCC2,
    &tisci_irq_DDR0,
    &tisci_irq_TIMER0,
    &tisci_irq_TIMER1,
    &tisci_irq_TIMER2,
    &tisci_irq_TIMER3,
    &tisci_irq_TIMER4,
    &tisci_irq_TIMER5,
    &tisci_irq_TIMER6,
    &tisci_irq_TIMER7,
    &tisci_irq_TIMER8,
    &tisci_irq_TIMER9,
    &tisci_irq_TIMER10,
    &tisci_irq_GTC0,
    &tisci_irq_TIMER11,
    &tisci_irq_TIMER12,
    &tisci_irq_TIMER13,
    &tisci_irq_TIMER14,
    &tisci_irq_TIMER15,
    &tisci_irq_TIMER16,
    &tisci_irq_TIMER17,
    &tisci_irq_TIMER18,
    &tisci_irq_TIMER19,
    &tisci_irq_MCU_TIMER1,
    &tisci_irq_MCU_TIMER2,
    &tisci_irq_MCU_TIMER3,
    &tisci_irq_MCU_TIMER4,
    &tisci_irq_MCU_TIMER5,
    &tisci_irq_MCU_TIMER6,
    &tisci_irq_MCU_TIMER7,
    &tisci_irq_MCU_TIMER8,
    &tisci_irq_MCU_TIMER9,
    &tisci_irq_ECAP0,
    &tisci_irq_ECAP1,
    &tisci_irq_ECAP2,
    &tisci_irq_EHRPWM0,
    &tisci_irq_EHRPWM1,
    &tisci_irq_EHRPWM2,
    &tisci_irq_EHRPWM3,
    &tisci_irq_EHRPWM4,
    &tisci_irq_EHRPWM5,
    &tisci_irq_ELM0,
    &tisci_irq_MMCSD0,
    &tisci_irq_MMCSD1,
    &tisci_irq_MMCSD2,
    &tisci_irq_EQEP0,
    &tisci_irq_EQEP1,
    &tisci_irq_EQEP2,
    &tisci_irq_ESM0,
    &tisci_irq_MCU_ESM0,
    &tisci_irq_WKUP_ESM0,
    &tisci_irq_MCU_FSS0_FSAS_0,
    &tisci_irq_MCU_FSS0_HYPERBUS1P0_0,
    &tisci_irq_MCU_FSS0_OSPI_0,
    &tisci_irq_MCU_FSS0_OSPI_1,
    &tisci_irq_GPIO0,
    &tisci_irq_GPIO1,
    &tisci_irq_GPIO2,
    &tisci_irq_GPIO3,
    &tisci_irq_GPIO4,
    &tisci_irq_GPIO5,
    &tisci_irq_GPIO6,
    &tisci_irq_GPIO7,
    &tisci_irq_WKUP_GPIO0,
    &tisci_irq_WKUP_GPIO1,
    &tisci_irq_GPMC0,
    &tisci_irq_I3C0,
    &tisci_irq_MCU_I3C0,
    &tisci_irq_MCU_I3C1,
    &tisci_irq_PRU_ICSSG0,
    &tisci_irq_PRU_ICSSG1,
    &tisci_irq_C66SS0_INTROUTER0,
    &tisci_irq_C66SS1_INTROUTER0,
    &tisci_irq_CMPEVENT_INTRTR0,
    &tisci_irq_MAIN2MCU_LVL_INTRTR0,
    &tisci_irq_MAIN2MCU_PLS_INTRTR0,
    &tisci_irq_GPIOMUX_INTRTR0,
    &tisci_irq_R5FSS0_INTROUTER0,
    &tisci_irq_R5FSS1_INTROUTER0,
    &tisci_irq_TIMESYNC_INTRTR0,
    &tisci_irq_WKUP_GPIOMUX_INTRTR0,
    &tisci_irq_AASRC0,
    &tisci_irq_DECODER0,
    &tisci_irq_UART0,
    &tisci_irq_MCU_UART0,
    &tisci_irq_DSS_DSI0,
    &tisci_irq_DSS_EDP0,
    &tisci_irq_DSS0,
    &tisci_irq_ENCODER0,
    &tisci_irq_WKUP_VTM0,
    &tisci_irq_MCAN0,
    &tisci_irq_MCAN1,
    &tisci_irq_MCAN2,
    &tisci_irq_MCAN3,
    &tisci_irq_MCAN4,
    &tisci_irq_MCAN5,
    &tisci_irq_MCAN6,
    &tisci_irq_MCAN7,
    &tisci_irq_MCAN8,
    &tisci_irq_MCAN9,
    &tisci_irq_MCAN10,
    &tisci_irq_MCAN11,
    &tisci_irq_MCAN12,
    &tisci_irq_MCAN13,
    &tisci_irq_MCU_MCAN0,
    &tisci_irq_MCU_MCAN1,
    &tisci_irq_MCASP0,
    &tisci_irq_MCASP1,
    &tisci_irq_MCASP2,
    &tisci_irq_MCASP3,
    &tisci_irq_MCASP4,
    &tisci_irq_MCASP5,
    &tisci_irq_MCASP6,
    &tisci_irq_MCASP7,
    &tisci_irq_MCASP8,
    &tisci_irq_MCASP9,
    &tisci_irq_MCASP10,
    &tisci_irq_MCASP11,
    &tisci_irq_MLB0,
    &tisci_irq_I2C0,
    &tisci_irq_I2C1,
    &tisci_irq_I2C2,
    &tisci_irq_I2C3,
    &tisci_irq_I2C4,
    &tisci_irq_I2C5,
    &tisci_irq_I2C6,
    &tisci_irq_MCU_I2C0,
    &tisci_irq_MCU_I2C1,
    &tisci_irq_WKUP_I2C0,
    &tisci_irq_navss512l_main_0,
    &tisci_irq_NAVSS0_CPTS_0,
    &tisci_irq_NAVSS0_MODSS_INTAGGR_0,
    &tisci_irq_NAVSS0_MODSS_INTAGGR_1,
    &tisci_irq_NAVSS0_UDMASS_INTAGGR_0,
    &tisci_irq_NAVSS0_INTR_ROUTER_0,
    &tisci_irq_NAVSS0_MAILBOX_0,
    &tisci_irq_NAVSS0_MAILBOX_1,
    &tisci_irq_NAVSS0_MAILBOX_2,
    &tisci_irq_NAVSS0_MAILBOX_3,
    &tisci_irq_NAVSS0_MAILBOX_4,
    &tisci_irq_NAVSS0_MAILBOX_5,
    &tisci_irq_NAVSS0_MAILBOX_6,
    &tisci_irq_NAVSS0_MAILBOX_7,
    &tisci_irq_NAVSS0_MAILBOX_8,
    &tisci_irq_NAVSS0_MAILBOX_9,
    &tisci_irq_NAVSS0_MAILBOX_10,
    &tisci_irq_NAVSS0_MAILBOX_11,
    &tisci_irq_NAVSS0_MCRC_0,
    &tisci_irq_NAVSS0_TBU_0,
    &tisci_irq_NAVSS0_TCU_0,
    &tisci_irq_MCU_NAVSS0_INTAGGR_0,
    &tisci_irq_MCU_NAVSS0_INTR_ROUTER_0,
    &tisci_irq_MCU_NAVSS0_MCRC_0,
    &tisci_irq_PCIE0,
    &tisci_irq_PCIE1,
    &tisci_irq_PCIE2,
    &tisci_irq_PCIE3,
    &tisci_irq_RTI24,
    &tisci_irq_RTI25,
    &tisci_irq_SA2_UL0,
    &tisci_irq_MCU_SA2_UL0,
    &tisci_irq_MCSPI0,
    &tisci_irq_MCSPI1,
    &tisci_irq_MCSPI2,
    &tisci_irq_MCSPI3,
    &tisci_irq_MCSPI4,
    &tisci_irq_MCSPI5,
    &tisci_irq_MCSPI6,
    &tisci_irq_MCSPI7,
    &tisci_irq_MCU_MCSPI0,
    &tisci_irq_MCU_MCSPI1,
    &tisci_irq_MCU_MCSPI2,
    &tisci_irq_UFS0,
    &tisci_irq_UART1,
    &tisci_irq_UART2,
    &tisci_irq_UART3,
    &tisci_irq_UART4,
    &tisci_irq_UART5,
    &tisci_irq_UART6,
    &tisci_irq_UART7,
    &tisci_irq_UART8,
    &tisci_irq_UART9,
    &tisci_irq_WKUP_UART0,
    &tisci_irq_USB0,
    &tisci_irq_USB1,
    &tisci_irq_VPFE0,
};
#endif

const uint32_t gRmIrqTreeCount = sizeof(gRmIrqTree)/sizeof(gRmIrqTree[0]);
