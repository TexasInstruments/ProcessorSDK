/********************************************************************
*
* C7X256V1_CLEC INTERRUPT MAP. header file
*
* Copyright (C) 2015-2024 Texas Instruments Incorporated.
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
#ifndef CSLR_C7X256V1_CLEC_INTERRUPT_MAP_H_
#define CSLR_C7X256V1_CLEC_INTERRUPT_MAP_H_

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* List of intr sources for receiver: C7X256V1_CLEC
*/

#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_RTI5_INTR_WWD_0                                           (4U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_CTRL_MMR0_IPC_SET1_IPC_SET_IPCFG_0                        (5U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_MAILBOX0_MAILBOX_CLUSTER_1_MAILBOX_CLUSTER_PEND_3         (6U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_MAILBOX0_MAILBOX_CLUSTER_2_MAILBOX_CLUSTER_PEND_3         (7U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_MAILBOX0_MAILBOX_CLUSTER_3_MAILBOX_CLUSTER_PEND_3         (8U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_MAILBOX0_MAILBOX_CLUSTER_4_MAILBOX_CLUSTER_PEND_3         (9U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_MAILBOX0_MAILBOX_CLUSTER_7_MAILBOX_CLUSTER_PEND_2         (10U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_20                     (12U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_21                     (13U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_22                     (14U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_23                     (15U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_100                   (16U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_101                   (17U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_102                   (18U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_103                   (19U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_104                   (20U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_105                   (21U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_106                   (22U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_107                   (23U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_108                   (24U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_109                   (25U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_110                   (26U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_111                   (27U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_112                   (28U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_113                   (29U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_114                   (30U)
#define CSLR_C7X256V1_CLEC_SOC_EVENTS_IN_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_115                   (31U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_0                                  (32U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_1                                  (33U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_2                                  (34U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_3                                  (35U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_4                                  (36U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_5                                  (37U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_6                                  (38U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_7                                  (39U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_8                                  (40U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_9                                  (41U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_10                                 (42U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_11                                 (43U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_12                                 (44U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_13                                 (45U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_14                                 (46U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MAIN_GPIOMUX_INTROUTER0_OUTP_15                                 (47U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_CPSW0_CPTS_COMP_0                                               (48U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC0_INFIFO_LEVEL_0                                           (50U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC0_INGROUP_LEVEL_0                                          (51U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC0_OUTFIFO_LEVEL_0                                          (52U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC0_OUTGROUP_LEVEL_0                                         (53U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC0_ERR_LEVEL_0                                              (54U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC1_INFIFO_LEVEL_0                                           (55U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC1_INGROUP_LEVEL_0                                          (56U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC1_OUTFIFO_LEVEL_0                                          (57U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC1_OUTGROUP_LEVEL_0                                         (58U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_AASRC1_ERR_LEVEL_0                                              (59U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MLB0_MLBSS_MLB_INT_0                                            (60U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MLB0_MLBSS_MLB_AHB_INT_0                                        (61U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MLB0_MLBSS_MLB_AHB_INT_1                                        (62U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER8_INTR_PEND_0                                              (64U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER9_INTR_PEND_0                                              (65U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER10_INTR_PEND_0                                             (66U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER11_INTR_PEND_0                                             (67U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER12_INTR_PEND_0                                             (68U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER13_INTR_PEND_0                                             (69U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER14_INTR_PEND_0                                             (70U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER15_INTR_PEND_0                                             (71U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_32                          (96U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_33                          (97U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_34                          (98U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_35                          (99U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_36                          (100U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_37                          (101U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_38                          (102U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_39                          (103U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCU_MCU_GPIOMUX_INTROUTER0_OUTP_0                               (104U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCU_MCU_GPIOMUX_INTROUTER0_OUTP_1                               (105U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCU_MCU_GPIOMUX_INTROUTER0_OUTP_2                               (106U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCU_MCU_GPIOMUX_INTROUTER0_OUTP_3                               (107U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SA3_SS0_INTAGGR_0_INTAGGR_VINTR_4                               (112U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SA3_SS0_INTAGGR_0_INTAGGR_VINTR_5                               (113U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_GLUELOGIC_MAIN_DCC_DONE_GLUE_DCC_DONE_0                         (128U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_GLUELOGIC_SOC_ACCESS_ERR_INTR_GLUE_OUT_0                        (129U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_EFUSE0_EFC_ERROR_0                                              (131U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_WKUP_RTCSS0_RTC_EVENT_PEND_0                                    (132U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_GLUELOGIC_SOC_CBASS_ERR_INTR_GLUE_MAIN_CBASS_AGG_ERR_INTR_0     (133U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_CPSW0_EVNT_PEND_0                                               (134U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_CPSW0_MDIO_PEND_0                                               (135U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_CPSW0_STAT_PEND_0                                               (136U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SMS0_TIFS_CBASS_0_FW_EXCEPTION_INTR_0                           (143U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SMS0_COMMON_0_COMBINED_SEC_IN_0                                 (144U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ECAP0_ECAP_INT_0                                                (145U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ECAP1_ECAP_INT_0                                                (146U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ECAP2_ECAP_INT_0                                                (147U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ECAP3_ECAP_INT_0                                                (148U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ECAP4_ECAP_INT_0                                                (149U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ECAP5_ECAP_INT_0                                                (150U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ADC0_GEN_LEVEL_0                                                (151U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER0_INTR_PEND_0                                              (152U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER1_INTR_PEND_0                                              (153U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER2_INTR_PEND_0                                              (154U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER3_INTR_PEND_0                                              (155U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER4_INTR_PEND_0                                              (156U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER5_INTR_PEND_0                                              (157U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER6_INTR_PEND_0                                              (158U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_TIMER7_INTR_PEND_0                                              (159U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SA3_SS0_SA_UL_0_SA_UL_PKA_0                                     (160U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SA3_SS0_SA_UL_0_SA_UL_TRNG_0                                    (161U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MMCSD0_EMMCSDSS_INTR_0                                          (165U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCRC64_0_INT_MCRC_0                                             (166U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS1_FSAS_0_ECC_INTR_ERR_PEND_0                                 (169U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS0_FSAS_ECC_INTR_ERR_PEND_0                                   (170U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS0_OSPI0_LVL_INTR_0                                           (171U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS0_FSAS_FOTA_STAT_INTR_PEND_0                                 (172U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS0_FSAS_FOTA_STAT_ERR_PEND_0                                  (173U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS0_OTFA_INTR_ERR_PEND_0                                       (174U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS1_FSAS_0_OTFA_INTR_ERR_PEND_0                                (175U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SMS0_AESEIP38T_0_AES_SINTREQUEST_P_0                            (176U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DDPA0_DDPA_INTR_0                                               (177U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS1_OSPI_0_OSPI_LVL_INTR_0                                     (178U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_FSS1_HYPERBUS1P0_0_HPB_INTR_0                                   (179U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ESM0_ESM_INT_CFG_LVL_0                                          (180U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ESM0_ESM_INT_HI_LVL_0                                           (181U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_ESM0_ESM_INT_LOW_LVL_0                                          (182U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0                               (183U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_WKUP_VTM0_THERM_LVL_GT_TH2_INTR_0                               (184U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0                               (185U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                          (186U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN0_MCANSS_MCAN_LVL_INT_0                                     (187U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN0_MCANSS_MCAN_LVL_INT_1                                     (188U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN2_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                          (189U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN3_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                          (190U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN4_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                          (191U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_I2C0_POINTRPEND_0                                               (193U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_I2C1_POINTRPEND_0                                               (194U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_I2C2_POINTRPEND_0                                               (195U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_I2C3_POINTRPEND_0                                               (196U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_WKUP_I2C0_POINTRPEND_0                                          (197U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_SMS0_AESEIP38T_0_AES_SINTREQUEST_S_0                            (198U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN1_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                          (199U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DEBUGSS0_AQCMPINTR_LEVEL_0                                      (201U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_DEBUGSS0_CTM_LEVEL_0                                            (202U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_PSCSS0_PSC_ALLINT_0                                             (203U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCSPI0_INTR_SPI_0                                               (204U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCSPI1_INTR_SPI_0                                               (205U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCSPI2_INTR_SPI_0                                               (206U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCSPI3_INTR_SPI_0                                               (207U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCSPI4_INTR_SPI_0                                               (208U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_UART0_USART_IRQ_0                                               (210U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_UART1_USART_IRQ_0                                               (211U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_UART2_USART_IRQ_0                                               (212U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_UART3_USART_IRQ_0                                               (213U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_UART4_USART_IRQ_0                                               (214U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_UART5_USART_IRQ_0                                               (215U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_UART6_USART_IRQ_0                                               (216U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_WKUP_UART0_USART_IRQ_0                                          (218U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_0                                                      (220U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_1                                                      (221U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_2                                                      (222U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_3                                                      (223U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_4                                                      (224U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_5                                                      (225U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_6                                                      (226U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_IRQ_7                                                      (227U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_USB0_MISC_LEVEL_0                                               (228U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_EPWM0_EPWM_ETINT_0                                              (229U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_EPWM0_EPWM_TRIPZINT_0                                           (230U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_EPWM1_EPWM_ETINT_0                                              (231U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_EPWM1_EPWM_TRIPZINT_0                                           (233U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_EPWM2_EPWM_ETINT_0                                              (234U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_EPWM2_EPWM_TRIPZINT_0                                           (235U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN1_MCANSS_MCAN_LVL_INT_0                                     (245U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN1_MCANSS_MCAN_LVL_INT_1                                     (246U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN2_MCANSS_MCAN_LVL_INT_0                                     (247U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN2_MCANSS_MCAN_LVL_INT_1                                     (248U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN3_MCANSS_MCAN_LVL_INT_0                                     (249U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN3_MCANSS_MCAN_LVL_INT_1                                     (250U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN4_MCANSS_MCAN_LVL_INT_0                                     (251U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCAN4_MCANSS_MCAN_LVL_INT_1                                     (252U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_GLUELOGIC_EXT_INTN_GLUE_OUT_0                                   (256U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_RL2_OF_CBA4_0_ERR_LVL_0                                         (260U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_RL2_OF_CBA4_1_ERR_LVL_0                                         (261U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_RL2_OF_CBA4_2_ERR_LVL_0                                         (262U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_RL2_OF_CBA4_3_ERR_LVL_0                                         (263U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP0_REC_INTR_PEND_0                                          (267U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP0_XMIT_INTR_PEND_0                                         (268U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP1_REC_INTR_PEND_0                                          (269U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP1_XMIT_INTR_PEND_0                                         (270U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP2_REC_INTR_PEND_0                                          (271U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP2_XMIT_INTR_PEND_0                                         (272U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP3_REC_INTR_PEND_0                                          (273U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP3_XMIT_INTR_PEND_0                                         (274U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP4_REC_INTR_PEND_0                                          (275U)
#define CSLR_C7X256V1_CLEC_GIC_SPI_MCASP4_XMIT_INTR_PEND_0                                         (276U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_C7X256V1_CLEC_INTERRUPT_MAP_H_ */

