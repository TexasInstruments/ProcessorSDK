/*
 *  Copyright (c) Texas Instruments Incorporated 2021-2022
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
 *  \file udma_soc.h
 *
 *  \brief UDMA Low Level Driver J721S2 SOC specific file.
 */

#ifndef UDMA_SOC_H_
#define UDMA_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* Macro to find maximum of given values */
#define UDMA_MAX(X,Y)  ((X>Y) ? X:Y)
/**
 *  \anchor Udma_InstanceIdSoc
 *  \name UDMA Instance ID specific to SOC
 *
 *  UDMA instance ID - Main/MCU NAVSS
 *
 *  @{
 */
/** \brief Main NAVSS UDMA instance */
#define UDMA_INST_ID_MAIN_0             (UDMA_INST_ID_0)
/** \brief MCU NAVSS UDMA instance */
#define UDMA_INST_ID_MCU_0              (UDMA_INST_ID_1)
/** \brief BCDMA instance */
#define UDMA_INST_ID_BCDMA_0            (UDMA_INST_ID_2)
/** \brief Start of UDMAP instance */
#define UDMA_INST_ID_UDMAP_START        (UDMA_INST_ID_0)
/** \brief Maximum number of UDMAP instance */
#define UDMA_INST_ID_UDMAP_MAX          (UDMA_INST_ID_1)
/** \brief Total number of UDMAP instances */
#define UDMA_NUM_UDMAP_INST_ID          (UDMA_INST_ID_UDMAP_MAX - UDMA_INST_ID_UDMAP_START + 1U)
/** \brief Start of BCDMA instance */
#define UDMA_INST_ID_BCDMA_START        (UDMA_INST_ID_2)
/** \brief Maximum number of BCDMA instance */
#define UDMA_INST_ID_BCDMA_MAX          (UDMA_INST_ID_2)
/** \brief Total number of BCDMA instances */
#define UDMA_NUM_BCDMA_INST_ID          (UDMA_INST_ID_BCDMA_MAX - UDMA_INST_ID_BCDMA_START + 1U)
/** \brief Total number of PKTDMA instances */
#define UDMA_NUM_PKTDMA_INST_ID          (0U)
/** \brief Start of UDMA instance */
#define UDMA_INST_ID_START              (UDMA_INST_ID_0)
/** \brief Maximum number of UDMA instance */
#define UDMA_INST_ID_MAX                (UDMA_INST_ID_2)
/** \brief Total number of UDMA instances */
#define UDMA_NUM_INST_ID                (UDMA_NUM_UDMAP_INST_ID + UDMA_NUM_BCDMA_INST_ID + UDMA_NUM_PKTDMA_INST_ID)
/* @} */
 
/**
 *  \anchor Udma_SocCfg
 *  \name UDMA SOC Configuration
 *
 *  UDMA Soc Cfg - Flags to indicate the presnce of various SOC specific modules.
 *
 *  @{
 */
/** \brief Flag to indicate UDMAP module is present or not in the SOC*/
#define UDMA_SOC_CFG_UDMAP_PRESENT               (1U)

/** \brief Flag to indicate BCDMA module is present or not in the SOC*/
#define UDMA_SOC_CFG_BCDMA_PRESENT               (1U) 

/** \brief Flag to indicate PKTDMA module is present or not in the SOC*/
#define UDMA_SOC_CFG_PKTDMA_PRESENT              (0U) 
	
/** \brief Flag to indicate Proxy is present or not in the SOC*/
#define UDMA_SOC_CFG_PROXY_PRESENT               (1U) 
	
/** \brief Flag to indicate Interrupt Router is present or not in the SOC*/
#define UDMA_SOC_CFG_INTR_ROUTER_PRESENT         (1U) 

/** \brief Flag to indicate Clec is present or not in the SOC*/
#if defined (BUILD_C7X)
#define UDMA_SOC_CFG_CLEC_PRESENT                (1U) 
#else
#define UDMA_SOC_CFG_CLEC_PRESENT                (0U)
#endif

/** \brief Flag to indicate Normal RA is present or not in the SOC*/
#define UDMA_SOC_CFG_RA_NORMAL_PRESENT           (1U) 

/** \brief Flag to indicate LCDMA RA is present or not in the SOC*/
#define UDMA_SOC_CFG_RA_LCDMA_PRESENT            (1U)

/** \brief Flag to indicate Ring Monitor is present or not in the SOC*/
#define UDMA_SOC_CFG_RING_MON_PRESENT            (1U)

/** \brief Flag to indicate VPAC1 is present or not in the SOC*/
#define UDMA_SOC_CFG_VPAC1_PRESENT               (0U)

/** \brief Flag to indicate the SOC needs ring reset workaround */
#define UDMA_SOC_CFG_APPLY_RING_WORKAROUND       (0U)

/* Flag to indicate DRU local to C7X cores is present or not in the SoC */
#define UDMA_LOCAL_C7X_DRU_PRESENT               (0U)
/* @} */

 /**
 *  \anchor Udma_TxChFdepth
 *  \name UDMA Tx Channels FDEPTH
 *
 *  UDMA Tx Ch Fdepth - Fdepth of various types of channels present in the SOC.
 *
 *  @{
 */
/** \brief Tx Ultra High Capacity Channel FDEPTH*/
#define UDMA_TX_UHC_CHANS_FDEPTH         (CSL_NAVSS_UDMAP_TX_UHC_CHANS_FDEPTH)
/** \brief Tx High Capacity Channel FDEPTH*/
#define UDMA_TX_HC_CHANS_FDEPTH 		 (CSL_NAVSS_UDMAP_TX_HC_CHANS_FDEPTH)
/** \brief Tx Normal Channel FDEPTH*/
#define UDMA_TX_CHANS_FDEPTH 			 (CSL_NAVSS_UDMAP_TX_CHANS_FDEPTH)
/* @} */

/**
 *  \anchor Udma_RingAccAselEndpointSoc
 *  \name UDMA Ringacc address select (asel) endpoint
 *
 *  List of all valid address select (asel) endpoints in the SOC.
 *
 *  @{
 */
/** \brief Physical address (normal) */
#define UDMA_RINGACC_ASEL_ENDPOINT_PHYSADDR          (0U)
/* @} */

/** \brief Invalid Ring Mode*/
#define UDMA_RING_MODE_INVALID          (CSL_RINGACC_RING_MODE_INVALID)

/** \brief Number of Mapped TX Group */
#define UDMA_NUM_MAPPED_TX_GROUP     (0U)
/**
 *  \anchor Udma_MappedTxGrpSoc
 *  \name Mapped TX Group specific to a SOC
 *
 *  List of all mapped TX groups present in the SOC.
 *
 *  @{
 */
/* No mapped TX channels/rings in J721S2 */
/* @} */

/** \brief Number of Mapped RX Group */
#define UDMA_NUM_MAPPED_RX_GROUP     (0U)
/**
 *  \anchor Udma_MappedRxGrpSoc
 *  \name Mapped RX Group specific to a SOC
 *
 *  List of all mapped RX groups present in the SOC.
 *
 *  @{
 */
/* No mapped RX channels/rings in J721S2 */
/* @} */

/** \brief Number of UTC instance */
#define UDMA_NUM_UTC_INSTANCE           (CSL_NAVSS_UTC_CNT)

/**
 *  \anchor Udma_UtcIdSoc
 *  \name UTC ID specific to a SOC
 *
 *  List of all UTC's present in the SOC.
 *
 *  @{
 */
#define UDMA_UTC_ID_MSMC_DRU0           (UDMA_UTC_ID0)
#define UDMA_UTC_ID_VPAC_TC0            (UDMA_UTC_ID1)
#define UDMA_UTC_ID_VPAC_TC1            (UDMA_UTC_ID2)
#define UDMA_UTC_ID_DMPAC_TC0           (UDMA_UTC_ID3)
/* @} */
/** \brief External start channel of DRU0 UTC */
#define UDMA_UTC_START_CH_DRU0              (0U)
/** \brief Number of channels present in DRU0 UTC */
#define UDMA_UTC_NUM_CH_DRU0                (CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILS_THREAD_CNT)
/** \brief Start thread ID of DRU0 UTC */
#define UDMA_UTC_START_THREAD_ID_DRU0       (CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILD_THREAD_OFFSET)

/** \brief External start channel of VPAC TC0 UTC */
#define UDMA_UTC_START_CH_VPAC_TC0          (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILS_THREAD_OFFSET \
                                            - CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILS_THREAD_OFFSET)
/** \brief Number of channels present in VPAC TC0 UTC */
#define UDMA_UTC_NUM_CH_VPAC_TC0            (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILS_THREAD_CNT)
/** \brief Start thread ID of VPAC TC0 UTC */
#define UDMA_UTC_START_THREAD_ID_VPAC_TC0   (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILD_THREAD_OFFSET)

/** \brief External start channel of VPAC TC1 UTC */
#define UDMA_UTC_START_CH_VPAC_TC1          (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILS_THREAD_OFFSET \
                                            - CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILS_THREAD_OFFSET)
/** \brief Number of channels present in VPAC TC1 UTC */
#define UDMA_UTC_NUM_CH_VPAC_TC1            (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILS_THREAD_CNT)
/** \brief Start thread ID of VPAC TC1 UTC */
#define UDMA_UTC_START_THREAD_ID_VPAC_TC1   (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILD_THREAD_OFFSET)

/** \brief External start channel of DMPAC TC0 UTC */
#define UDMA_UTC_START_CH_DMPAC_TC0         (CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILS_THREAD_OFFSET \
                                            - CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILS_THREAD_OFFSET)
/** \brief Number of channels present in DMPAC TC0 UTC */
#define UDMA_UTC_NUM_CH_DMPAC_TC0           (CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILS_THREAD_CNT)
/** \brief Start thread ID of DMPAC TC0 UTC */
#define UDMA_UTC_START_THREAD_ID_DMPAC_TC0  (CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILD_THREAD_OFFSET)

/**
 *  \anchor Udma_CoreId
 *  \name Core ID specific to a SOC
 *
 *  List of all cores present in the SOC.
 *
 *  @{
 */
/*
 * Locally used core ID to define default RM configuration.
 * Not to be used by caller
 */
/* Main domain cores */
#define UDMA_CORE_ID_MPU1_0             (0U)
#define UDMA_CORE_ID_MCU2_0             (1U)
#define UDMA_CORE_ID_MCU2_1             (2U)
#define UDMA_CORE_ID_MCU3_0             (3U)
#define UDMA_CORE_ID_MCU3_1             (4U)
#define UDMA_CORE_ID_C7X_1              (5U)
#define UDMA_CORE_ID_C7X_2              (6U)
#define UDMA_NUM_MAIN_CORE              (7U)
/* MCU domain cores - Note: This should be after all main domain cores */
#define UDMA_CORE_ID_MCU1_0             (UDMA_NUM_MAIN_CORE + 0U)
#define UDMA_CORE_ID_MCU1_1             (UDMA_NUM_MAIN_CORE + 1U)
#define UDMA_NUM_MCU_CORE               (2U)
/* Total number of cores */
#define UDMA_NUM_CORE                   (UDMA_NUM_MAIN_CORE + UDMA_NUM_MCU_CORE)
/* @} */

/**
 *  \anchor Udma_DruSubmitCoreId
 *  \name DRU core ID register to use for direct TR submission.
 *   Each CPU should have a unique submit register to avoid corrupting
 *   submit word when SW is running from multiple CPU at the same time.
 *
 *   Note: Since only 3 submit register set is present, we need to share some
 *   of them across cores. This means that Direct TR from these cores can't
 *   run simultaneously.
 *   In this case C7x1 and C7x2 are provided unique ID which are more likely to
 *   use direct TR mode and other cores share the same core ID.
 *
 *  List of all DRU cores ID to use for all the CPUs present in the SOC.
 *
 *  @{
 */
#define UDMA_DRU_CORE_ID_MPU1_0         (CSL_DRU_CORE_ID_2)
#define UDMA_DRU_CORE_ID_MCU2_0         (CSL_DRU_CORE_ID_2)
#define UDMA_DRU_CORE_ID_MCU2_1         (CSL_DRU_CORE_ID_2)
#define UDMA_DRU_CORE_ID_MCU3_0         (CSL_DRU_CORE_ID_2)
#define UDMA_DRU_CORE_ID_MCU3_1         (CSL_DRU_CORE_ID_2)
#define UDMA_DRU_CORE_ID_C7X_1          (CSL_DRU_CORE_ID_0)
#define UDMA_DRU_CORE_ID_C7X_2          (CSL_DRU_CORE_ID_1)
#define UDMA_DRU_CORE_ID_MCU1_0         (CSL_DRU_CORE_ID_2)
#define UDMA_DRU_CORE_ID_MCU1_1         (CSL_DRU_CORE_ID_2)
/* @} */

/**
 *  \anchor Udma_RmResId
 *  \name UDMA Resources ID
 *
 *  List of all UDMA Resources Id's.
 *
 *  @{
 */

/** \brief Ultra High Capacity Block Copy Channels */
#define UDMA_RM_RES_ID_BC_UHC                   (0U)
/** \brief High Capacity Block Copy Channels */
#define UDMA_RM_RES_ID_BC_HC                    (1U) 
/** \brief Normal Capacity Block Copy Channels */
#define UDMA_RM_RES_ID_BC                       (2U)
/* List of all UDMAP and BCDMA TX/RX Channel Resource Id's */
/** \brief Ultra High Capacity TX and Block Copy Channels */
#define UDMA_RM_RES_ID_TX_UHC                   (3U)
/** \brief High Capacity TX and Block Copy Channels */
#define UDMA_RM_RES_ID_TX_HC                    (4U)
/** \brief Normal Capacity TX and Block Copy Channels */
#define UDMA_RM_RES_ID_TX                       (5U)
/** \brief Ultra High Capacity RX Channels */
#define UDMA_RM_RES_ID_RX_UHC                   (6U)
/** \brief High Capacity RX Channels */
#define UDMA_RM_RES_ID_RX_HC                    (7U)
/** \brief Normal Capacity RX Channels */
#define UDMA_RM_RES_ID_RX                       (8U)

/* List of all Resources Id's shared between all Instances */
/** \brief Global Event */
#define UDMA_RM_RES_ID_GLOBAL_EVENT             (9U)
/** \brief Virtual Interrupts */
#define UDMA_RM_RES_ID_VINTR                    (10U)
/** \brief Interrupt Router Interrupts */
#define UDMA_RM_RES_ID_IR_INTR                  (11U)

/** \brief UTC - Extended Channels (MSMC_DRU/VPAC_TC0/VPAC_TC1/DMPAC) */
#define UDMA_RM_RES_ID_UTC                      (12U)
/** \brief Free Flows */
#define UDMA_RM_RES_ID_RX_FLOW                  (13U)
/** \brief Free Rings */
#define UDMA_RM_RES_ID_RING                     (14U)
/** \brief Proxy */
#define UDMA_RM_RES_ID_PROXY                    (15U)
/** \brief Ring Monitors */
#define UDMA_RM_RES_ID_RING_MON                 (16U)

/** \brief Total number of BCDMA resources */
#define UDMA_RM_NUM_BCDMA_RES                   (12U)
/** \brief Total number of UDMAP resources */
#define UDMA_RM_NUM_UDMAP_RES                   (17U)  

/** \brief Total number of resources for which the range need to be queried from default BoardCfg */
#define UDMA_RM_DEFAULT_BOARDCFG_NUM_RES                         UDMA_MAX(UDMA_RM_NUM_UDMAP_RES, UDMA_RM_NUM_BCDMA_RES)

/* @} */

/** \brief Total number of shared resources - 
 *  Free_Flows/Global_Event/IR Intr/VINT */
#define UDMA_RM_NUM_SHARED_RES                  (4U) 
/** \brief Maximum no.of instances to split a shared resource. 
 *  This should be max(UDMA_NUM_CORE,UDMA_NUM_INST_ID) */
#define UDMA_RM_SHARED_RES_MAX_INST             (UDMA_NUM_CORE)

/* Start of C7x events associated to CLEC that UDMA Driver will manage */
/* Events  0 - 32  : left for other drivers
 * Events 16 - 47  : For routing DRU Local Events from CLEC (done by Vision Apps/TIDL)
 * Events 48 - 63  : managed by UDMA for routing various UDMA events to C7x  */ 
#define UDMA_C7X_CORE_INTR_OFFSET               (48U)
/* Number of C7x Events available for UDMA */
#define UDMA_C7X_CORE_NUM_INTR                  (64U - UDMA_C7X_CORE_INTR_OFFSET)

/**
 *  \anchor Udma_PsilCh
 *  \name PSIL Channels
 *
 *  List of all PSIL channels across MCU and main domains
 *
 *  @{
 */

/**
 *  \anchor Udma_PsilChMain
 *  \name Main PSIL Channels
 *
 *  List of all Main PSIL channels and the corresponding counts
 *
 *  @{
 */
#define UDMA_PSIL_CH_MAIN_SAUL0_TX          (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_VPAC_TC0_TX       (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_VPAC_TC1_TX       (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_DMPAC_TC0_TX      (CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_CSI_TX            (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_CPSW2_TX          (CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILD_THREAD_OFFSET)

#define UDMA_PSIL_CH_MAIN_SAUL0_RX          (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_VPAC_TC0_RX       (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_VPAC_TC1_RX       (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_DMPAC_TC0_RX      (CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_CSI_RX            (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_MAIN_CPSW2_RX          (CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILS_THREAD_OFFSET)

#define UDMA_PSIL_CH_MAIN_SAUL0_TX_CNT      (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_VPAC_TC0_TX_CNT   (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_VPAC_TC1_TX_CNT   (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_DMPAC_TC0_TX_CNT  (CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_CSI_TX_CNT        (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_CPSW2_TX_CNT      (CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILD_THREAD_CNT)

#define UDMA_PSIL_CH_MAIN_SAUL0_RX_CNT      (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_VPAC_TC0_RX_CNT   (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_VPAC_TC1_RX_CNT   (CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_DMPAC_TC0_RX_CNT  (CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_CSI_RX_CNT        (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_MAIN_CPSW2_RX_CNT      (CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILS_THREAD_CNT)
/* @} */

/**
 *  \anchor Udma_PsilChMcu
 *  \name Mcu PSIL Channels
 *
 *  List of all Mcu PSIL channels and the corresponding counts
 *
 *  @{
 */
#define UDMA_PSIL_CH_MCU_CPSW0_TX           (CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_MCU_SAUL0_TX           (CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILD_THREAD_OFFSET)

#define UDMA_PSIL_CH_MCU_CPSW0_RX           (CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_MCU_SAUL0_RX           (CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILS_THREAD_OFFSET)

#define UDMA_PSIL_CH_MCU_CPSW0_TX_CNT       (CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_MCU_SAUL0_TX_CNT       (CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILD_THREAD_CNT)

#define UDMA_PSIL_CH_MCU_CPSW0_RX_CNT       (CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_MCU_SAUL0_RX_CNT       (CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILS_THREAD_CNT)
/* @} */

/* @} */

/**
 *  \anchor Udma_PdmaCh
 *  \name PDMA Channels
 *
 *  List of all PDMA channels across MCU and main domains
 *
 *  @{
 */

/**
 *  \anchor Udma_PdmaChMainTx
 *  \name Main TX PDMA Channels
 *
 *  List of all Main PDMA TX channels
 *
 *  @{
 */
/*
 * PDMA Main McASP TX Channels
 */
#define UDMA_PDMA_CH_MAIN_MCASP0_TX     (CSL_PDMA_CH_MAIN_MCASP0_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCASP1_TX     (CSL_PDMA_CH_MAIN_MCASP1_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCASP2_TX     (CSL_PDMA_CH_MAIN_MCASP2_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCASP3_TX     (CSL_PDMA_CH_MAIN_MCASP3_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCASP4_TX     (CSL_PDMA_CH_MAIN_MCASP4_CH0_TX)
/*
 * PDMA Main UART TX Channels
 */
#define UDMA_PDMA_CH_MAIN_UART0_TX      (CSL_PDMA_CH_MAIN_UART0_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART1_TX      (CSL_PDMA_CH_MAIN_UART1_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART2_TX      (CSL_PDMA_CH_MAIN_UART2_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART3_TX      (CSL_PDMA_CH_MAIN_UART3_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART4_TX      (CSL_PDMA_CH_MAIN_UART4_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART5_TX      (CSL_PDMA_CH_MAIN_UART5_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART6_TX      (CSL_PDMA_CH_MAIN_UART6_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART7_TX      (CSL_PDMA_CH_MAIN_UART7_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART8_TX      (CSL_PDMA_CH_MAIN_UART8_CH0_TX)
#define UDMA_PDMA_CH_MAIN_UART9_TX      (CSL_PDMA_CH_MAIN_UART9_CH0_TX)
/*
 * PDMA Main McSPI TX Channels
 */
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI0_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI0_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI0_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI0_CH3_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI1_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI1_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI1_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI1_CH3_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI2_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI2_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI2_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI2_CH3_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI3_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI3_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI3_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI3_CH3_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI4_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI4_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI4_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI4_CH3_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI5_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI5_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI5_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI5_CH3_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI6_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI6_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI6_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI6_CH3_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH0_TX (CSL_PDMA_CH_MAIN_MCSPI7_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH1_TX (CSL_PDMA_CH_MAIN_MCSPI7_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH2_TX (CSL_PDMA_CH_MAIN_MCSPI7_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH3_TX (CSL_PDMA_CH_MAIN_MCSPI7_CH3_TX)
/*
 * PDMA MAIN MCAN TX Channels
 */
#define UDMA_PDMA_CH_MAIN_MCAN0_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN0_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN0_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN0_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN0_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN0_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN1_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN1_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN1_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN1_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN1_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN1_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN2_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN2_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN2_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN2_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN2_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN2_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN3_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN3_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN3_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN3_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN3_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN3_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN4_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN4_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN4_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN4_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN4_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN4_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN5_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN5_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN5_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN5_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN5_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN5_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN6_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN6_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN6_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN6_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN6_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN6_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN7_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN7_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN7_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN7_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN7_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN7_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN8_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN8_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN8_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN8_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN8_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN8_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN9_CH0_TX  (CSL_PDMA_CH_MAIN_MCAN9_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN9_CH1_TX  (CSL_PDMA_CH_MAIN_MCAN9_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN9_CH2_TX  (CSL_PDMA_CH_MAIN_MCAN9_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN10_CH0_TX (CSL_PDMA_CH_MAIN_MCAN10_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN10_CH1_TX (CSL_PDMA_CH_MAIN_MCAN10_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN10_CH2_TX (CSL_PDMA_CH_MAIN_MCAN10_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN11_CH0_TX (CSL_PDMA_CH_MAIN_MCAN11_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN11_CH1_TX (CSL_PDMA_CH_MAIN_MCAN11_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN11_CH2_TX (CSL_PDMA_CH_MAIN_MCAN11_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN12_CH0_TX (CSL_PDMA_CH_MAIN_MCAN12_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN12_CH1_TX (CSL_PDMA_CH_MAIN_MCAN12_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN12_CH2_TX (CSL_PDMA_CH_MAIN_MCAN12_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN13_CH0_TX (CSL_PDMA_CH_MAIN_MCAN13_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN13_CH1_TX (CSL_PDMA_CH_MAIN_MCAN13_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN13_CH2_TX (CSL_PDMA_CH_MAIN_MCAN13_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN14_CH0_TX (CSL_PDMA_CH_MAIN_MCAN14_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN14_CH1_TX (CSL_PDMA_CH_MAIN_MCAN14_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN14_CH2_TX (CSL_PDMA_CH_MAIN_MCAN14_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN15_CH0_TX (CSL_PDMA_CH_MAIN_MCAN15_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN15_CH1_TX (CSL_PDMA_CH_MAIN_MCAN15_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN15_CH2_TX (CSL_PDMA_CH_MAIN_MCAN15_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN16_CH0_TX (CSL_PDMA_CH_MAIN_MCAN16_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN16_CH1_TX (CSL_PDMA_CH_MAIN_MCAN16_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN16_CH2_TX (CSL_PDMA_CH_MAIN_MCAN16_CH2_TX)
#define UDMA_PDMA_CH_MAIN_MCAN17_CH0_TX (CSL_PDMA_CH_MAIN_MCAN17_CH0_TX)
#define UDMA_PDMA_CH_MAIN_MCAN17_CH1_TX (CSL_PDMA_CH_MAIN_MCAN17_CH1_TX)
#define UDMA_PDMA_CH_MAIN_MCAN17_CH2_TX (CSL_PDMA_CH_MAIN_MCAN17_CH2_TX)
/* @} */

/**
 *  \anchor Udma_PdmaChMcuTx
 *  \name MCU TX PDMA Channels
 *
 *  List of all MCU PDMA TX channels
 *
 *  @{
 */
/*
 * PDMA MCU McSPI TX Channels
 */
#define UDMA_PDMA_CH_MCU_MCSPI0_CH0_TX  (CSL_PDMA_CH_MCU_MCSPI0_CH0_TX)
#define UDMA_PDMA_CH_MCU_MCSPI0_CH1_TX  (CSL_PDMA_CH_MCU_MCSPI0_CH1_TX)
#define UDMA_PDMA_CH_MCU_MCSPI0_CH2_TX  (CSL_PDMA_CH_MCU_MCSPI0_CH2_TX)
#define UDMA_PDMA_CH_MCU_MCSPI0_CH3_TX  (CSL_PDMA_CH_MCU_MCSPI0_CH3_TX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH0_TX  (CSL_PDMA_CH_MCU_MCSPI1_CH0_TX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH1_TX  (CSL_PDMA_CH_MCU_MCSPI1_CH1_TX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH2_TX  (CSL_PDMA_CH_MCU_MCSPI1_CH2_TX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH3_TX  (CSL_PDMA_CH_MCU_MCSPI1_CH3_TX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH0_TX  (CSL_PDMA_CH_MCU_MCSPI2_CH0_TX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH1_TX  (CSL_PDMA_CH_MCU_MCSPI2_CH1_TX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH2_TX  (CSL_PDMA_CH_MCU_MCSPI2_CH2_TX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH3_TX  (CSL_PDMA_CH_MCU_MCSPI2_CH3_TX)
/*
 * PDMA MCU MCAN TX Channels
 */
#define UDMA_PDMA_CH_MCU_MCAN0_CH0_TX   (CSL_PDMA_CH_MCU_MCAN0_CH0_TX)
#define UDMA_PDMA_CH_MCU_MCAN0_CH1_TX   (CSL_PDMA_CH_MCU_MCAN0_CH1_TX)
#define UDMA_PDMA_CH_MCU_MCAN0_CH2_TX   (CSL_PDMA_CH_MCU_MCAN0_CH2_TX)
#define UDMA_PDMA_CH_MCU_MCAN1_CH0_TX   (CSL_PDMA_CH_MCU_MCAN1_CH0_TX)
#define UDMA_PDMA_CH_MCU_MCAN1_CH1_TX   (CSL_PDMA_CH_MCU_MCAN1_CH1_TX)
#define UDMA_PDMA_CH_MCU_MCAN1_CH2_TX   (CSL_PDMA_CH_MCU_MCAN1_CH2_TX)
/*
 * PDMA MCU UART TX Channels
 */
#define UDMA_PDMA_CH_MCU_UART0_TX       (CSL_PDMA_CH_MCU_UART0_CH0_TX)
/* @} */

/**
 *  \anchor Udma_PdmaChMainRx
 *  \name Main RX PDMA Channels
 *
 *  List of all Main PDMA RX channels
 *
 *  @{
 */
/*
 * PDMA Main McASP RX Channels
 */
#define UDMA_PDMA_CH_MAIN_MCASP0_RX     (CSL_PDMA_CH_MAIN_MCASP0_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCASP1_RX     (CSL_PDMA_CH_MAIN_MCASP1_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCASP2_RX     (CSL_PDMA_CH_MAIN_MCASP2_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCASP3_RX     (CSL_PDMA_CH_MAIN_MCASP3_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCASP4_RX     (CSL_PDMA_CH_MAIN_MCASP4_CH0_RX)
/*
 * PDMA Main UART RX Channels
 */
#define UDMA_PDMA_CH_MAIN_UART0_RX      (CSL_PDMA_CH_MAIN_UART0_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART1_RX      (CSL_PDMA_CH_MAIN_UART1_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART2_RX      (CSL_PDMA_CH_MAIN_UART2_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART3_RX      (CSL_PDMA_CH_MAIN_UART3_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART4_RX      (CSL_PDMA_CH_MAIN_UART4_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART5_RX      (CSL_PDMA_CH_MAIN_UART5_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART6_RX      (CSL_PDMA_CH_MAIN_UART6_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART7_RX      (CSL_PDMA_CH_MAIN_UART7_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART8_RX      (CSL_PDMA_CH_MAIN_UART8_CH0_RX)
#define UDMA_PDMA_CH_MAIN_UART9_RX      (CSL_PDMA_CH_MAIN_UART9_CH0_RX)
/*
 * PDMA Main McSPI RX Channels
 */
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI0_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI0_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI0_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI0_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI0_CH3_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI1_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI1_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI1_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI1_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI1_CH3_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI2_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI2_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI2_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI2_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI2_CH3_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI3_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI3_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI3_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI3_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI3_CH3_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI4_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI4_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI4_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI4_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI4_CH3_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI5_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI5_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI5_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI5_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI5_CH3_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI6_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI6_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI6_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI6_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI6_CH3_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH0_RX (CSL_PDMA_CH_MAIN_MCSPI7_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH1_RX (CSL_PDMA_CH_MAIN_MCSPI7_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH2_RX (CSL_PDMA_CH_MAIN_MCSPI7_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCSPI7_CH3_RX (CSL_PDMA_CH_MAIN_MCSPI7_CH3_RX)
/*
 * PDMA MAIN MCAN RX Channels
 */
#define UDMA_PDMA_CH_MAIN_MCAN0_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN0_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN0_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN0_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN0_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN0_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN1_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN1_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN1_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN1_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN1_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN1_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN2_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN2_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN2_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN2_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN2_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN2_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN3_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN3_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN3_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN3_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN3_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN3_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN4_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN4_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN4_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN4_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN4_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN4_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN5_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN5_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN5_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN5_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN5_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN5_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN6_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN6_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN6_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN6_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN6_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN6_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN7_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN7_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN7_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN7_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN7_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN7_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN8_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN8_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN8_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN8_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN8_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN8_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN9_CH0_RX  (CSL_PDMA_CH_MAIN_MCAN9_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN9_CH1_RX  (CSL_PDMA_CH_MAIN_MCAN9_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN9_CH2_RX  (CSL_PDMA_CH_MAIN_MCAN9_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN10_CH0_RX (CSL_PDMA_CH_MAIN_MCAN10_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN10_CH1_RX (CSL_PDMA_CH_MAIN_MCAN10_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN10_CH2_RX (CSL_PDMA_CH_MAIN_MCAN10_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN11_CH0_RX (CSL_PDMA_CH_MAIN_MCAN11_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN11_CH1_RX (CSL_PDMA_CH_MAIN_MCAN11_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN11_CH2_RX (CSL_PDMA_CH_MAIN_MCAN11_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN12_CH0_RX (CSL_PDMA_CH_MAIN_MCAN12_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN12_CH1_RX (CSL_PDMA_CH_MAIN_MCAN12_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN12_CH2_RX (CSL_PDMA_CH_MAIN_MCAN12_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN13_CH0_RX (CSL_PDMA_CH_MAIN_MCAN13_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN13_CH1_RX (CSL_PDMA_CH_MAIN_MCAN13_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN13_CH2_RX (CSL_PDMA_CH_MAIN_MCAN13_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN14_CH0_RX (CSL_PDMA_CH_MAIN_MCAN14_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN14_CH1_RX (CSL_PDMA_CH_MAIN_MCAN14_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN14_CH2_RX (CSL_PDMA_CH_MAIN_MCAN14_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN15_CH0_RX (CSL_PDMA_CH_MAIN_MCAN15_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN15_CH1_RX (CSL_PDMA_CH_MAIN_MCAN15_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN15_CH2_RX (CSL_PDMA_CH_MAIN_MCAN15_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN16_CH0_RX (CSL_PDMA_CH_MAIN_MCAN16_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN16_CH1_RX (CSL_PDMA_CH_MAIN_MCAN16_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN16_CH2_RX (CSL_PDMA_CH_MAIN_MCAN16_CH2_RX)
#define UDMA_PDMA_CH_MAIN_MCAN17_CH0_RX (CSL_PDMA_CH_MAIN_MCAN17_CH0_RX)
#define UDMA_PDMA_CH_MAIN_MCAN17_CH1_RX (CSL_PDMA_CH_MAIN_MCAN17_CH1_RX)
#define UDMA_PDMA_CH_MAIN_MCAN17_CH2_RX (CSL_PDMA_CH_MAIN_MCAN17_CH2_RX)
/* @} */

/**
 *  \anchor Udma_PdmaChMcuRx
 *  \name MCU RX PDMA Channels
 *
 *  List of all MCU PDMA RX channels
 *
 *  @{
 */
/*
 * PDMA MCU ADC RX Channels
 */
#define UDMA_PDMA_CH_MCU_ADC0_CH0_RX    (CSL_PDMA_CH_MCU_ADC0_CH0_RX)
#define UDMA_PDMA_CH_MCU_ADC0_CH1_RX    (CSL_PDMA_CH_MCU_ADC0_CH1_RX)
#define UDMA_PDMA_CH_MCU_ADC1_CH0_RX    (CSL_PDMA_CH_MCU_ADC1_CH0_RX)
#define UDMA_PDMA_CH_MCU_ADC1_CH1_RX    (CSL_PDMA_CH_MCU_ADC1_CH1_RX)
/*
 * PDMA MCU McSPI RX Channels
 */
#define UDMA_PDMA_CH_MCU_MCSPI0_CH0_RX  (CSL_PDMA_CH_MCU_MCSPI0_CH0_RX)
#define UDMA_PDMA_CH_MCU_MCSPI0_CH1_RX  (CSL_PDMA_CH_MCU_MCSPI0_CH1_RX)
#define UDMA_PDMA_CH_MCU_MCSPI0_CH2_RX  (CSL_PDMA_CH_MCU_MCSPI0_CH2_RX)
#define UDMA_PDMA_CH_MCU_MCSPI0_CH3_RX  (CSL_PDMA_CH_MCU_MCSPI0_CH3_RX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH0_RX  (CSL_PDMA_CH_MCU_MCSPI1_CH0_RX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH1_RX  (CSL_PDMA_CH_MCU_MCSPI1_CH1_RX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH2_RX  (CSL_PDMA_CH_MCU_MCSPI1_CH2_RX)
#define UDMA_PDMA_CH_MCU_MCSPI1_CH3_RX  (CSL_PDMA_CH_MCU_MCSPI1_CH3_RX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH0_RX  (CSL_PDMA_CH_MCU_MCSPI2_CH0_RX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH1_RX  (CSL_PDMA_CH_MCU_MCSPI2_CH1_RX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH2_RX  (CSL_PDMA_CH_MCU_MCSPI2_CH2_RX)
#define UDMA_PDMA_CH_MCU_MCSPI2_CH3_RX  (CSL_PDMA_CH_MCU_MCSPI2_CH3_RX)
/*
 * PDMA MCU MCAN RX Channels
 */
#define UDMA_PDMA_CH_MCU_MCAN0_CH0_RX   (CSL_PDMA_CH_MCU_MCAN0_CH0_RX)
#define UDMA_PDMA_CH_MCU_MCAN0_CH1_RX   (CSL_PDMA_CH_MCU_MCAN0_CH1_RX)
#define UDMA_PDMA_CH_MCU_MCAN0_CH2_RX   (CSL_PDMA_CH_MCU_MCAN0_CH2_RX)
#define UDMA_PDMA_CH_MCU_MCAN1_CH0_RX   (CSL_PDMA_CH_MCU_MCAN1_CH0_RX)
#define UDMA_PDMA_CH_MCU_MCAN1_CH1_RX   (CSL_PDMA_CH_MCU_MCAN1_CH1_RX)
#define UDMA_PDMA_CH_MCU_MCAN1_CH2_RX   (CSL_PDMA_CH_MCU_MCAN1_CH2_RX)
/*
 * PDMA MCU UART RX Channels
 */
#define UDMA_PDMA_CH_MCU_UART0_RX       (CSL_PDMA_CH_MCU_UART0_CH0_RX)
/* @} */

/* @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Returns the core ID
 *
 *  \return Core ID \ref Udma_CoreId
 */
uint32_t Udma_getCoreId(void);

/**
 *  \brief Returns the core tisci device ID
 *
 *  \return Core tisci Dev ID
 */
uint16_t Udma_getCoreSciDevId(void);

/**
 *  \brief Returns UTRUE if the memory is cache coherent
 *
 *  \return UTRUE/UFALSE
 */
uint32_t Udma_isCacheCoherent(void);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UDMA_SOC_H_ */
