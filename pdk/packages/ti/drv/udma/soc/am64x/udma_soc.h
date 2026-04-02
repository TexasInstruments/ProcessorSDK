/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2021
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
 *  \brief UDMA Low Level Driver AM64x SOC specific file.
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
#define UDMA_MAX(X,Y)  (X>Y ? X:Y)

/**
 *  \anchor Udma_InstanceIdSoc
 *  \name UDMA Instance ID specific to SOC
 *
 *  UDMA instance ID - BCDMA/PKTDMA
 *
 *  @{
 */
/** \brief BCDMA instance */
#define UDMA_INST_ID_BCDMA_0             (UDMA_INST_ID_2)
/** \brief PKTDMA instance */
#define UDMA_INST_ID_PKTDMA_0            (UDMA_INST_ID_3)
/** \brief Start of UDMA instance */
#define UDMA_INST_ID_START               (UDMA_INST_ID_2)
/** \brief Maximum number of UDMA instance */
#define UDMA_INST_ID_MAX                 (UDMA_INST_ID_3)
/** \brief Total number of UDMA instances */
#define UDMA_NUM_INST_ID                 (UDMA_INST_ID_MAX - UDMA_INST_ID_START + 1U)
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
#define UDMA_SOC_CFG_UDMAP_PRESENT               (0U)

/** \brief Flag to indicate BCDMA module is present or not in the SOC*/
#define UDMA_SOC_CFG_BCDMA_PRESENT               (1U) 

/** \brief Flag to indicate PKTDMA module is present or not in the SOC*/
#define UDMA_SOC_CFG_PKTDMA_PRESENT              (1U) 
    
/** \brief Flag to indicate Proxy is present or not in the SOC*/
#define UDMA_SOC_CFG_PROXY_PRESENT               (0U) 
	
/** \brief Flag to indicate Interrupt Router is present or not in the SOC*/
#define UDMA_SOC_CFG_INTR_ROUTER_PRESENT         (0U) 

/** \brief Flag to indicate Clec is present or not in the SOC*/
#define UDMA_SOC_CFG_CLEC_PRESENT                (0U) 

/** \brief Flag to indicate Normal RA is present or not in the SOC*/
#define UDMA_SOC_CFG_RA_NORMAL_PRESENT           (0U) 

/** \brief Flag to indicate LCDMA RA is present or not in the SOC*/
#define UDMA_SOC_CFG_RA_LCDMA_PRESENT            (1U)

/** \brief Flag to indicate Ring Monitor is present or not in the SOC*/
#define UDMA_SOC_CFG_RING_MON_PRESENT            (0U)

/** \brief Flag to indicate the SOC needs ring reset workaround */
#define UDMA_SOC_CFG_APPLY_RING_WORKAROUND       (0U)
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
#define UDMA_TX_UHC_CHANS_FDEPTH         (CSL_DMSS_UDMAP_TX_UHC_CHANS_FDEPTH)
/** \brief Tx High Capacity Channel FDEPTH*/
#define UDMA_TX_HC_CHANS_FDEPTH          (CSL_DMSS_UDMAP_TX_HC_CHANS_FDEPTH)
/** \brief Tx Normal Channel FDEPTH*/
#define UDMA_TX_CHANS_FDEPTH             (CSL_DMSS_UDMAP_TX_CHANS_FDEPTH)
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
#define UDMA_RINGACC_ASEL_ENDPOINT_PHYSADDR          (CSL_LCDMA_RINGACC_ASEL_ENDPOINT_PHYSADDR)
/** \brief PCIE0 */
#define UDMA_RINGACC_ASEL_ENDPOINT_PCIE0             (CSL_LCDMA_RINGACC_ASEL_ENDPOINT_PCIE0)
/** \brief ARM ACP port: write-allocate cacheable, bufferable */
#define UDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC      (CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC)
/** \brief ARM ACP port: read-allocate, cacheable, bufferable */
#define UDMA_RINGACC_ASEL_ENDPOINT_ACP_RD_ALLOC      (CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_RD_ALLOC)
/* @} */

/** \brief Invalid Ring Mode*/
#define UDMA_RING_MODE_INVALID          (CSL_LCDMA_RINGACC_RING_MODE_INVALID)

/** \brief Number of Mapped TX Group */
#define UDMA_NUM_MAPPED_TX_GROUP     (4U)
/**
 *  \anchor Udma_MappedTxGrpSoc
 *  \name Mapped TX Group specific to a SOC
 *
 *  List of all mapped TX groups present in the SOC.
 *
 *  @{
 */
#define UDMA_MAPPED_TX_GROUP_CPSW       (UDMA_MAPPED_GROUP0)
#define UDMA_MAPPED_TX_GROUP_SAUL       (UDMA_MAPPED_GROUP1)
#define UDMA_MAPPED_TX_GROUP_ICSSG_0    (UDMA_MAPPED_GROUP2)
#define UDMA_MAPPED_TX_GROUP_ICSSG_1    (UDMA_MAPPED_GROUP3)
/* @} */

/** \brief Number of Mapped RX Group */
#define UDMA_NUM_MAPPED_RX_GROUP     (4U)
/**
 *  \anchor Udma_MappedRxGrpSoc
 *  \name Mapped RX Group specific to a SOC
 *
 *  List of all mapped RX groups present in the SOC.
 *
 *  @{
 */
#define UDMA_MAPPED_RX_GROUP_CPSW       (UDMA_MAPPED_GROUP4)
#define UDMA_MAPPED_RX_GROUP_SAUL       (UDMA_MAPPED_GROUP5)
#define UDMA_MAPPED_RX_GROUP_ICSSG_0    (UDMA_MAPPED_GROUP6)
#define UDMA_MAPPED_RX_GROUP_ICSSG_1    (UDMA_MAPPED_GROUP7)
/* @} */

/** \brief Number of UTC instance */
#define UDMA_NUM_UTC_INSTANCE           (CSL_DMSS_UTC_CNT)
/**
 *  \anchor Udma_UtcIdSoc
 *  \name UTC ID specific to a SOC
 *
 *  List of all UTC's present in the SOC.
 *
 *  @{
 */
/* No UTC in AM64x  */
/* @} */

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
#define UDMA_CORE_ID_MPU1_0             (0U)
#define UDMA_CORE_ID_MCU2_0             (1U)
#define UDMA_CORE_ID_MCU2_1             (2U)
#define UDMA_CORE_ID_MCU1_0             (3U)
#define UDMA_CORE_ID_MCU1_1             (4U)
#define UDMA_CORE_ID_M4F_0              (5U)
/* Total number of cores */
#define UDMA_NUM_CORE                   (6U)
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
/** \brief Ultra High Capacity TX Channels */
#define UDMA_RM_RES_ID_TX_UHC                   (3U)
/** \brief High Capacity TX Channels */
#define UDMA_RM_RES_ID_TX_HC                    (4U) 
/** \brief Normal Capacity TX Channels */
#define UDMA_RM_RES_ID_TX                       (5U)
/** \brief Ultra High Capacity RX Channels */
#define UDMA_RM_RES_ID_RX_UHC                   (6U) 
/** \brief High Capacity RX Channels */
#define UDMA_RM_RES_ID_RX_HC                    (7U)       
/** \brief Normal Capacity RX Channels */
#define UDMA_RM_RES_ID_RX                       (8U) 
/** \brief Global Event */
#define UDMA_RM_RES_ID_GLOBAL_EVENT             (9U) 
/** \brief Virtual Interrupts */
#define UDMA_RM_RES_ID_VINTR                    (10U)        
/** \brief [Pktdma Only] Mapped TX Channels for CPSW */
#define UDMA_RM_RES_ID_MAPPED_TX_CPSW           (11U)        
/** \brief [Pktdma Only] Mapped TX Channels for SAUL_0 */
#define UDMA_RM_RES_ID_MAPPED_TX_SAUL_0         (12U)        
/** \brief [Pktdma Only] Mapped TX Channels for SAUL_1 */
#define UDMA_RM_RES_ID_MAPPED_TX_SAUL_1         (13U)       
/** \brief [Pktdma Only] Mapped TX Channels for ICSSG_0 */
#define UDMA_RM_RES_ID_MAPPED_TX_ICSSG_0        (14U)        
/** \brief [Pktdma Only] Mapped TX Channels for ICSSG_1 */
#define UDMA_RM_RES_ID_MAPPED_TX_ICSSG_1        (15U)       
/** \brief [Pktdma Only] Mapped RX Channels for CPSW */
#define UDMA_RM_RES_ID_MAPPED_RX_CPSW           (16U)        
/** \brief [Pktdma Only] Mapped RX Channels for SAUL_0 */
#define UDMA_RM_RES_ID_MAPPED_RX_SAUL_0         (17U)        
/** \brief [Pktdma Only] Mapped RX Channels for SAUL_1 */
#define UDMA_RM_RES_ID_MAPPED_RX_SAUL_1         (18U)          
/** \brief [Pktdma Only] Mapped RX Channels for SAUL_2 */
#define UDMA_RM_RES_ID_MAPPED_RX_SAUL_2         (19U)        
/** \brief [Pktdma Only] Mapped RX Channels for SAUL_3 */
#define UDMA_RM_RES_ID_MAPPED_RX_SAUL_3         (20U)       
/** \brief [Pktdma Only] Mapped RX Channels for ICSSG_0 */
#define UDMA_RM_RES_ID_MAPPED_RX_ICSSG_0        (21U)        
/** \brief [Pktdma Only] Mapped RX Channels for ICSSG_1 */
#define UDMA_RM_RES_ID_MAPPED_RX_ICSSG_1        (22U) 
/** \brief [Pktdma Only] Mapped TX Rings for CPSW */
#define UDMA_RM_RES_ID_MAPPED_TX_RING_CPSW      (23U)   
/** \brief [Pktdma Only] Mapped TX Rings for SAUL_0 */
#define UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_0    (24U)   
/** \brief [Pktdma Only] Mapped TX Rings for SAUL_1 */
#define UDMA_RM_RES_ID_MAPPED_TX_RING_SAUL_1    (25U)  
/** \brief [Pktdma Only] Mapped TX Rings for ICSSG_0 */
#define UDMA_RM_RES_ID_MAPPED_TX_RING_ICSSG_0   (26U)   
/** \brief [Pktdma Only] Mapped TX Rings for ICSSG_1 */
#define UDMA_RM_RES_ID_MAPPED_TX_RING_ICSSG_1   (27U)  
/** \brief [Pktdma Only] Mapped RX Rings for CPSW */
#define UDMA_RM_RES_ID_MAPPED_RX_RING_CPSW      (28U)   
/** \brief [Pktdma Only] Mapped RX Rings for SAUL_0 */
#define UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_0    (29U)   
/** \brief [Pktdma Only] Mapped RX Rings for SAUL_1 */
#define UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_1    (30U)
/** \brief [Pktdma Only] Mapped RX Rings for SAUL_2 */
#define UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_2    (31U)   
/** \brief [Pktdma Only] Mapped RX Rings for SAUL_3 */
#define UDMA_RM_RES_ID_MAPPED_RX_RING_SAUL_3    (32U)  
/** \brief [Pktdma Only] Mapped RX Rings for ICSSG_0 */
#define UDMA_RM_RES_ID_MAPPED_RX_RING_ICSSG_0   (33U)   
/** \brief [Pktdma Only] Mapped RX Rings for ICSSG_1 */
#define UDMA_RM_RES_ID_MAPPED_RX_RING_ICSSG_1   (34U)  
/** \brief Total number of BCDMA resources */
#define UDMA_RM_NUM_BCDMA_RES                   (11U) 
/** \brief Total number of PKTDMA resources */
#define UDMA_RM_NUM_PKTDMA_RES                  (35U) 
/** \brief Total number of resources */
#define UDMA_RM_NUM_RES                         UDMA_MAX(UDMA_RM_NUM_BCDMA_RES, UDMA_RM_NUM_PKTDMA_RES)
/* @} */

/** \brief Total number of shared resources - 
 *  Global_Event/IR Intr */
#define UDMA_RM_NUM_SHARED_RES                  (2U) 
/** \brief Maximum no.of instances to split a shared resource. 
 *  This should be max(UDMA_NUM_CORE,UDMA_NUM_INST_ID) */
#define UDMA_RM_SHARED_RES_MAX_INST             (UDMA_NUM_CORE)

/**
 *  \anchor Udma_PsilCh
 *  \name PSIL Channels
 *
 *  List of all PSIL channels and the corresponding counts
 *
 *  @{
 */

#define UDMA_PSIL_CH_CPSW2_TX           (CSL_PSILCFG_DMSS_CPSW2_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_SAUL0_TX           (CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_ICSS_G0_TX         (CSL_PSILCFG_DMSS_ICSS_G0_PSILD_THREAD_OFFSET)
#define UDMA_PSIL_CH_ICSS_G1_TX         (CSL_PSILCFG_DMSS_ICSS_G1_PSILD_THREAD_OFFSET)

#define UDMA_PSIL_CH_CPSW2_RX           (CSL_PSILCFG_DMSS_CPSW2_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_SAUL0_RX           (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_ICSS_G0_RX         (CSL_PSILCFG_DMSS_ICSS_G0_PSILS_THREAD_OFFSET)
#define UDMA_PSIL_CH_ICSS_G1_RX         (CSL_PSILCFG_DMSS_ICSS_G1_PSILS_THREAD_OFFSET)


#define UDMA_PSIL_CH_CPSW2_TX_CNT       (CSL_PSILCFG_DMSS_CPSW2_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_SAUL0_TX_CNT       (CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_ICSS_G0_TX_CNT     (CSL_PSILCFG_DMSS_ICSS_G0_PSILD_THREAD_CNT)
#define UDMA_PSIL_CH_ICSS_G1_TX_CNT     (CSL_PSILCFG_DMSS_ICSS_G1_PSILD_THREAD_CNT)

#define UDMA_PSIL_CH_CPSW2_RX_CNT       (CSL_PSILCFG_DMSS_CPSW2_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_SAUL0_RX_CNT       (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_ICSS_G0_RX_CNT     (CSL_PSILCFG_DMSS_ICSS_G0_PSILS_THREAD_CNT)
#define UDMA_PSIL_CH_ICSS_G1_RX_CNT     (CSL_PSILCFG_DMSS_ICSS_G1_PSILS_THREAD_CNT)

/* @} */


/**
 *  \anchor Udma_PdmaCh
 *  \name PDMA Channels
 *
 *  List of all PDMA channels across dmss domain
 *
 *  @{
 */

/**
 *  \anchor Udma_PdmaChMain0Tx
 *  \name Main0 TX PDMA Channels
 *
 *  List of all Main0 PDMA TX channels
 *
 *  @{
 */

/*
 * PDMA MAIN0 MCSPI TX Channels
 */
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH3_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH3_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH3_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH0_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH1_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH1_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH2_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH2_TX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH3_TX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH3_TX)
/*
 * PDMA MAIN0 UART TX Channels
 */
#define UDMA_PDMA_CH_MAIN0_UART0_TX       (CSL_PDMA_CH_MAIN0_UART0_CH0_TX)
#define UDMA_PDMA_CH_MAIN0_UART1_TX       (CSL_PDMA_CH_MAIN0_UART1_CH0_TX)

/* @} */

/**
 *  \anchor Udma_PdmaChMain0Rx
 *  \name Main0 RX PDMA Channels
 *
 *  List of all Main0 PDMA RX channels
 *
 *  @{
 */

/*
 * PDMA MAIN0 MCSPI RX Channels
 */
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI0_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI0_CH3_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI1_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI1_CH3_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI2_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI2_CH3_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH0_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH1_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH1_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH2_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH2_RX)
#define UDMA_PDMA_CH_MAIN0_MCSPI3_CH3_RX    (CSL_PDMA_CH_MAIN0_MCSPI3_CH3_RX)
/*
 * PDMA MAIN0 UART RX Channels
 */
#define UDMA_PDMA_CH_MAIN0_UART0_RX       (CSL_PDMA_CH_MAIN0_UART0_CH0_RX)
#define UDMA_PDMA_CH_MAIN0_UART1_RX       (CSL_PDMA_CH_MAIN0_UART1_CH0_RX)

/* @} */

/**
 *  \anchor Udma_PdmaChMain1Tx
 *  \name Main1 TX PDMA Channels
 *
 *  List of all Main1 PDMA TX channels
 *
 *  @{
 */

/*
 * PDMA MAIN1 MCSPI TX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH0_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH1_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH1_TX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH2_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH2_TX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH3_TX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH3_TX)
/*
 * PDMA MAIN1 UART TX Channels
 */
#define UDMA_PDMA_CH_MAIN1_UART2_TX (CSL_PDMA_CH_MAIN1_UART2_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART3_TX (CSL_PDMA_CH_MAIN1_UART3_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART4_TX (CSL_PDMA_CH_MAIN1_UART4_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART5_TX (CSL_PDMA_CH_MAIN1_UART5_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_UART6_TX (CSL_PDMA_CH_MAIN1_UART6_CH0_TX)
/*
 * PDMA MAIN1 MCAN TX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH0_TX (CSL_PDMA_CH_MAIN1_MCAN0_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH1_TX (CSL_PDMA_CH_MAIN1_MCAN0_CH1_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH2_TX (CSL_PDMA_CH_MAIN1_MCAN0_CH2_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH0_TX (CSL_PDMA_CH_MAIN1_MCAN1_CH0_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH1_TX (CSL_PDMA_CH_MAIN1_MCAN1_CH1_TX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH2_TX (CSL_PDMA_CH_MAIN1_MCAN1_CH2_TX)

/* @} */

/**
 *  \anchor Udma_PdmaChMain1Rx
 *  \name Main1 RX PDMA Channels
 *
 *  List of all Main1 PDMA RX channels
 *
 *  @{
 */

/*
 * PDMA MAIN1 MCSPI RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH0_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH1_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH1_RX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH2_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH2_RX)
#define UDMA_PDMA_CH_MAIN1_MCSPI4_CH3_RX    (CSL_PDMA_CH_MAIN1_MCSPI4_CH3_RX)
/*
 * PDMA MAIN1 UART RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_UART2_RX (CSL_PDMA_CH_MAIN1_UART2_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART3_RX (CSL_PDMA_CH_MAIN1_UART3_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART4_RX (CSL_PDMA_CH_MAIN1_UART4_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART5_RX (CSL_PDMA_CH_MAIN1_UART5_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_UART6_RX (CSL_PDMA_CH_MAIN1_UART6_CH0_RX)
/*
 * PDMA MAIN1 MCAN RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH0_RX (CSL_PDMA_CH_MAIN1_MCAN0_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH1_RX (CSL_PDMA_CH_MAIN1_MCAN0_CH1_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN0_CH2_RX (CSL_PDMA_CH_MAIN1_MCAN0_CH2_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH0_RX (CSL_PDMA_CH_MAIN1_MCAN1_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH1_RX (CSL_PDMA_CH_MAIN1_MCAN1_CH1_RX)
#define UDMA_PDMA_CH_MAIN1_MCAN1_CH2_RX (CSL_PDMA_CH_MAIN1_MCAN1_CH2_RX)
/*
 * PDMA MAIN1 ADC RX Channels
 */
#define UDMA_PDMA_CH_MAIN1_ADC0_CH0_RX  (CSL_PDMA_CH_MAIN1_ADC0_CH0_RX)
#define UDMA_PDMA_CH_MAIN1_ADC0_CH1_RX  (CSL_PDMA_CH_MAIN1_ADC0_CH1_RX)

/* @} */

/* @} */

/**
 *  \brief UDMA mapped channel ring attributes.
 */
typedef struct
{
    uint32_t    defaultRing;
    /**< Default ring number of a particular mapped channel. */
    uint32_t    startFreeRing;
    /**< Start free ring number of a particular mapped channel. */
    uint32_t    numFreeRing; 
    /**< Number of free rings for a particular mapped channel. */
}Udma_MappedChRingAttributes;

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
 *  \brief Returns TRUE if the memory is cache coherent
 *
 *  \return TRUE/FALSE
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
