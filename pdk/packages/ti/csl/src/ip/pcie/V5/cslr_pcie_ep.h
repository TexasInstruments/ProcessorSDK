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
 *  Name        : cslr_pcie_ep.h
*/
#ifndef CSLR_PCIE_EP_H_
#define CSLR_PCIE_EP_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  : PCIE data region0
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t PCIE_DATA_MEM[1];   /* PCIe data region0 */
} CSL_pcie_ep_dat0Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_PCIE_EP_DAT0_PCIE_DATA_MEM(PCIE_DATA_MEM)                             (0x00000000U+((PCIE_DATA_MEM)*0x4U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* PCIE_DATA_MEM */

#define CSL_PCIE_EP_DAT0_PCIE_DATA_MEM_PCIE_DATA_MASK                             (0xFFFFFFFFU)
#define CSL_PCIE_EP_DAT0_PCIE_DATA_MEM_PCIE_DATA_SHIFT                            (0x00000000U)
#define CSL_PCIE_EP_DAT0_PCIE_DATA_MEM_PCIE_DATA_MAX                              (0xFFFFFFFFU)

/**************************************************************************
* Hardware Region  : PCIE data region1
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t PCIE_DATA_MEM[1];   /* PCIe data region1 */
} CSL_pcie_ep_dat1Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_PCIE_EP_DAT1_PCIE_DATA_MEM(PCIE_DATA_MEM)                             (0x00000000U+((PCIE_DATA_MEM)*0x4U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* PCIE_DATA_MEM */

#define CSL_PCIE_EP_DAT1_PCIE_DATA_MEM_PCIE_DATA_MASK                             (0xFFFFFFFFU)
#define CSL_PCIE_EP_DAT1_PCIE_DATA_MEM_PCIE_DATA_SHIFT                            (0x00000000U)
#define CSL_PCIE_EP_DAT1_PCIE_DATA_MEM_PCIE_DATA_MAX                              (0xFFFFFFFFU)

/**************************************************************************
* Hardware Region  : PCIE data region2-untranslated address
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t PCIE_DATA_MEM[1];   /* PCIe data region2 */
} CSL_pcie_ep_dat2Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_PCIE_EP_DAT2_PCIE_DATA_MEM(PCIE_DATA_MEM)                             (0x00000000U+((PCIE_DATA_MEM)*0x4U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* PCIE_DATA_MEM */

#define CSL_PCIE_EP_DAT2_PCIE_DATA_MEM_PCIE_DATA_MASK                             (0xFFFFFFFFU)
#define CSL_PCIE_EP_DAT2_PCIE_DATA_MEM_PCIE_DATA_SHIFT                            (0x00000000U)
#define CSL_PCIE_EP_DAT2_PCIE_DATA_MEM_PCIE_DATA_MAX                              (0xFFFFFFFFU)

/**************************************************************************
* Hardware Region  : EP mode PCIE core registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t I_VENDOR_ID_DEVICE_ID;
    volatile uint32_t I_COMMAND_STATUS;
    volatile uint32_t I_REVISION_ID_CLASS_CODE;
    volatile uint32_t I_BIST_HEADER_LATENCY_CACHE_LINE;
    volatile uint32_t I_BASE_ADDR_0;
    volatile uint32_t I_BASE_ADDR_1;
    volatile uint32_t I_BASE_ADDR_2;
    volatile uint32_t I_BASE_ADDR_3;
    volatile uint32_t I_BASE_ADDR_4;
    volatile uint32_t I_BASE_ADDR_5;
    volatile uint32_t RSVD_0A;
    volatile uint32_t I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I;
    volatile uint32_t RSVD_0C;
    volatile uint32_t I_CAPABILITIES_POINTER;
    volatile uint32_t RSVD_0E;
    volatile uint32_t I_INTRPT_LINE_INTRPT_PIN;
    volatile uint32_t RSVD_010_01F[16];
} CSL_pcie_ep_coreRegs_EP_PF0_i_pcie_base;


typedef struct {
    volatile uint32_t I_PWR_MGMT_CAP;
    volatile uint32_t I_PWR_MGMT_CTRL_STAT_REP;
    volatile uint32_t RSVD_022_023[2];
} CSL_pcie_ep_coreRegs_EP_PF0_i_power_mgmt_cap_struct;


typedef struct {
    volatile uint32_t I_MSI_CTRL_REG;
    volatile uint32_t I_MSI_MSG_LOW_ADDR;
    volatile uint32_t I_MSI_MSG_HI_ADDR;
    volatile uint32_t I_MSI_MSG_DATA;
    volatile uint32_t I_MSI_MASK;
    volatile uint32_t I_MSI_PENDING_BITS;
    volatile uint32_t RSVD_02A_02B[2];
} CSL_pcie_ep_coreRegs_EP_PF0_i_MSI_cap_struct;


typedef struct {
    volatile uint32_t I_MSIX_CTRL;
    volatile uint32_t I_MSIX_TBL_OFFSET;
    volatile uint32_t I_MSIX_PENDING_INTRPT;
    volatile uint32_t RSVD_02F;
} CSL_pcie_ep_coreRegs_EP_PF0_i_MSIX_cap_struct;


typedef struct {
    volatile uint32_t I_PCIE_CAP_LIST;
    volatile uint32_t I_PCIE_DEV_CAP;
    volatile uint32_t I_PCIE_DEV_CTRL_STATUS;
    volatile uint32_t I_LINK_CAP;
    volatile uint32_t I_LINK_CTRL_STATUS;
    volatile uint32_t RSVD_035;
    volatile uint32_t RSVD_036;
    volatile uint32_t RSVD_037_038[2];
    volatile uint32_t I_PCIE_DEV_CAP_2;
    volatile uint32_t I_PCIE_DEV_CTRL_STATUS_2;
    volatile uint32_t I_LINK_CAP_2_REG;
    volatile uint32_t I_LINK_CTRL_STATUS_2;
    volatile uint32_t RSVD_03D_03F[3];
} CSL_pcie_ep_coreRegs_EP_PF0_i_PCIE_cap_struct;


typedef struct {
    volatile uint32_t I_AER_ENHANCED_CAP_HDR;
    volatile uint32_t I_UNCORR_ERR_STATUS;
    volatile uint32_t I_UNCORR_ERR_MASK;
    volatile uint32_t I_UNCORR_ERR_SEVERITY;
    volatile uint32_t I_CORR_ERR_STATUS;
    volatile uint32_t I_CORR_ERR_MASK;
    volatile uint32_t I_ADVCD_ERR_CAP_CTRL;
    volatile uint32_t I_HDR_LOG_0;
    volatile uint32_t I_HDR_LOG_1;
    volatile uint32_t I_HDR_LOG_2;
    volatile uint32_t I_HDR_LOG_3;
    volatile uint32_t RSVD_04B_04D[3];
    volatile uint32_t I_TLP_PRE_LOG_0;
} CSL_pcie_ep_coreRegs_EP_PF0_i_AER_cap_struct;


typedef struct {
    volatile uint32_t I_DEV_SER_NUM_CAP_HDR;
    volatile uint32_t I_DEV_SER_NUM_0;
    volatile uint32_t I_DEV_SER_NUM_1;
    volatile uint32_t RSVD_057;
} CSL_pcie_ep_coreRegs_EP_PF0_i_device_serial_num_cap_struct;


typedef struct {
    volatile uint32_t I_PWR_BDGTG_ENHC_CAP_HDR;
    volatile uint32_t I_PWR_BDGTG_DATA_SEL;
    volatile uint32_t I_PWR_BDGTG_DATA_REGISTER;
    volatile uint32_t I_PWR_BDGT_CAP;
    volatile uint32_t RSVD_05C_05F[4];
} CSL_pcie_ep_coreRegs_EP_PF0_i_power_budgeting_cap_struct;


typedef struct {
    volatile uint32_t I_RESIZE_BAR_EXT_CAP_HDR;
    volatile uint32_t I_RESIZE_BAR_CAP_0;
    volatile uint32_t I_RESIZE_BAR_CTRL_0;
    volatile uint32_t I_RESIZE_BAR_CAP_1;
    volatile uint32_t I_RESIZE_BAR_CTRL_1;
    volatile uint32_t I_RESIZE_BAR_CAP_2;
    volatile uint32_t I_RESIZE_BAR_CTRL_2;
    volatile uint32_t I_RESIZE_BAR_CAP_3;
    volatile uint32_t I_RESIZE_BAR_CTRL_3;
    volatile uint32_t I_RESIZE_BAR_CAP_4;
    volatile uint32_t I_RESIZE_BAR_CTRL_4;
    volatile uint32_t I_RESIZE_BAR_CAP_5;
    volatile uint32_t I_RESIZE_BAR_CTRL_5;
} CSL_pcie_ep_coreRegs_EP_PF0_i_resizable_BAR_cap_struct;


typedef struct {
    volatile uint32_t I_LTR_EXT_CAP_HDR;
    volatile uint32_t I_LTR_SNOOP_LAT;
} CSL_pcie_ep_coreRegs_EP_PF0_i_LTR_cap_struct;


typedef struct {
    volatile uint32_t I_DPA_EXT_CAP_HEADER_REG;
    volatile uint32_t I_DPA_CAP_REG;
    volatile uint32_t I_DPA_LAT_INDICATOR_REG;
    volatile uint32_t I_DPA_CTRL_STATUS_REG;
    volatile uint32_t I_DPA_POWER_ALLOC_REG0;
    volatile uint32_t I_DPA_POWER_ALLOC_REG1;
    volatile uint32_t RSVD_07C_07F[4];
} CSL_pcie_ep_coreRegs_EP_PF0_i_DPA_cap_struct;


typedef struct {
    volatile uint32_t I_SEC_PCIE_CAP_HDR_REG;
    volatile uint32_t I_LINK_CONTROL3_REG;
    volatile uint32_t I_LANE_ERROR_STATUS_REG;
    volatile uint32_t I_LANE_EQUALIZATION_CONTROL_REG0;
} CSL_pcie_ep_coreRegs_EP_PF0_i_PCIE_sec_ext_cap_struct;


typedef struct {
    volatile uint32_t I_VSEC_HEADER_REG;
    volatile uint32_t I_VENDOR_SPECIFIC_HEADER_REG;
    volatile uint32_t I_VENDOR_SPECIFIC_CONTROL_REG;
    volatile uint32_t I_VENDOR_SPECIFIC_DATA_REG0;
} CSL_pcie_ep_coreRegs_EP_PF0_i_regf_VSEC_struct;


typedef struct {
    volatile uint32_t I_PASID_HEADER_REG;
    volatile uint32_t I_PASID_CAP_REG;
} CSL_pcie_ep_coreRegs_EP_PF0_i_regf_PASID_struct;


typedef struct {
    volatile uint32_t I_VC_ENH_CAP_HEADER_REG;
    volatile uint32_t I_PORT_VC_CAP_REG_1;
    volatile uint32_t I_PORT_VC_CAP_REG_2;
    volatile uint32_t I_PORT_VC_CTRL_STS_REG;
    volatile uint32_t I_VC_RES_CAP_REG_0;
    volatile uint32_t I_VC_RES_CTRL_REG_0;
    volatile uint32_t I_VC_RES_STS_REG_0;
    volatile uint32_t I_VC_RES_CAP_REG_1;
    volatile uint32_t I_VC_RES_CTRL_REG_1;
    volatile uint32_t I_VC_RES_STS_REG_1;
    volatile uint32_t I_VC_RES_CAP_REG_2;
    volatile uint32_t I_VC_RES_CTRL_REG_2;
    volatile uint32_t I_VC_RES_STS_REG_2;
    volatile uint32_t I_VC_RES_CAP_REG_3;
    volatile uint32_t I_VC_RES_CTRL_REG_3;
    volatile uint32_t I_VC_RES_STS_REG_3;
} CSL_pcie_ep_coreRegs_EP_PF0_i_VC_cap_struct;


typedef struct {
    volatile uint32_t I_L1_PM_EXT_CAP_HDR;
    volatile uint32_t I_L1_PM_CAP;
    volatile uint32_t I_L1_PM_CTRL_1;
    volatile uint32_t I_L1_PM_CTRL_2;
} CSL_pcie_ep_coreRegs_EP_PF0_i_regf_L1_PM_cap_struct;


typedef struct {
    volatile uint32_t I_PTM_EXTENDED_CAPABILITY_HEADER_REG;
    volatile uint32_t I_PTM_CAPABILITIES_REG;
    volatile uint32_t I_PTM_CONTROL_REG;
} CSL_pcie_ep_coreRegs_EP_PF0_i_regf_ptm_cap;


typedef struct {
    volatile uint32_t I_PL_CONFIG_0_REG;
    volatile uint32_t I_PL_CONFIG_1_REG;
    volatile uint32_t I_DLL_TMR_CONFIG_REG;
    volatile uint32_t I_RCV_CRED_LIM_0_REG;
    volatile uint32_t I_RCV_CRED_LIM_1_REG;
    volatile uint32_t I_TRANSM_CRED_LIM_0_REG;
    volatile uint32_t I_TRANSM_CRED_LIM_1_REG;
    volatile uint32_t I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG;
    volatile uint32_t I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG;
    volatile uint32_t I_L0S_TIMEOUT_LIMIT_REG;
    volatile uint32_t I_TRANSMIT_TLP_COUNT_REG;
    volatile uint32_t I_TRANSMIT_TLP_PAYLOAD_DWORD_COUNT_REG;
    volatile uint32_t I_RECEIVE_TLP_COUNT_REG;
    volatile uint32_t I_RECEIVE_TLP_PAYLOAD_DWORD_COUNT_REG;
    volatile uint32_t I_COMPLN_TMOUT_LIM_0_REG;
    volatile uint32_t I_COMPLN_TMOUT_LIM_1_REG;
    volatile uint32_t I_L1_ST_REENTRY_DELAY_REG;
    volatile uint32_t I_VENDOR_ID_REG;
    volatile uint32_t I_ASPM_L1_ENTRY_TMOUT_DELAY_REG;
    volatile uint32_t I_PME_TURNOFF_ACK_DELAY_REG;
    volatile uint32_t I_LINKWIDTH_CONTROL_REG;
    volatile uint32_t I_PL_CONFIG_2_REG;
    volatile uint8_t  Resv_112[24];
    volatile uint32_t I_MULTI_VC_CONROL_REG;
    volatile uint32_t I_SRIS_CONTROL_REG;
    volatile uint8_t  Resv_128[8];
    volatile uint32_t I_RCV_CRED_LIM_0_REG_VC1;
    volatile uint32_t I_RCV_CRED_LIM_1_REG_VC1;
    volatile uint32_t I_TRANSM_CRED_LIM_0_REG_VC1;
    volatile uint32_t I_TRANSM_CRED_LIM_1_REG_VC1;
    volatile uint32_t I_RCV_CRED_LIM_0_REG_VC2;
    volatile uint32_t I_RCV_CRED_LIM_1_REG_VC2;
    volatile uint32_t I_TRANSM_CRED_LIM_0_REG_VC2;
    volatile uint32_t I_TRANSM_CRED_LIM_1_REG_VC2;
    volatile uint32_t I_RCV_CRED_LIM_0_REG_VC3;
    volatile uint32_t I_RCV_CRED_LIM_1_REG_VC3;
    volatile uint32_t I_TRANSM_CRED_LIM_0_REG_VC3;
    volatile uint32_t I_TRANSM_CRED_LIM_1_REG_VC3;
    volatile uint8_t  Resv_240[64];
    volatile uint32_t I_FC_INIT_DELAY_REG;
    volatile uint8_t  Resv_256[12];
    volatile uint32_t I_SHDW_HDR_LOG_0_REG;
    volatile uint32_t I_SHDW_HDR_LOG_1_REG;
    volatile uint32_t I_SHDW_HDR_LOG_2_REG;
    volatile uint32_t I_SHDW_HDR_LOG_3_REG;
    volatile uint32_t I_SHDW_FUNC_NUM_REG;
    volatile uint32_t I_SHDW_UR_ERR_REG;
    volatile uint8_t  Resv_320[40];
    volatile uint32_t I_PM_CLK_FREQUENCY_REG;
    volatile uint32_t I_DEBUG_DLLP_COUNT_GEN1_REG;
    volatile uint32_t I_DEBUG_DLLP_COUNT_GEN2_REG;
    volatile uint32_t I_DEBUG_DLLP_COUNT_GEN3_REG;
    volatile uint8_t  Resv_344[8];
    volatile uint32_t I_VENDOR_DEFINED_MESSAGE_TAG_REG;
    volatile uint8_t  Resv_512[164];
    volatile uint32_t I_NEGOTIATED_LANE_MAP_REG;
    volatile uint32_t I_RECEIVE_FTS_COUNT_REG;
    volatile uint32_t I_DEBUG_MUX_CONTROL_REG;
    volatile uint32_t I_LOCAL_ERROR_STATUS_REGISTER;
    volatile uint32_t I_LOCAL_INTRPT_MASK_REG;
    volatile uint32_t I_LCRC_ERR_COUNT_REG;
    volatile uint32_t I_ECC_CORR_ERR_COUNT_REG;
    volatile uint32_t I_LTR_SNOOP_LAT_REG;
    volatile uint32_t I_LTR_MSG_GEN_CTL_REG;
    volatile uint32_t I_PME_SERVICE_TIMEOUT_DELAY_REG;
    volatile uint32_t I_ROOT_PORT_REQUESTOR_ID_REG;
    volatile uint32_t I_EP_BUS_DEVICE_NUMBER_REG;
    volatile uint8_t  Resv_564[4];
    volatile uint32_t I_DEBUG_MUX_CONTROL_2_REG;
    volatile uint32_t I_PHY_STATUS_1_REG;
    volatile uint32_t I_DEBUG_MUX_CONTROL_3_REG;
    volatile uint32_t I_PF_0_BAR_CONFIG_0_REG;
    volatile uint32_t I_PF_0_BAR_CONFIG_1_REG;
    volatile uint8_t  Resv_704[120];
    volatile uint32_t I_PF_CONFIG_REG;
    volatile uint8_t  Resv_864[156];
    volatile uint32_t I_GEN3_DEFAULT_PRESET_REG;
    volatile uint32_t I_GEN3_LINK_EQ_TIMEOUT_2MS_REG;
    volatile uint32_t I_PIPE_FIFO_LATENCY_CTRL_REG;
    volatile uint8_t  Resv_892[16];
    volatile uint32_t I_GEN3_LINK_EQ_CTRL_REG;
    volatile uint32_t I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0;
    volatile uint8_t  Resv_3200[2300];
    volatile uint32_t I_ECC_CORR_ERR_COUNT_REG_AXI;
    volatile uint8_t  Resv_3208[4];
    volatile uint32_t LOW_POWER_DEBUG_AND_CONTROL0;
    volatile uint32_t LOW_POWER_DEBUG_AND_CONTROL1;
    volatile uint32_t LOW_POWER_DEBUG_AND_CONTROL2;
    volatile uint32_t TL_INTERNAL_CONTROL;
    volatile uint8_t  Resv_3328[104];
    volatile uint32_t I_LOCAL_ERROR_STATUS_2_REGISTER;
    volatile uint32_t I_LOCAL_INTRPT_MASK_2_REG;
    volatile uint8_t  Resv_3488[152];
    volatile uint32_t I_LD_CTRL;
    volatile uint32_t RX_ELEC_IDLE_FILTER_CONTROL;
    volatile uint32_t I_PTM_LOCAL_CONTROL_REG;
    volatile uint32_t I_PTM_LOCAL_STATUS_REG;
    volatile uint32_t I_PTM_LATENCY_PARAMETERS_INDEX_REG;
    volatile uint32_t I_PTM_LATENCY_PARAMETERS_REG;
    volatile uint32_t I_PTM_CONTEXT_1_REG;
    volatile uint32_t I_PTM_CONTEXT_2_REG;
    volatile uint32_t I_PTM_CONTEXT_3_REG;
    volatile uint32_t I_PTM_CONTEXT_4_REG;
    volatile uint32_t I_PTM_CONTEXT_5_REG;
    volatile uint32_t I_PTM_CONTEXT_6_REG;
    volatile uint32_t I_PTM_CONTEXT_7_REG;
    volatile uint32_t I_PTM_CONTEXT_8_REG;
    volatile uint32_t I_PTM_CONTEXT_9_REG;
    volatile uint32_t I_PTM_CONTEXT_10_REG;
    volatile uint32_t I_PTM_CONTEXT_11_REG;
    volatile uint8_t  Resv_3660[104];
    volatile uint32_t I_EQ_DEBUG_MON_CONTROL_REG;
    volatile uint32_t I_EQ_DEBUG_MON_STATUS0_REG;
    volatile uint32_t I_EQ_DEBUG_MON_STATUS_REG;
    volatile uint8_t  Resv_3676[4];
    volatile uint32_t I_AXI_FEATURE_REG;
    volatile uint32_t I_LINK_EQ_CONTROL_2_REG;
    volatile uint32_t I_CORE_FEATURE_REG;
    volatile uint8_t  Resv_3720[32];
    volatile uint32_t I_RX_INVERT_POLARITY_REG;
} CSL_pcie_ep_coreRegs_LM_i_regf_lm_pcie_base;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_0;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_1;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_2;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_3;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_4;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_5;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_6;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_7;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_8;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_9;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_10;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_11;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_12;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_13;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_14;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_15;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_16;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_17;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_18;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_19;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_20;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_21;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_22;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_23;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_24;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_25;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_26;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_27;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_28;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_29;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_30;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
    volatile uint32_t DESC0;
    volatile uint32_t DESC1;
    volatile uint8_t  Resv_20[4];
    volatile uint32_t DESC3;
    volatile uint32_t AXI_ADDR0;
    volatile uint32_t AXI_ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ob_31;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ib_0;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ib_1;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_wrapper_ib_7;


typedef struct {
    volatile uint32_t C0;
} CSL_pcie_ep_coreRegs_atu_credit_threshold;


typedef struct {
    volatile uint32_t L0;
} CSL_pcie_ep_coreRegs_atu_link_down_indicator_bit;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_0;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_1;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_2;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_3;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_4;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_5;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_6;


typedef struct {
    volatile uint32_t ADDR0;
    volatile uint32_t ADDR1;
} CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_7;


typedef struct {
    CSL_pcie_ep_coreRegs_EP_PF0_i_pcie_base EP_PF0_I_PCIE_BASE;
    CSL_pcie_ep_coreRegs_EP_PF0_i_power_mgmt_cap_struct EP_PF0_I_POWER_MGMT_CAP_STRUCT;
    CSL_pcie_ep_coreRegs_EP_PF0_i_MSI_cap_struct EP_PF0_I_MSI_CAP_STRUCT;
    CSL_pcie_ep_coreRegs_EP_PF0_i_MSIX_cap_struct EP_PF0_I_MSIX_CAP_STRUCT;
    CSL_pcie_ep_coreRegs_EP_PF0_i_PCIE_cap_struct EP_PF0_I_PCIE_CAP_STRUCT;
    CSL_pcie_ep_coreRegs_EP_PF0_i_AER_cap_struct EP_PF0_I_AER_CAP_STRUCT;
    volatile uint8_t  Resv_336[20];
    CSL_pcie_ep_coreRegs_EP_PF0_i_device_serial_num_cap_struct EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT;
    CSL_pcie_ep_coreRegs_EP_PF0_i_power_budgeting_cap_struct EP_PF0_I_POWER_BUDGETING_CAP_STRUCT;
    CSL_pcie_ep_coreRegs_EP_PF0_i_resizable_BAR_cap_struct EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT;
    volatile uint8_t  Resv_440[4];
    CSL_pcie_ep_coreRegs_EP_PF0_i_LTR_cap_struct EP_PF0_I_LTR_CAP_STRUCT;
    CSL_pcie_ep_coreRegs_EP_PF0_i_DPA_cap_struct EP_PF0_I_DPA_CAP_STRUCT;
    volatile uint8_t  Resv_768[280];
    CSL_pcie_ep_coreRegs_EP_PF0_i_PCIE_sec_ext_cap_struct EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT;
    volatile uint8_t  Resv_1024[240];
    CSL_pcie_ep_coreRegs_EP_PF0_i_regf_VSEC_struct EP_PF0_I_REGF_VSEC_STRUCT;
    volatile uint8_t  Resv_1088[48];
    CSL_pcie_ep_coreRegs_EP_PF0_i_regf_PASID_struct EP_PF0_I_REGF_PASID_STRUCT;
    volatile uint8_t  Resv_1216[120];
    CSL_pcie_ep_coreRegs_EP_PF0_i_VC_cap_struct EP_PF0_I_VC_CAP_STRUCT;
    volatile uint8_t  Resv_2304[1024];
    CSL_pcie_ep_coreRegs_EP_PF0_i_regf_L1_PM_cap_struct EP_PF0_I_REGF_L1_PM_CAP_STRUCT;
    volatile uint8_t  Resv_2592[272];
    CSL_pcie_ep_coreRegs_EP_PF0_i_regf_ptm_cap EP_PF0_I_REGF_PTM_CAP;
    volatile uint8_t  Resv_1048576[1045972];
    CSL_pcie_ep_coreRegs_LM_i_regf_lm_pcie_base LM_I_REGF_LM_PCIE_BASE;
    volatile uint8_t  Resv_4194304[3142004];
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_0 ATU_WRAPPER_OB_0;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_1 ATU_WRAPPER_OB_1;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_2 ATU_WRAPPER_OB_2;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_3 ATU_WRAPPER_OB_3;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_4 ATU_WRAPPER_OB_4;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_5 ATU_WRAPPER_OB_5;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_6 ATU_WRAPPER_OB_6;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_7 ATU_WRAPPER_OB_7;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_8 ATU_WRAPPER_OB_8;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_9 ATU_WRAPPER_OB_9;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_10 ATU_WRAPPER_OB_10;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_11 ATU_WRAPPER_OB_11;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_12 ATU_WRAPPER_OB_12;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_13 ATU_WRAPPER_OB_13;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_14 ATU_WRAPPER_OB_14;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_15 ATU_WRAPPER_OB_15;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_16 ATU_WRAPPER_OB_16;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_17 ATU_WRAPPER_OB_17;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_18 ATU_WRAPPER_OB_18;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_19 ATU_WRAPPER_OB_19;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_20 ATU_WRAPPER_OB_20;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_21 ATU_WRAPPER_OB_21;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_22 ATU_WRAPPER_OB_22;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_23 ATU_WRAPPER_OB_23;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_24 ATU_WRAPPER_OB_24;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_25 ATU_WRAPPER_OB_25;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_26 ATU_WRAPPER_OB_26;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_27 ATU_WRAPPER_OB_27;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_28 ATU_WRAPPER_OB_28;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_29 ATU_WRAPPER_OB_29;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_30 ATU_WRAPPER_OB_30;
    CSL_pcie_ep_coreRegs_atu_wrapper_ob_31 ATU_WRAPPER_OB_31;
    volatile uint8_t  Resv_4196352[1024];
    CSL_pcie_ep_coreRegs_atu_wrapper_ib_0 ATU_WRAPPER_IB_0;
    CSL_pcie_ep_coreRegs_atu_wrapper_ib_1 ATU_WRAPPER_IB_1;
    CSL_pcie_ep_coreRegs_atu_wrapper_ib_7 ATU_WRAPPER_IB_7;
    volatile uint8_t  Resv_4196384[8];
    CSL_pcie_ep_coreRegs_atu_credit_threshold ATU_CREDIT_THRESHOLD;
    CSL_pcie_ep_coreRegs_atu_link_down_indicator_bit ATU_LINK_DOWN_INDICATOR_BIT;
    volatile uint8_t  Resv_4196416[24];
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_0 ATU_FUNC0_WRAPPER_IB_EP_0;
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_1 ATU_FUNC0_WRAPPER_IB_EP_1;
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_2 ATU_FUNC0_WRAPPER_IB_EP_2;
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_3 ATU_FUNC0_WRAPPER_IB_EP_3;
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_4 ATU_FUNC0_WRAPPER_IB_EP_4;
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_5 ATU_FUNC0_WRAPPER_IB_EP_5;
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_6 ATU_FUNC0_WRAPPER_IB_EP_6;
    CSL_pcie_ep_coreRegs_atu_func0_wrapper_ib_ep_7 ATU_FUNC0_WRAPPER_IB_EP_7;
} CSL_pcie_ep_coreRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_VENDOR_ID_DEVICE_ID                 (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS                      (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE              (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE      (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0                         (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_1                         (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_2                         (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_3                         (0x0000001CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_4                         (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_5                         (0x00000024U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0A                               (0x00000028U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I     (0x0000002CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0C                               (0x00000030U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_CAPABILITIES_POINTER                (0x00000034U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0E                               (0x00000038U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN              (0x0000003CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_010_01F(RSVD_010_01F)            (0x00000040U+((RSVD_010_01F)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP            (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP  (0x00000084U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_RSVD_022_023(RSVD_022_023) (0x00000088U+((RSVD_022_023)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG                   (0x00000090U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_LOW_ADDR               (0x00000094U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_HI_ADDR                (0x00000098U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_DATA                   (0x0000009CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MASK                       (0x000000A0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_PENDING_BITS               (0x000000A4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_RSVD_02A_02B(RSVD_02A_02B)       (0x000000A8U+((RSVD_02A_02B)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL                     (0x000000B0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_TBL_OFFSET               (0x000000B4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_PENDING_INTRPT           (0x000000B8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_RSVD_02F                        (0x000000BCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST                 (0x000000C0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP                  (0x000000C4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS          (0x000000C8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP                      (0x000000CCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS              (0x000000D0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_035                        (0x000000D4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_036                        (0x000000D8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_037_038(RSVD_037_038)      (0x000000DCU+((RSVD_037_038)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2                (0x000000E4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2        (0x000000E8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG                (0x000000ECU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2            (0x000000F0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_03D_03F(RSVD_03D_03F)      (0x000000F4U+((RSVD_03D_03F)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR           (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS              (0x00000104U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK                (0x00000108U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY            (0x0000010CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS                (0x00000110U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK                  (0x00000114U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL             (0x00000118U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_0                      (0x0000011CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_1                      (0x00000120U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_2                      (0x00000124U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_3                      (0x00000128U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_RSVD_04B_04D(RSVD_04B_04D)       (0x0000012CU+((RSVD_04B_04D)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_TLP_PRE_LOG_0                  (0x00000138U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR (0x00000150U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_0    (0x00000154U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_1    (0x00000158U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_RSVD_057           (0x0000015CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR (0x00000160U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_SEL (0x00000164U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER (0x00000168U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGT_CAP       (0x0000016CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_RSVD_05C_05F(RSVD_05C_05F) (0x00000170U+((RSVD_05C_05F)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR (0x00000180U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0     (0x00000184U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0    (0x00000188U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1     (0x0000018CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1    (0x00000190U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2     (0x00000194U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2    (0x00000198U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3     (0x0000019CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3    (0x000001A0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4     (0x000001A4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4    (0x000001A8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5     (0x000001ACU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5    (0x000001B0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR                (0x000001B8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT                  (0x000001BCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG         (0x000001C0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG                    (0x000001C4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_LAT_INDICATOR_REG          (0x000001C8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG            (0x000001CCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0           (0x000001D0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1           (0x000001D4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_RSVD_07C_07F(RSVD_07C_07F)       (0x000001D8U+((RSVD_07C_07F)*0x4U))
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG  (0x00000300U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG     (0x00000304U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_ERROR_STATUS_REG (0x00000308U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0 (0x0000030CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG              (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG   (0x00000404U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG  (0x00000408U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_DATA_REG0    (0x0000040CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG            (0x00000440U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG               (0x00000444U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG           (0x000004C0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_1               (0x000004C4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_2               (0x000004C8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CTRL_STS_REG            (0x000004CCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_0                (0x000004D0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0               (0x000004D4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0                (0x000004D8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_1                (0x000004DCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1               (0x000004E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1                (0x000004E4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_2                (0x000004E8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2               (0x000004ECU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2                (0x000004F0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_3                (0x000004F4U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3               (0x000004F8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3                (0x000004FCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR       (0x00000900U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP               (0x00000904U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1            (0x00000908U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_2            (0x0000090CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG (0x00000A20U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG             (0x00000A24U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG                  (0x00000A28U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG                 (0x00100000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG                 (0x00100004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG              (0x00100008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG              (0x0010000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG              (0x00100010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG           (0x00100014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG           (0x00100018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG (0x0010001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG (0x00100020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L0S_TIMEOUT_LIMIT_REG           (0x00100024U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_COUNT_REG          (0x00100028U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_PAYLOAD_DWORD_COUNT_REG (0x0010002CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_COUNT_REG           (0x00100030U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_PAYLOAD_DWORD_COUNT_REG (0x00100034U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_0_REG          (0x00100038U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_1_REG          (0x0010003CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L1_ST_REENTRY_DELAY_REG         (0x00100040U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_ID_REG                   (0x00100044U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG   (0x00100048U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_TURNOFF_ACK_DELAY_REG       (0x0010004CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG           (0x00100050U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG                 (0x00100054U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG             (0x00100070U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SRIS_CONTROL_REG                (0x00100074U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1          (0x00100080U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1          (0x00100084U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1       (0x00100088U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1       (0x0010008CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2          (0x00100090U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2          (0x00100094U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2       (0x00100098U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2       (0x0010009CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3          (0x001000A0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3          (0x001000A4U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3       (0x001000A8U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3       (0x001000ACU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_FC_INIT_DELAY_REG               (0x001000F0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_0_REG              (0x00100100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_1_REG              (0x00100104U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_2_REG              (0x00100108U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_3_REG              (0x0010010CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_FUNC_NUM_REG               (0x00100110U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG                 (0x00100114U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PM_CLK_FREQUENCY_REG            (0x00100140U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN1_REG       (0x00100144U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN2_REG       (0x00100148U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN3_REG       (0x0010014CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_DEFINED_MESSAGE_TAG_REG  (0x00100158U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG         (0x00100200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG           (0x00100204U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG           (0x00100208U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER     (0x0010020CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG           (0x00100210U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LCRC_ERR_COUNT_REG              (0x00100214U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG          (0x00100218U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG               (0x0010021CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG             (0x00100220U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG   (0x00100224U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ROOT_PORT_REQUESTOR_ID_REG      (0x00100228U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG        (0x0010022CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG         (0x00100234U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG                (0x00100238U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG         (0x0010023CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG           (0x00100240U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG           (0x00100244U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_CONFIG_REG                   (0x001002C0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG         (0x00100360U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG    (0x00100364U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG      (0x00100368U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG           (0x0010037CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0 (0x00100380U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI      (0x00100C80U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0      (0x00100C88U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL1      (0x00100C8CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2      (0x00100C90U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_TL_INTERNAL_CONTROL               (0x00100C94U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER   (0x00100D00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG         (0x00100D04U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL                         (0x00100DA0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL       (0x00100DA4U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG           (0x00100DA8U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_STATUS_REG            (0x00100DACU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_INDEX_REG (0x00100DB0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG      (0x00100DB4U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_1_REG               (0x00100DB8U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_2_REG               (0x00100DBCU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_3_REG               (0x00100DC0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_4_REG               (0x00100DC4U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_5_REG               (0x00100DC8U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_6_REG               (0x00100DCCU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_7_REG               (0x00100DD0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_8_REG               (0x00100DD4U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_9_REG               (0x00100DD8U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_10_REG              (0x00100DDCU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_11_REG              (0x00100DE0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG        (0x00100E4CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG        (0x00100E50U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG         (0x00100E54U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG                 (0x00100E5CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG           (0x00100E60U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG                (0x00100E64U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RX_INVERT_POLARITY_REG          (0x00100E88U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0                                   (0x00400000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR1                                   (0x00400004U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0                                   (0x00400008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC1                                   (0x0040000CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC3                                   (0x00400014U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0                               (0x00400018U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR1                               (0x0040001CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0                                   (0x00400020U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR1                                   (0x00400024U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC0                                   (0x00400028U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC1                                   (0x0040002CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC3                                   (0x00400034U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0                               (0x00400038U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR1                               (0x0040003CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0                                   (0x00400040U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR1                                   (0x00400044U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC0                                   (0x00400048U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC1                                   (0x0040004CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC3                                   (0x00400054U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0                               (0x00400058U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR1                               (0x0040005CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0                                   (0x00400060U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR1                                   (0x00400064U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC0                                   (0x00400068U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC1                                   (0x0040006CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC3                                   (0x00400074U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0                               (0x00400078U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR1                               (0x0040007CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0                                   (0x00400080U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR1                                   (0x00400084U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC0                                   (0x00400088U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC1                                   (0x0040008CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC3                                   (0x00400094U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0                               (0x00400098U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR1                               (0x0040009CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0                                   (0x004000A0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR1                                   (0x004000A4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC0                                   (0x004000A8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC1                                   (0x004000ACU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC3                                   (0x004000B4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0                               (0x004000B8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR1                               (0x004000BCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0                                   (0x004000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR1                                   (0x004000C4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC0                                   (0x004000C8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC1                                   (0x004000CCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC3                                   (0x004000D4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0                               (0x004000D8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR1                               (0x004000DCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0                                   (0x004000E0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR1                                   (0x004000E4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC0                                   (0x004000E8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC1                                   (0x004000ECU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC3                                   (0x004000F4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0                               (0x004000F8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR1                               (0x004000FCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0                                   (0x00400100U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR1                                   (0x00400104U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC0                                   (0x00400108U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC1                                   (0x0040010CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC3                                   (0x00400114U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0                               (0x00400118U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR1                               (0x0040011CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0                                   (0x00400120U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR1                                   (0x00400124U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC0                                   (0x00400128U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC1                                   (0x0040012CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC3                                   (0x00400134U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0                               (0x00400138U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR1                               (0x0040013CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0                                  (0x00400140U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR1                                  (0x00400144U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC0                                  (0x00400148U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC1                                  (0x0040014CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC3                                  (0x00400154U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0                              (0x00400158U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR1                              (0x0040015CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0                                  (0x00400160U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR1                                  (0x00400164U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC0                                  (0x00400168U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC1                                  (0x0040016CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC3                                  (0x00400174U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0                              (0x00400178U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR1                              (0x0040017CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0                                  (0x00400180U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR1                                  (0x00400184U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC0                                  (0x00400188U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC1                                  (0x0040018CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC3                                  (0x00400194U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0                              (0x00400198U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR1                              (0x0040019CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0                                  (0x004001A0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR1                                  (0x004001A4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC0                                  (0x004001A8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC1                                  (0x004001ACU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC3                                  (0x004001B4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0                              (0x004001B8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR1                              (0x004001BCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0                                  (0x004001C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR1                                  (0x004001C4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC0                                  (0x004001C8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC1                                  (0x004001CCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC3                                  (0x004001D4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0                              (0x004001D8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR1                              (0x004001DCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0                                  (0x004001E0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR1                                  (0x004001E4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC0                                  (0x004001E8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC1                                  (0x004001ECU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC3                                  (0x004001F4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0                              (0x004001F8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR1                              (0x004001FCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0                                  (0x00400200U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR1                                  (0x00400204U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC0                                  (0x00400208U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC1                                  (0x0040020CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC3                                  (0x00400214U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0                              (0x00400218U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR1                              (0x0040021CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0                                  (0x00400220U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR1                                  (0x00400224U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC0                                  (0x00400228U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC1                                  (0x0040022CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC3                                  (0x00400234U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0                              (0x00400238U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR1                              (0x0040023CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0                                  (0x00400240U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR1                                  (0x00400244U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC0                                  (0x00400248U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC1                                  (0x0040024CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC3                                  (0x00400254U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0                              (0x00400258U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR1                              (0x0040025CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0                                  (0x00400260U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR1                                  (0x00400264U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC0                                  (0x00400268U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC1                                  (0x0040026CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC3                                  (0x00400274U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0                              (0x00400278U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR1                              (0x0040027CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0                                  (0x00400280U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR1                                  (0x00400284U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC0                                  (0x00400288U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC1                                  (0x0040028CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC3                                  (0x00400294U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0                              (0x00400298U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR1                              (0x0040029CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0                                  (0x004002A0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR1                                  (0x004002A4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC0                                  (0x004002A8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC1                                  (0x004002ACU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC3                                  (0x004002B4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0                              (0x004002B8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR1                              (0x004002BCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0                                  (0x004002C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR1                                  (0x004002C4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC0                                  (0x004002C8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC1                                  (0x004002CCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC3                                  (0x004002D4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0                              (0x004002D8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR1                              (0x004002DCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0                                  (0x004002E0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR1                                  (0x004002E4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC0                                  (0x004002E8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC1                                  (0x004002ECU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC3                                  (0x004002F4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0                              (0x004002F8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR1                              (0x004002FCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0                                  (0x00400300U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR1                                  (0x00400304U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC0                                  (0x00400308U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC1                                  (0x0040030CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC3                                  (0x00400314U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0                              (0x00400318U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR1                              (0x0040031CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0                                  (0x00400320U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR1                                  (0x00400324U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC0                                  (0x00400328U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC1                                  (0x0040032CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC3                                  (0x00400334U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0                              (0x00400338U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR1                              (0x0040033CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0                                  (0x00400340U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR1                                  (0x00400344U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC0                                  (0x00400348U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC1                                  (0x0040034CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC3                                  (0x00400354U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0                              (0x00400358U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR1                              (0x0040035CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0                                  (0x00400360U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR1                                  (0x00400364U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC0                                  (0x00400368U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC1                                  (0x0040036CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC3                                  (0x00400374U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0                              (0x00400378U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR1                              (0x0040037CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0                                  (0x00400380U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR1                                  (0x00400384U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC0                                  (0x00400388U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC1                                  (0x0040038CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC3                                  (0x00400394U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0                              (0x00400398U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR1                              (0x0040039CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0                                  (0x004003A0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR1                                  (0x004003A4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC0                                  (0x004003A8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC1                                  (0x004003ACU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC3                                  (0x004003B4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0                              (0x004003B8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR1                              (0x004003BCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0                                  (0x004003C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR1                                  (0x004003C4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC0                                  (0x004003C8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC1                                  (0x004003CCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC3                                  (0x004003D4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0                              (0x004003D8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR1                              (0x004003DCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0                                  (0x004003E0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR1                                  (0x004003E4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC0                                  (0x004003E8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC1                                  (0x004003ECU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC3                                  (0x004003F4U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0                              (0x004003F8U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR1                              (0x004003FCU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0                                   (0x00400800U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR1                                   (0x00400804U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0                                   (0x00400808U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR1                                   (0x0040080CU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0                                   (0x00400810U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR1                                   (0x00400814U)
#define CSL_PCIE_EP_CORE_ATU_CREDIT_THRESHOLD_C0                                  (0x00400820U)
#define CSL_PCIE_EP_CORE_ATU_LINK_DOWN_INDICATOR_BIT_L0                           (0x00400824U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR0                          (0x00400840U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR1                          (0x00400844U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR0                          (0x00400848U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR1                          (0x0040084CU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR0                          (0x00400850U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR1                          (0x00400854U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR0                          (0x00400858U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR1                          (0x0040085CU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR0                          (0x00400860U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR1                          (0x00400864U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR0                          (0x00400868U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR1                          (0x0040086CU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR0                          (0x00400870U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR1                          (0x00400874U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR0                          (0x00400878U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR1                          (0x0040087CU)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* I_VENDOR_ID_DEVICE_ID */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_VENDOR_ID_DEVICE_ID_VID_MASK        (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_VENDOR_ID_DEVICE_ID_VID_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_VENDOR_ID_DEVICE_ID_VID_MAX         (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_VENDOR_ID_DEVICE_ID_DID_MASK        (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_VENDOR_ID_DEVICE_ID_DID_SHIFT       (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_VENDOR_ID_DEVICE_ID_DID_MAX         (0x0000FFFFU)

/* I_COMMAND_STATUS */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_ISE_MASK             (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_ISE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_ISE_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_MSE_MASK             (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_MSE_SHIFT            (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_MSE_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_BE_MASK              (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_BE_SHIFT             (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_BE_MAX               (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R0_MASK              (0x00000038U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R0_SHIFT             (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R0_MAX               (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_PERE_MASK            (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_PERE_SHIFT           (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_PERE_MAX             (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R1_MASK              (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R1_SHIFT             (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R1_MAX               (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_SE_MASK              (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_SE_SHIFT             (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_SE_MAX               (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R2_MASK              (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R2_SHIFT             (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R2_MAX               (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_IMD_MASK             (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_IMD_SHIFT            (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_IMD_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R3_MASK              (0x0000F800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R3_SHIFT             (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R3_MAX               (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R4_MASK              (0x00070000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R4_SHIFT             (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R4_MAX               (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_IS_MASK              (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_IS_SHIFT             (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_IS_MAX               (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_CL_MASK              (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_CL_SHIFT             (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_CL_MAX               (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R5_MASK              (0x00E00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R5_SHIFT             (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R5_MAX               (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_MDPE_MASK            (0x01000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_MDPE_SHIFT           (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_MDPE_MAX             (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R6_MASK              (0x06000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R6_SHIFT             (0x00000019U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_R6_MAX               (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_STA_MASK             (0x08000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_STA_SHIFT            (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_STA_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_RTA_MASK             (0x10000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_RTA_SHIFT            (0x0000001CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_RTA_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_RMA_MASK             (0x20000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_RMA_SHIFT            (0x0000001DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_RMA_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_SSE_MASK             (0x40000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_SSE_SHIFT            (0x0000001EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_SSE_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_DPE_MASK             (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_DPE_SHIFT            (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_COMMAND_STATUS_DPE_MAX              (0x00000001U)

/* I_REVISION_ID_CLASS_CODE */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_RID_MASK     (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_RID_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_RID_MAX      (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_PIB_MASK     (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_PIB_SHIFT    (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_PIB_MAX      (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_SCC_MASK     (0x00FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_SCC_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_SCC_MAX      (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_CC_MASK      (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_CC_SHIFT     (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_REVISION_ID_CLASS_CODE_CC_MAX       (0x000000FFU)

/* I_BIST_HEADER_LATENCY_CACHE_LINE */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_CLS_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_CLS_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_CLS_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_LT_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_LT_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_LT_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_HT_MASK (0x007F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_HT_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_HT_MAX (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_DT_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_DT_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_DT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_BR_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_BR_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BIST_HEADER_LATENCY_CACHE_LINE_BR_MAX (0x000000FFU)

/* I_BASE_ADDR_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_MSI0_MASK               (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_MSI0_SHIFT              (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_MSI0_MAX                (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R7_MASK                 (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R7_SHIFT                (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R7_MAX                  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_S0_MASK                 (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_S0_SHIFT                (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_S0_MAX                  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_P0_MASK                 (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_P0_SHIFT                (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_P0_MAX                  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R8_MASK                 (0x000000F0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R8_SHIFT                (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_R8_MAX                  (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMR0_MASK              (0x00000F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMR0_SHIFT             (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMR0_MAX               (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMRW_MASK              (0xFFFFF000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMRW_SHIFT             (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_0_BAMRW_MAX               (0x000FFFFFU)

/* I_BASE_ADDR_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_1_R7_MASK                 (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_1_R7_SHIFT                (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_1_R7_MAX                  (0xFFFFFFFFU)

/* I_BASE_ADDR_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_2_R7_MASK                 (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_2_R7_SHIFT                (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_2_R7_MAX                  (0xFFFFFFFFU)

/* I_BASE_ADDR_3 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_3_R7_MASK                 (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_3_R7_SHIFT                (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_3_R7_MAX                  (0xFFFFFFFFU)

/* I_BASE_ADDR_4 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_4_R7_MASK                 (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_4_R7_SHIFT                (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_4_R7_MAX                  (0xFFFFFFFFU)

/* I_BASE_ADDR_5 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_5_R7_MASK                 (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_5_R7_SHIFT                (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_BASE_ADDR_5_R7_MAX                  (0xFFFFFFFFU)

/* RSVD_0A */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0A_RSVD_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0A_RSVD_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0A_RSVD_MAX                      (0xFFFFFFFFU)

/* I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I_SVID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I_SVID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I_SVID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I_SID_MASK (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I_SID_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_SUBSYSTEM_VENDOR_ID_SUBSYSTEM_I_SID_MAX (0x0000FFFFU)

/* RSVD_0C */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0C_RSVD_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0C_RSVD_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0C_RSVD_MAX                      (0xFFFFFFFFU)

/* I_CAPABILITIES_POINTER */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_CAPABILITIES_POINTER_CP_MASK        (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_CAPABILITIES_POINTER_CP_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_CAPABILITIES_POINTER_CP_MAX         (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_CAPABILITIES_POINTER_R15_MASK       (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_CAPABILITIES_POINTER_R15_SHIFT      (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_CAPABILITIES_POINTER_R15_MAX        (0x00FFFFFFU)

/* RSVD_0E */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0E_RSVD_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0E_RSVD_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_0E_RSVD_MAX                      (0xFFFFFFFFU)

/* I_INTRPT_LINE_INTRPT_PIN */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_ILR_MASK     (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_ILR_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_ILR_MAX      (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_IPR_MASK     (0x00000700U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_IPR_SHIFT    (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_IPR_MAX      (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_R16_MASK     (0xFFFFF800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_R16_SHIFT    (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_I_INTRPT_LINE_INTRPT_PIN_R16_MAX      (0x001FFFFFU)

/* RSVD_010_01F */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_010_01F_RSVD_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_010_01F_RSVD_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_BASE_RSVD_010_01F_RSVD_MAX                 (0xFFFFFFFFU)

/* I_PWR_MGMT_CAP */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_CID_MASK   (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_CID_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_CID_MAX    (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_CP_MASK    (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_CP_SHIFT   (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_CP_MAX     (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_VID_MASK   (0x00070000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_VID_SHIFT  (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_VID_MAX    (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PC_MASK    (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PC_SHIFT   (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PC_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_R0_MASK    (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_R0_SHIFT   (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_R0_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_DSI_MASK   (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_DSI_SHIFT  (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_DSI_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_MCRAPS_MASK (0x01C00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_MCRAPS_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_MCRAPS_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_D1S_MASK   (0x02000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_D1S_SHIFT  (0x00000019U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_D1S_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_D2S_MASK   (0x04000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_D2S_SHIFT  (0x0000001AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_D2S_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD0S_MASK (0x08000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD0S_SHIFT (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD0S_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD1S_MASK (0x10000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD1S_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD1S_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD2S_MASK (0x20000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD2S_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSD2S_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSDHS_MASK (0x40000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSDHS_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSDHS_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSDCS_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSDCS_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CAP_PSDCS_MAX  (0x00000001U)

/* I_PWR_MGMT_CTRL_STAT_REP */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PS_MASK (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PS_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PS_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R4_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R4_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R4_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_NSR_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_NSR_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_NSR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R3_MASK (0x000000F0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R3_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R3_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PE_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PE_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R2_MASK (0x00007E00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R2_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R2_MAX (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PMES_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PMES_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_PMES_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R1_MASK (0x00FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R1_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_R1_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_DR_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_DR_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_I_PWR_MGMT_CTRL_STAT_REP_DR_MAX (0x000000FFU)

/* RSVD_022_023 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_RSVD_022_023_RSVD_MASK    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_RSVD_022_023_RSVD_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_MGMT_CAP_STRUCT_RSVD_022_023_RSVD_MAX     (0xFFFFFFFFU)

/* I_MSI_CTRL_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CID1_MASK         (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CID1_SHIFT        (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CID1_MAX          (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CP1_MASK          (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CP1_SHIFT         (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_CP1_MAX           (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_ME_MASK           (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_ME_SHIFT          (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_ME_MAX            (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MMC_MASK          (0x000E0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MMC_SHIFT         (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MMC_MAX           (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MME_MASK          (0x00700000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MME_SHIFT         (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MME_MAX           (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_BAC64_MASK        (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_BAC64_SHIFT       (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_BAC64_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MC_MASK           (0x01000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MC_SHIFT          (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_MC_MAX            (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_R0_MASK           (0xFE000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_R0_SHIFT          (0x00000019U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_CTRL_REG_R0_MAX            (0x0000007FU)

/* I_MSI_MSG_LOW_ADDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_LOW_ADDR_R1_MASK       (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_LOW_ADDR_R1_SHIFT      (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_LOW_ADDR_R1_MAX        (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_LOW_ADDR_MAL_MASK      (0xFFFFFFFCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_LOW_ADDR_MAL_SHIFT     (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_LOW_ADDR_MAL_MAX       (0x3FFFFFFFU)

/* I_MSI_MSG_HI_ADDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_HI_ADDR_MAH_MASK       (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_HI_ADDR_MAH_SHIFT      (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_HI_ADDR_MAH_MAX        (0xFFFFFFFFU)

/* I_MSI_MSG_DATA */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_DATA_MD_MASK           (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_DATA_MD_SHIFT          (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_DATA_MD_MAX            (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_DATA_R2_MASK           (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_DATA_R2_SHIFT          (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MSG_DATA_R2_MAX            (0x0000FFFFU)

/* I_MSI_MASK */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MASK_MM_MASK               (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MASK_MM_SHIFT              (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MASK_MM_MAX                (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MASK_R0_MASK               (0xFFFFFFFEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MASK_R0_SHIFT              (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_MASK_R0_MAX                (0x7FFFFFFFU)

/* I_MSI_PENDING_BITS */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_PENDING_BITS_MP_MASK       (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_PENDING_BITS_MP_SHIFT      (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_PENDING_BITS_MP_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_PENDING_BITS_R0_MASK       (0xFFFFFFFEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_PENDING_BITS_R0_SHIFT      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_I_MSI_PENDING_BITS_R0_MAX        (0x7FFFFFFFU)

/* RSVD_02A_02B */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_RSVD_02A_02B_RSVD_MASK           (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_RSVD_02A_02B_RSVD_SHIFT          (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSI_CAP_STRUCT_RSVD_02A_02B_RSVD_MAX            (0xFFFFFFFFU)

/* I_MSIX_CTRL */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_CID_MASK            (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_CID_SHIFT           (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_CID_MAX             (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_CP_MASK             (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_CP_SHIFT            (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_CP_MAX              (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_MSIXTS_MASK         (0x07FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_MSIXTS_SHIFT        (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_MSIXTS_MAX          (0x000007FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_R0_MASK             (0x38000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_R0_SHIFT            (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_R0_MAX              (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_FM_MASK             (0x40000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_FM_SHIFT            (0x0000001EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_FM_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_MSIXE_MASK          (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_MSIXE_SHIFT         (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_CTRL_MSIXE_MAX           (0x00000001U)

/* I_MSIX_TBL_OFFSET */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_TBL_OFFSET_BARI_MASK     (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_TBL_OFFSET_BARI_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_TBL_OFFSET_BARI_MAX      (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_TBL_OFFSET_TO_MASK       (0xFFFFFFF8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_TBL_OFFSET_TO_SHIFT      (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_TBL_OFFSET_TO_MAX        (0x1FFFFFFFU)

/* I_MSIX_PENDING_INTRPT */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_PENDING_INTRPT_BARI1_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_PENDING_INTRPT_BARI1_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_PENDING_INTRPT_BARI1_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_PENDING_INTRPT_PBAO_MASK (0xFFFFFFF8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_PENDING_INTRPT_PBAO_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_I_MSIX_PENDING_INTRPT_PBAO_MAX  (0x1FFFFFFFU)

/* RSVD_02F */

#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_RSVD_02F_RSVD_MASK              (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_RSVD_02F_RSVD_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_MSIX_CAP_STRUCT_RSVD_02F_RSVD_MAX               (0xFFFFFFFFU)

/* I_PCIE_CAP_LIST */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_CID_MASK        (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_CID_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_CID_MAX         (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_NCP_MASK        (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_NCP_SHIFT       (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_NCP_MAX         (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_PCV_MASK        (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_PCV_SHIFT       (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_PCV_MAX         (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_DT_MASK         (0x00F00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_DT_SHIFT        (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_DT_MAX          (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_SS_MASK         (0x01000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_SS_SHIFT        (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_SS_MAX          (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_IMN_MASK        (0x3E000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_IMN_SHIFT       (0x00000019U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_IMN_MAX         (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_TRS_MASK        (0x40000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_TRS_SHIFT       (0x0000001EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_TRS_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_R0_MASK         (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_R0_SHIFT        (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_CAP_LIST_R0_MAX          (0x00000001U)

/* I_PCIE_DEV_CAP */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_MPS_MASK         (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_MPS_SHIFT        (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_MPS_MAX          (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_PFS_MASK         (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_PFS_SHIFT        (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_PFS_MAX          (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_ETFS_MASK        (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_ETFS_SHIFT       (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_ETFS_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_AL0SL_MASK       (0x000001C0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_AL0SL_SHIFT      (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_AL0SL_MAX        (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_AL1SL_MASK       (0x00000E00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_AL1SL_SHIFT      (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_AL1SL_MAX        (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R1_MASK          (0x00007000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R1_SHIFT         (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R1_MAX           (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_RBER_MASK        (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_RBER_SHIFT       (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_RBER_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R2_MASK          (0x00030000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R2_SHIFT         (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R2_MAX           (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_CSPLV_MASK       (0x03FC0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_CSPLV_SHIFT      (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_CSPLV_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_CPLS_MASK        (0x0C000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_CPLS_SHIFT       (0x0000001AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_CPLS_MAX         (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_FC_MASK          (0x10000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_FC_SHIFT         (0x0000001CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_FC_MAX           (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R3_MASK          (0xE0000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R3_SHIFT         (0x0000001DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_R3_MAX           (0x00000007U)

/* I_PCIE_DEV_CTRL_STATUS */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ECER_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ECER_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ECER_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ENFER_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ENFER_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ENFER_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EFER_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EFER_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EFER_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EURR_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EURR_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EURR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ERO_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ERO_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ERO_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_MPS_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_MPS_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_MPS_MAX  (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ETFE_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ETFE_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ETFE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EPH_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EPH_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EPH_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EAP_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EAP_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_EAP_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ENS_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ENS_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_ENS_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_MRRS_MASK (0x00007000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_MRRS_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_MRRS_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_FLR_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_FLR_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_FLR_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_CED_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_CED_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_CED_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_NFED_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_NFED_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_NFED_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_FED_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_FED_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_FED_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_URD_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_URD_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_URD_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_APD_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_APD_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_APD_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_TP_MASK  (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_TP_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_TP_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_R4_MASK  (0xFFC00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_R4_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_R4_MAX   (0x000003FFU)

/* I_LINK_CAP */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_MLS_MASK             (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_MLS_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_MLS_MAX              (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_MLW_MASK             (0x000003F0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_MLW_SHIFT            (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_MLW_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_ASPM_MASK            (0x00000C00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_ASPM_SHIFT           (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_ASPM_MAX             (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_L0SEL_MASK           (0x00007000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_L0SEL_SHIFT          (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_L0SEL_MAX            (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_L1EL_MASK            (0x00038000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_L1EL_SHIFT           (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_L1EL_MAX             (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_CPM_MASK             (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_CPM_SHIFT            (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_CPM_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_SDERC_MASK           (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_SDERC_SHIFT          (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_SDERC_MAX            (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_DLLARC_MASK          (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_DLLARC_SHIFT         (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_DLLARC_MAX           (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_LBNC_MASK            (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_LBNC_SHIFT           (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_LBNC_MAX             (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_AOC_MASK             (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_AOC_SHIFT            (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_AOC_MAX              (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_R5_MASK              (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_R5_SHIFT             (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_R5_MAX               (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_PN_MASK              (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_PN_SHIFT             (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_PN_MAX               (0x000000FFU)

/* I_LINK_CTRL_STATUS */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ASPMC_MASK   (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ASPMC_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ASPMC_MAX    (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R6_MASK      (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R6_SHIFT     (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R6_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_RCB_MASK     (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_RCB_SHIFT    (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_RCB_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LD_MASK      (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LD_SHIFT     (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LD_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_RL_MASK      (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_RL_SHIFT     (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_RL_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_CCC_MASK     (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_CCC_SHIFT    (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_CCC_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ES_MASK      (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ES_SHIFT     (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ES_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ECPM_MASK    (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ECPM_SHIFT   (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_ECPM_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_HAWD_MASK    (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_HAWD_SHIFT   (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_HAWD_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LBMIE_MASK   (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LBMIE_SHIFT  (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LBMIE_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LABIE_MASK   (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LABIE_SHIFT  (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LABIE_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R15_12_MASK  (0x0000F000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R15_12_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R15_12_MAX   (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLS_MASK     (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLS_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLS_MAX      (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLW_MASK     (0x03F00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLW_SHIFT    (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_NLW_MAX      (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R8_MASK      (0x04000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R8_SHIFT     (0x0000001AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_R8_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LTS_MASK     (0x08000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LTS_SHIFT    (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LTS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_SCC_MASK     (0x10000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_SCC_SHIFT    (0x0000001CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_SCC_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_DLLA_MASK    (0x20000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_DLLA_SHIFT   (0x0000001DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_DLLA_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LBMS_MASK    (0x40000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LBMS_SHIFT   (0x0000001EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LBMS_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LABS_MASK    (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LABS_SHIFT   (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_LABS_MAX     (0x00000001U)

/* RSVD_035 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_035_RSVD_MASK              (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_035_RSVD_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_035_RSVD_MAX               (0xFFFFFFFFU)

/* RSVD_036 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_036_RSVD_MASK              (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_036_RSVD_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_036_RSVD_MAX               (0xFFFFFFFFU)

/* RSVD_037_038 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_037_038_RSVD_MASK          (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_037_038_RSVD_SHIFT         (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_037_038_RSVD_MAX           (0xFFFFFFFFU)

/* I_PCIE_DEV_CAP_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_CTR_MASK       (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_CTR_SHIFT      (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_CTR_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_CTDS_MASK      (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_CTDS_SHIFT     (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_CTDS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_AFS_MASK       (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_AFS_SHIFT      (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_AFS_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_OPRS_MASK      (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_OPRS_SHIFT     (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_OPRS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS32_MASK   (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS32_SHIFT  (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS32_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS64_MASK   (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS64_SHIFT  (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS64_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS128_MASK  (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS128_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_BAOCS128_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R12_MASK       (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R12_SHIFT      (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R12_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_LMS_MASK       (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_LMS_SHIFT      (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_LMS_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_TCS_MASK       (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_TCS_SHIFT      (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_TCS_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R13_MASK       (0x0000C000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R13_SHIFT      (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R13_MAX        (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_OPFFS_MASK     (0x000C0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_OPFFS_SHIFT    (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_OPFFS_MAX      (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_EXFS_MASK      (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_EXFS_SHIFT     (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_EXFS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_EEPS_MASK      (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_EEPS_SHIFT     (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_EEPS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_MEEP_MASK      (0x00C00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_MEEP_SHIFT     (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_MEEP_MAX       (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R14_MASK       (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R14_SHIFT      (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CAP_2_R14_MAX        (0x000000FFU)

/* I_PCIE_DEV_CTRL_STATUS_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_CTV_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_CTV_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_CTV_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_CTD_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_CTD_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_CTD_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_AFE_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_AFE_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_AFE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_AORE_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_AORE_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_AORE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R16_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R16_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R16_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_IDORE_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_IDORE_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_IDORE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_IDOCE_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_IDOCE_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_IDOCE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_LTRME_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_LTRME_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_LTRME_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R17_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R17_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R17_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_OBFFE_MASK (0x00006000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_OBFFE_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_OBFFE_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R18_MASK (0xFFFF8000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R18_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_PCIE_DEV_CTRL_STATUS_2_R18_MAX (0x0001FFFFU)

/* I_LINK_CAP_2_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_SLSV_MASK      (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_SLSV_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_SLSV_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R1_MASK        (0x000001E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R1_SHIFT       (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R1_MAX         (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_LSOGSSV_MASK   (0x00000E00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_LSOGSSV_SHIFT  (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_LSOGSSV_MAX    (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R2_MASK        (0x0000F000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R2_SHIFT       (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R2_MAX         (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_LSORSSV_MASK   (0x00070000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_LSORSSV_SHIFT  (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_LSORSSV_MAX    (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R3_MASK        (0x00700000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R3_SHIFT       (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R3_MAX         (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R23_MASK       (0x01800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R23_SHIFT      (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R23_MAX        (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R25_MASK       (0x7E000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R25_SHIFT      (0x00000019U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R25_MAX        (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R31_MASK       (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R31_SHIFT      (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CAP_2_REG_R31_MAX        (0x00000001U)

/* I_LINK_CTRL_STATUS_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_TLS_MASK   (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_TLS_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_TLS_MAX    (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EC_MASK    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EC_SHIFT   (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EC_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_HASD_MASK  (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_HASD_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_HASD_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_SDE_MASK   (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_SDE_SHIFT  (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_SDE_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_TM_MASK    (0x00000380U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_TM_SHIFT   (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_TM_MAX     (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EMC_MASK   (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EMC_SHIFT  (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EMC_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CS_MASK    (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CS_SHIFT   (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CS_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CDE_MASK   (0x0000F000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CDE_SHIFT  (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CDE_MAX    (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CDEL_MASK  (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CDEL_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_CDEL_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EQC_MASK   (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EQC_SHIFT  (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EQC_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP1S_MASK  (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP1S_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP1S_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP2S_MASK  (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP2S_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP2S_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP3S_MASK  (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP3S_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_EP3S_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_LE_MASK    (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_LE_SHIFT   (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_LE_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_R21_MASK   (0xFFC00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_R21_SHIFT  (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_I_LINK_CTRL_STATUS_2_R21_MAX    (0x000003FFU)

/* RSVD_03D_03F */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_03D_03F_RSVD_MASK          (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_03D_03F_RSVD_SHIFT         (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_CAP_STRUCT_RSVD_03D_03F_RSVD_MAX           (0xFFFFFFFFU)

/* I_AER_ENHANCED_CAP_HDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_PEECI_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_PEECI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_PEECI_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_CV_MASK   (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_CV_SHIFT  (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_CV_MAX    (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_NCO_MASK  (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_NCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_AER_ENHANCED_CAP_HDR_NCO_MAX   (0x00000FFFU)

/* I_UNCORR_ERR_STATUS */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_LTE_MASK     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_LTE_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_LTE_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R0_MASK      (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R0_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R0_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_DLPES_MASK   (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_DLPES_SHIFT  (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_DLPES_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_SDES_MASK    (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_SDES_SHIFT   (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_SDES_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R1_MASK      (0x00000FC0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R1_SHIFT     (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R1_MAX       (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_PTS_MASK     (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_PTS_SHIFT    (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_PTS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_FCPES_MASK   (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_FCPES_SHIFT  (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_FCPES_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_CTS_MASK     (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_CTS_SHIFT    (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_CTS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_CAS_MASK     (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_CAS_SHIFT    (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_CAS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_UCS_MASK     (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_UCS_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_UCS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_ROS_MASK     (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_ROS_SHIFT    (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_ROS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_MTS_MASK     (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_MTS_SHIFT    (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_MTS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_EES_MASK     (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_EES_SHIFT    (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_EES_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_URES_MASK    (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_URES_SHIFT   (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_URES_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R2_MASK      (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R2_SHIFT     (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R2_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_UIE_MASK     (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_UIE_SHIFT    (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_UIE_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R3_MASK      (0xFF800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R3_SHIFT     (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_STATUS_R3_MAX       (0x000001FFU)

/* I_UNCORR_ERR_MASK */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_LTEM_MASK      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_LTEM_SHIFT     (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_LTEM_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R4_MASK        (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R4_SHIFT       (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R4_MAX         (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_DLPEM_MASK     (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_DLPEM_SHIFT    (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_DLPEM_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_SDESM_MASK     (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_SDESM_SHIFT    (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_SDESM_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R5_MASK        (0x00000FC0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R5_SHIFT       (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R5_MAX         (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_PTM_MASK       (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_PTM_SHIFT      (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_PTM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_FCPEM_MASK     (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_FCPEM_SHIFT    (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_FCPEM_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_CTM_MASK       (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_CTM_SHIFT      (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_CTM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_CAM_MASK       (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_CAM_SHIFT      (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_CAM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UCM_MASK       (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UCM_SHIFT      (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UCM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_ROM_MASK       (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_ROM_SHIFT      (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_ROM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_MTM_MASK       (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_MTM_SHIFT      (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_MTM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_EEM_MASK       (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_EEM_SHIFT      (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_EEM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UREM_MASK      (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UREM_SHIFT     (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UREM_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R6_MASK        (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R6_SHIFT       (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R6_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UIEM_MASK      (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UIEM_SHIFT     (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_UIEM_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R7_MASK        (0xFF800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R7_SHIFT       (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_MASK_R7_MAX         (0x000001FFU)

/* I_UNCORR_ERR_SEVERITY */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_LTES_MASK  (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_LTES_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_LTES_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R8_MASK    (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R8_SHIFT   (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R8_MAX     (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_DLPER_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_DLPER_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_DLPER_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_SDES_MASK  (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_SDES_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_SDES_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R10_MASK   (0x00000FC0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R10_SHIFT  (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R10_MAX    (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_PTS_MASK   (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_PTS_SHIFT  (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_PTS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_FCPES_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_FCPES_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_FCPES_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_CTS_MASK   (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_CTS_SHIFT  (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_CTS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_CAS_MASK   (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_CAS_SHIFT  (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_CAS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_UCS_MASK   (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_UCS_SHIFT  (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_UCS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_ROS_MASK   (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_ROS_SHIFT  (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_ROS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_MTS_MASK   (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_MTS_SHIFT  (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_MTS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_EES_MASK   (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_EES_SHIFT  (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_EES_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_URES_MASK  (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_URES_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_URES_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R11_MASK   (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R11_SHIFT  (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R11_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_UIES_MASK  (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_UIES_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_UIES_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R12_MASK   (0xFF800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R12_SHIFT  (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_UNCORR_ERR_SEVERITY_R12_MAX    (0x000001FFU)

/* I_CORR_ERR_STATUS */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RES_MASK       (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RES_SHIFT      (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RES_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R12_MASK       (0x0000003EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R12_SHIFT      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R12_MAX        (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_BTS_MASK       (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_BTS_SHIFT      (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_BTS_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_BDS_MASK       (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_BDS_SHIFT      (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_BDS_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RNRS_MASK      (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RNRS_SHIFT     (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RNRS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R13_MASK       (0x00000E00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R13_SHIFT      (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R13_MAX        (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RTTS_MASK      (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RTTS_SHIFT     (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_RTTS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_ANFES_MASK     (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_ANFES_SHIFT    (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_ANFES_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_CIES_MASK      (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_CIES_SHIFT     (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_CIES_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_HLOS_MASK      (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_HLOS_SHIFT     (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_HLOS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R14_MASK       (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R14_SHIFT      (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_STATUS_R14_MAX        (0x0000FFFFU)

/* I_CORR_ERR_MASK */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_REM_MASK         (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_REM_SHIFT        (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_REM_MAX          (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R15_MASK         (0x0000003EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R15_SHIFT        (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R15_MAX          (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_BTM_MASK         (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_BTM_SHIFT        (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_BTM_MAX          (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_BDM_MASK         (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_BDM_SHIFT        (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_BDM_MAX          (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_RNRM_MASK        (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_RNRM_SHIFT       (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_RNRM_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R16_MASK         (0x00000E00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R16_SHIFT        (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R16_MAX          (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_RTTM_MASK        (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_RTTM_SHIFT       (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_RTTM_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_ANFEM_MASK       (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_ANFEM_SHIFT      (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_ANFEM_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_CIEM_MASK        (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_CIEM_SHIFT       (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_CIEM_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_HLOM_MASK        (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_HLOM_SHIFT       (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_HLOM_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R17_MASK         (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R17_SHIFT        (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_CORR_ERR_MASK_R17_MAX          (0x0000FFFFU)

/* I_ADVCD_ERR_CAP_CTRL */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_FER_MASK    (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_FER_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_FER_MAX     (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EGC_MASK    (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EGC_SHIFT   (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EGC_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EEG_MASK    (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EEG_SHIFT   (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EEG_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_ECC_MASK    (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_ECC_SHIFT   (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_ECC_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EEC_MASK    (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EEC_SHIFT   (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_EEC_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_MHRC_MASK   (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_MHRC_SHIFT  (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_MHRC_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_MHRE_MASK   (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_MHRE_SHIFT  (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_MHRE_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_TPLP_MASK   (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_TPLP_SHIFT  (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_TPLP_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_R18_MASK    (0xFFFFF000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_R18_SHIFT   (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_ADVCD_ERR_CAP_CTRL_R18_MAX     (0x000FFFFFU)

/* I_HDR_LOG_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_0_HD0_MASK             (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_0_HD0_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_0_HD0_MAX              (0xFFFFFFFFU)

/* I_HDR_LOG_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_1_HD1_MASK             (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_1_HD1_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_1_HD1_MAX              (0xFFFFFFFFU)

/* I_HDR_LOG_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_2_HD2_MASK             (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_2_HD2_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_2_HD2_MAX              (0xFFFFFFFFU)

/* I_HDR_LOG_3 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_3_HD3_MASK             (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_3_HD3_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_HDR_LOG_3_HD3_MAX              (0xFFFFFFFFU)

/* RSVD_04B_04D */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_RSVD_04B_04D_RSVD_MASK           (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_RSVD_04B_04D_RSVD_SHIFT          (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_RSVD_04B_04D_RSVD_MAX            (0xFFFFFFFFU)

/* I_TLP_PRE_LOG_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_TLP_PRE_LOG_0_HD1_MASK         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_TLP_PRE_LOG_0_HD1_SHIFT        (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_AER_CAP_STRUCT_I_TLP_PRE_LOG_0_HD1_MAX          (0xFFFFFFFFU)

/* I_DEV_SER_NUM_CAP_HDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_PECID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_PECID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_PECID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_DSNCV_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_DSNCV_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_DSNCV_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_SNNCO_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_SNNCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_CAP_HDR_SNNCO_MAX (0x00000FFFU)

/* I_DEV_SER_NUM_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_0_DSND0_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_0_DSND0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_0_DSND0_MAX (0xFFFFFFFFU)

/* I_DEV_SER_NUM_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_1_DSND1_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_1_DSND1_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_I_DEV_SER_NUM_1_DSND1_MAX (0xFFFFFFFFU)

/* RSVD_057 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_RSVD_057_RSVD_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_RSVD_057_RSVD_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DEVICE_SERIAL_NUM_CAP_STRUCT_RSVD_057_RSVD_MAX  (0xFFFFFFFFU)

/* I_PWR_BDGTG_ENHC_CAP_HDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PECID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PECID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PECID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PCV_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PCV_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PCV_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PBNCO_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PBNCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_ENHC_CAP_HDR_PBNCO_MAX (0x00000FFFU)

/* I_PWR_BDGTG_DATA_SEL */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_SEL_PBDN_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_SEL_PBDN_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_SEL_PBDN_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_SEL_R0_MASK (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_SEL_R0_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_SEL_R0_MAX (0x00FFFFFFU)

/* I_PWR_BDGTG_DATA_REGISTER */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_BP_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_BP_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_BP_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_DS_MASK (0x00000300U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_DS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_DS_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PSS_MASK (0x00001C00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PSS_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PSS_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PS_MASK (0x00006000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PS_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PS_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_TYPE_MASK (0x00038000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_TYPE_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_TYPE_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PR_MASK (0x001C0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PR_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_PR_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_R1_MASK (0xFFE00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_R1_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGTG_DATA_REGISTER_R1_MAX (0x000007FFU)

/* I_PWR_BDGT_CAP */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGT_CAP_SA_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGT_CAP_SA_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGT_CAP_SA_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGT_CAP_R4_MASK (0xFFFFFFFEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGT_CAP_R4_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_I_PWR_BDGT_CAP_R4_MAX (0x7FFFFFFFU)

/* RSVD_05C_05F */

#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_RSVD_05C_05F_RSVD_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_RSVD_05C_05F_RSVD_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_POWER_BUDGETING_CAP_STRUCT_RSVD_05C_05F_RSVD_MAX (0xFFFFFFFFU)

/* I_RESIZE_BAR_EXT_CAP_HDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_PECID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_PECID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_PECID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_CV_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_CV_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_CV_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_NCO_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_NCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_EXT_CAP_HDR_NCO_MAX (0x00000FFFU)

/* I_RESIZE_BAR_CAP_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_R0_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_R0_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A1M_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A1M_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A1M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A2M_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A2M_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A2M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A4M_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A4M_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A4M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A8M_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A8M_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A8M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A16M_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A16M_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A16M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A32M_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A32M_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A32M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A64M_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A64M_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A64M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A128M_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A128M_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A128M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A256M_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A256M_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A256M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A512M_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A512M_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A512M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A1G_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A1G_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A1G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A2G_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A2G_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A2G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A4G_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A4G_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A4G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A8G_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A8G_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A8G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A16G_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A16G_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A16G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A32G_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A32G_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A32G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A64G_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A64G_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A64G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A128G_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A128G_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A128G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A256G_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A256G_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A256G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A512G_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A512G_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_A512G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_R1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_R1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_0_R1_MAX (0x000000FFU)

/* I_RESIZE_BAR_CTRL_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_BARI_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_BARI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_BARI_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_R2_MASK (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_R2_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_R2_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_RBARC_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_RBARC_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_RBARC_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_BARS_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_BARS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_BARS_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_R3_MASK (0xFFFFE000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_R3_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_0_R3_MAX (0x0007FFFFU)

/* I_RESIZE_BAR_CAP_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_R0_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_R0_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A1M_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A1M_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A1M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A2M_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A2M_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A2M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A4M_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A4M_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A4M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A8M_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A8M_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A8M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A16M_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A16M_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A16M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A32M_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A32M_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A32M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A64M_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A64M_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A64M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A128M_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A128M_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A128M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A256M_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A256M_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A256M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A512M_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A512M_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A512M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A1G_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A1G_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A1G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A2G_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A2G_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A2G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A4G_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A4G_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A4G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A8G_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A8G_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A8G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A16G_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A16G_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A16G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A32G_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A32G_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A32G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A64G_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A64G_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A64G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A128G_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A128G_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A128G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A256G_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A256G_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A256G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A512G_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A512G_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_A512G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_R1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_R1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_1_R1_MAX (0x000000FFU)

/* I_RESIZE_BAR_CTRL_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_BARI_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_BARI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_BARI_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_R2_MASK (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_R2_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_R2_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_RBARC_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_RBARC_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_RBARC_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_BARS_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_BARS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_BARS_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_R3_MASK (0xFFFFE000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_R3_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_1_R3_MAX (0x0007FFFFU)

/* I_RESIZE_BAR_CAP_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_R0_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_R0_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A1M_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A1M_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A1M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A2M_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A2M_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A2M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A4M_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A4M_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A4M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A8M_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A8M_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A8M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A16M_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A16M_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A16M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A32M_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A32M_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A32M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A64M_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A64M_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A64M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A128M_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A128M_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A128M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A256M_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A256M_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A256M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A512M_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A512M_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A512M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A1G_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A1G_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A1G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A2G_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A2G_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A2G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A4G_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A4G_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A4G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A8G_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A8G_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A8G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A16G_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A16G_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A16G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A32G_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A32G_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A32G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A64G_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A64G_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A64G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A128G_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A128G_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A128G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A256G_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A256G_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A256G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A512G_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A512G_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_A512G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_R1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_R1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_2_R1_MAX (0x000000FFU)

/* I_RESIZE_BAR_CTRL_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_BARI_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_BARI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_BARI_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_R2_MASK (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_R2_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_R2_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_RBARC_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_RBARC_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_RBARC_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_BARS_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_BARS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_BARS_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_R3_MASK (0xFFFFE000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_R3_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_2_R3_MAX (0x0007FFFFU)

/* I_RESIZE_BAR_CAP_3 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_R0_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_R0_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A1M_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A1M_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A1M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A2M_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A2M_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A2M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A4M_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A4M_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A4M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A8M_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A8M_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A8M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A16M_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A16M_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A16M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A32M_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A32M_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A32M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A64M_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A64M_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A64M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A128M_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A128M_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A128M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A256M_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A256M_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A256M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A512M_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A512M_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A512M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A1G_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A1G_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A1G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A2G_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A2G_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A2G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A4G_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A4G_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A4G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A8G_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A8G_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A8G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A16G_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A16G_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A16G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A32G_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A32G_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A32G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A64G_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A64G_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A64G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A128G_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A128G_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A128G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A256G_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A256G_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A256G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A512G_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A512G_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_A512G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_R1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_R1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_3_R1_MAX (0x000000FFU)

/* I_RESIZE_BAR_CTRL_3 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_BARI_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_BARI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_BARI_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_R2_MASK (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_R2_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_R2_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_RBARC_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_RBARC_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_RBARC_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_BARS_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_BARS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_BARS_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_R3_MASK (0xFFFFE000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_R3_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_3_R3_MAX (0x0007FFFFU)

/* I_RESIZE_BAR_CAP_4 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_R0_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_R0_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A1M_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A1M_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A1M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A2M_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A2M_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A2M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A4M_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A4M_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A4M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A8M_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A8M_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A8M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A16M_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A16M_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A16M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A32M_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A32M_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A32M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A64M_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A64M_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A64M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A128M_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A128M_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A128M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A256M_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A256M_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A256M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A512M_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A512M_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A512M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A1G_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A1G_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A1G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A2G_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A2G_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A2G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A4G_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A4G_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A4G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A8G_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A8G_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A8G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A16G_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A16G_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A16G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A32G_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A32G_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A32G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A64G_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A64G_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A64G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A128G_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A128G_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A128G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A256G_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A256G_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A256G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A512G_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A512G_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_A512G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_R1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_R1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_4_R1_MAX (0x000000FFU)

/* I_RESIZE_BAR_CTRL_4 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_BARI_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_BARI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_BARI_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_R2_MASK (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_R2_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_R2_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_RBARC_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_RBARC_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_RBARC_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_BARS_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_BARS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_BARS_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_R3_MASK (0xFFFFE000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_R3_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_4_R3_MAX (0x0007FFFFU)

/* I_RESIZE_BAR_CAP_5 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_R0_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_R0_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A1M_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A1M_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A1M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A2M_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A2M_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A2M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A4M_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A4M_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A4M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A8M_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A8M_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A8M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A16M_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A16M_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A16M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A32M_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A32M_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A32M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A64M_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A64M_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A64M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A128M_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A128M_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A128M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A256M_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A256M_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A256M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A512M_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A512M_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A512M_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A1G_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A1G_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A1G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A2G_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A2G_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A2G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A4G_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A4G_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A4G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A8G_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A8G_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A8G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A16G_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A16G_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A16G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A32G_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A32G_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A32G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A64G_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A64G_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A64G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A128G_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A128G_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A128G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A256G_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A256G_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A256G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A512G_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A512G_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_A512G_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_R1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_R1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CAP_5_R1_MAX (0x000000FFU)

/* I_RESIZE_BAR_CTRL_5 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_BARI_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_BARI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_BARI_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_R2_MASK (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_R2_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_R2_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_RBARC_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_RBARC_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_RBARC_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_BARS_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_BARS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_BARS_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_R3_MASK (0xFFFFE000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_R3_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_RESIZABLE_BAR_CAP_STRUCT_I_RESIZE_BAR_CTRL_5_R3_MAX (0x0007FFFFU)

/* I_LTR_EXT_CAP_HDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_PECID_MASK     (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_PECID_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_PECID_MAX      (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_CV_MASK        (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_CV_SHIFT       (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_CV_MAX         (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_NCO_MASK       (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_NCO_SHIFT      (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_EXT_CAP_HDR_NCO_MAX        (0x00000FFFU)

/* I_LTR_SNOOP_LAT */

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MSL_MASK         (0x000003FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MSL_SHIFT        (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MSL_MAX          (0x000003FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MSLS_MASK        (0x00001C00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MSLS_SHIFT       (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MSLS_MAX         (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_R0_MASK          (0x0000E000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_R0_SHIFT         (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_R0_MAX           (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MNSL_MASK        (0x03FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MNSL_SHIFT       (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MNSL_MAX         (0x000003FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MNSLS_MASK       (0x1C000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MNSLS_SHIFT      (0x0000001AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_MNSLS_MAX        (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_R1_MASK          (0xE0000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_R1_SHIFT         (0x0000001DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_LTR_CAP_STRUCT_I_LTR_SNOOP_LAT_R1_MAX           (0x00000007U)

/* I_DPA_EXT_CAP_HEADER_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_PECID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_PECID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_PECID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_CV_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_CV_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_CV_MAX  (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_NCO_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_NCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_EXT_CAP_HEADER_REG_NCO_MAX (0x00000FFFU)

/* I_DPA_CAP_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_MNS_MASK           (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_MNS_SHIFT          (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_MNS_MAX            (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R0_MASK            (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R0_SHIFT           (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R0_MAX             (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLU_MASK           (0x00000300U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLU_SHIFT          (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLU_MAX            (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R1_MASK            (0x00000C00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R1_SHIFT           (0x0000000AU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R1_MAX             (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_PAS_MASK           (0x00003000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_PAS_SHIFT          (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_PAS_MAX            (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R2_MASK            (0x0000C000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R2_SHIFT           (0x0000000EU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_R2_MAX             (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLV0_MASK          (0x00FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLV0_SHIFT         (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLV0_MAX           (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLV1_MASK          (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLV1_SHIFT         (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CAP_REG_TLV1_MAX           (0x000000FFU)

/* I_DPA_LAT_INDICATOR_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_LAT_INDICATOR_REG_TLIN_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_LAT_INDICATOR_REG_TLIN_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_LAT_INDICATOR_REG_TLIN_MAX (0xFFFFFFFFU)

/* I_DPA_CTRL_STATUS_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SS_MASK    (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SS_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SS_MAX     (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R3_MASK    (0x000000E0U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R3_SHIFT   (0x00000005U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R3_MAX     (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SCE_MASK   (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SCE_SHIFT  (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SCE_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R4_MASK    (0x0000FE00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R4_SHIFT   (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R4_MAX     (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SC_MASK    (0x001F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SC_SHIFT   (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_SC_MAX     (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R5_MASK    (0xFFE00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R5_SHIFT   (0x00000015U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_CTRL_STATUS_REG_R5_MAX     (0x000007FFU)

/* I_DPA_POWER_ALLOC_REG0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA0_0_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA0_0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA0_0_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA1_0_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA1_0_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA1_0_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA2_0_MASK (0x00FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA2_0_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA2_0_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA3_0_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA3_0_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG0_SPA3_0_MAX (0x000000FFU)

/* I_DPA_POWER_ALLOC_REG1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA0_1_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA0_1_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA0_1_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA1_1_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA1_1_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA1_1_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA2_1_MASK (0x00FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA2_1_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA2_1_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA3_1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA3_1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_I_DPA_POWER_ALLOC_REG1_SPA3_1_MAX (0x000000FFU)

/* RSVD_07C_07F */

#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_RSVD_07C_07F_RSVD_MASK           (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_RSVD_07C_07F_RSVD_SHIFT          (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_DPA_CAP_STRUCT_RSVD_07C_07F_RSVD_MAX            (0xFFFFFFFFU)

/* I_SEC_PCIE_CAP_HDR_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_PECI_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_PECI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_PECI_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_CV_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_CV_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_CV_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_NCO_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_NCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_SEC_PCIE_CAP_HDR_REG_NCO_MAX (0x00000FFFU)

/* I_LINK_CONTROL3_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_R1_MASK (0x000001FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_R1_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_R1_MAX (0x000001FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_ELSOSGV_MASK (0x00000E00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_ELSOSGV_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_ELSOSGV_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_R2_MASK (0xFFFFF000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_R2_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LINK_CONTROL3_REG_R2_MAX (0x000FFFFFU)

/* I_LANE_ERROR_STATUS_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_ERROR_STATUS_REG_LES_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_ERROR_STATUS_REG_LES_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_ERROR_STATUS_REG_LES_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_ERROR_STATUS_REG_R0_MASK (0xFFFFFFFEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_ERROR_STATUS_REG_R0_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_ERROR_STATUS_REG_R0_MAX (0x7FFFFFFFU)

/* I_LANE_EQUALIZATION_CONTROL_REG0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R0_MASK (0x0000007FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R0_MAX (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R0_1_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R0_1_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R0_1_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_UPTP0_MASK (0x00000F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_UPTP0_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_UPTP0_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_UPRPH0_MASK (0x00007000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_UPRPH0_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_UPRPH0_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R1_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R1_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R1_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R2_MASK (0x007F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R2_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R2_MAX (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R2_1_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R2_1_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R2_1_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R3_2_MASK (0x7F000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R3_2_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R3_2_MAX (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R3_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R3_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_PCIE_SEC_EXT_CAP_STRUCT_I_LANE_EQUALIZATION_CONTROL_REG0_R3_MAX (0x00000001U)

/* I_VSEC_HEADER_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_PECI_MASK    (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_PECI_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_PECI_MAX     (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_CV_MASK      (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_CV_SHIFT     (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_CV_MAX       (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_NCO_MASK     (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_NCO_SHIFT    (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VSEC_HEADER_REG_NCO_MAX      (0x00000FFFU)

/* I_VENDOR_SPECIFIC_HEADER_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VI_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VI_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VR_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VR_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VR_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VL_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VL_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_HEADER_REG_VL_MAX (0x00000FFFU)

/* I_VENDOR_SPECIFIC_CONTROL_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_VSEC_CIN_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_VSEC_CIN_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_VSEC_CIN_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_HTI_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_HTI_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_HTI_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_VSEC_COUT_MASK (0xFFFFFE00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_VSEC_COUT_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_CONTROL_REG_VSEC_COUT_MAX (0x007FFFFFU)

/* I_VENDOR_SPECIFIC_DATA_REG0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_DATA_REG0_GPD_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_DATA_REG0_GPD_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_VSEC_STRUCT_I_VENDOR_SPECIFIC_DATA_REG0_GPD_MAX (0xFFFFFFFFU)

/* I_PASID_HEADER_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_PECI_MASK  (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_PECI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_PECI_MAX   (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_CV_MASK    (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_CV_SHIFT   (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_CV_MAX     (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_NCO_MASK   (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_NCO_SHIFT  (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_HEADER_REG_NCO_MAX    (0x00000FFFU)

/* I_PASID_CAP_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_RSVD1_MASK    (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_RSVD1_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_RSVD1_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_EXPS_MASK     (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_EXPS_SHIFT    (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_EXPS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PRMS_MASK     (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PRMS_SHIFT    (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PRMS_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_RSVD2_MASK    (0x000000F8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_RSVD2_SHIFT   (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_RSVD2_MAX     (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_MPSW_MASK     (0x00001F00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_MPSW_SHIFT    (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_MPSW_MAX      (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_R15_MASK      (0x0000E000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_R15_SHIFT     (0x0000000DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_R15_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PASE_MASK     (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PASE_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PASE_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_EXPE_MASK     (0x00020000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_EXPE_SHIFT    (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_EXPE_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PRME_MASK     (0x00040000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PRME_SHIFT    (0x00000012U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_PRME_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_R31_MASK      (0xFFF80000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_R31_SHIFT     (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PASID_STRUCT_I_PASID_CAP_REG_R31_MAX       (0x00001FFFU)

/* I_VC_ENH_CAP_HEADER_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_PECID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_PECID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_PECID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_CV_MASK   (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_CV_SHIFT  (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_CV_MAX    (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_NCO_MASK  (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_NCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_ENH_CAP_HEADER_REG_NCO_MAX   (0x00000FFFU)

/* I_PORT_VC_CAP_REG_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_1_EVC_MASK      (0x00000007U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_1_EVC_SHIFT     (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_1_EVC_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_1_R0_MASK       (0xFFFFFFF8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_1_R0_SHIFT      (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_1_R0_MAX        (0x1FFFFFFFU)

/* I_PORT_VC_CAP_REG_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_2_R1_MASK       (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_2_R1_SHIFT      (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CAP_REG_2_R1_MAX        (0xFFFFFFFFU)

/* I_PORT_VC_CTRL_STS_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CTRL_STS_REG_R2_MASK    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CTRL_STS_REG_R2_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_PORT_VC_CTRL_STS_REG_R2_MAX     (0xFFFFFFFFU)

/* I_VC_RES_CAP_REG_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_0_R3_MASK        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_0_R3_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_0_R3_MAX         (0xFFFFFFFFU)

/* I_VC_RES_CTRL_REG_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_TVM0_MASK     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_TVM0_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_TVM0_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_TVM_MASK      (0x000000FEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_TVM_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_TVM_MAX       (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R4_MASK       (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R4_SHIFT      (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R4_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_LPAT_MASK     (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_LPAT_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_LPAT_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_PARS_MASK     (0x000E0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_PARS_SHIFT    (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_PARS_MAX      (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R5_MASK       (0x00F00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R5_SHIFT      (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R5_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_VCI_MASK      (0x07000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_VCI_SHIFT     (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_VCI_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R6_MASK       (0x78000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R6_SHIFT      (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_R6_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_VCEN_MASK     (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_VCEN_SHIFT    (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_0_VCEN_MAX      (0x00000001U)

/* I_VC_RES_STS_REG_0 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_PATS_MASK      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_PATS_SHIFT     (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_PATS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_VCNP_MASK      (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_VCNP_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_VCNP_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_R7_MASK        (0xFFFFFFFCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_R7_SHIFT       (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_0_R7_MAX         (0x3FFFFFFFU)

/* I_VC_RES_CAP_REG_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_1_R3_MASK        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_1_R3_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_1_R3_MAX         (0xFFFFFFFFU)

/* I_VC_RES_CTRL_REG_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_TVM0_MASK     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_TVM0_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_TVM0_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_TVM_MASK      (0x000000FEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_TVM_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_TVM_MAX       (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R4_MASK       (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R4_SHIFT      (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R4_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_LPAT_MASK     (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_LPAT_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_LPAT_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_PARS_MASK     (0x000E0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_PARS_SHIFT    (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_PARS_MAX      (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R5_MASK       (0x00F00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R5_SHIFT      (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R5_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_VCI_MASK      (0x07000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_VCI_SHIFT     (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_VCI_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R6_MASK       (0x78000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R6_SHIFT      (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_R6_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_VCEN_MASK     (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_VCEN_SHIFT    (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_1_VCEN_MAX      (0x00000001U)

/* I_VC_RES_STS_REG_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_PATS_MASK      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_PATS_SHIFT     (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_PATS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_VCNP_MASK      (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_VCNP_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_VCNP_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_R7_MASK        (0xFFFFFFFCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_R7_SHIFT       (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_1_R7_MAX         (0x3FFFFFFFU)

/* I_VC_RES_CAP_REG_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_2_R3_MASK        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_2_R3_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_2_R3_MAX         (0xFFFFFFFFU)

/* I_VC_RES_CTRL_REG_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_TVM0_MASK     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_TVM0_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_TVM0_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_TVM_MASK      (0x000000FEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_TVM_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_TVM_MAX       (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R4_MASK       (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R4_SHIFT      (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R4_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_LPAT_MASK     (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_LPAT_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_LPAT_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_PARS_MASK     (0x000E0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_PARS_SHIFT    (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_PARS_MAX      (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R5_MASK       (0x00F00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R5_SHIFT      (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R5_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_VCI_MASK      (0x07000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_VCI_SHIFT     (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_VCI_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R6_MASK       (0x78000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R6_SHIFT      (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_R6_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_VCEN_MASK     (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_VCEN_SHIFT    (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_2_VCEN_MAX      (0x00000001U)

/* I_VC_RES_STS_REG_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_PATS_MASK      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_PATS_SHIFT     (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_PATS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_VCNP_MASK      (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_VCNP_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_VCNP_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_R7_MASK        (0xFFFFFFFCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_R7_SHIFT       (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_2_R7_MAX         (0x3FFFFFFFU)

/* I_VC_RES_CAP_REG_3 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_3_R3_MASK        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_3_R3_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CAP_REG_3_R3_MAX         (0xFFFFFFFFU)

/* I_VC_RES_CTRL_REG_3 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_TVM0_MASK     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_TVM0_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_TVM0_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_TVM_MASK      (0x000000FEU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_TVM_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_TVM_MAX       (0x0000007FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R4_MASK       (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R4_SHIFT      (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R4_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_LPAT_MASK     (0x00010000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_LPAT_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_LPAT_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_PARS_MASK     (0x000E0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_PARS_SHIFT    (0x00000011U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_PARS_MAX      (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R5_MASK       (0x00F00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R5_SHIFT      (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R5_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_VCI_MASK      (0x07000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_VCI_SHIFT     (0x00000018U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_VCI_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R6_MASK       (0x78000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R6_SHIFT      (0x0000001BU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_R6_MAX        (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_VCEN_MASK     (0x80000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_VCEN_SHIFT    (0x0000001FU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_CTRL_REG_3_VCEN_MAX      (0x00000001U)

/* I_VC_RES_STS_REG_3 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_PATS_MASK      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_PATS_SHIFT     (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_PATS_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_VCNP_MASK      (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_VCNP_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_VCNP_MAX       (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_R7_MASK        (0xFFFFFFFCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_R7_SHIFT       (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_VC_CAP_STRUCT_I_VC_RES_STS_REG_3_R7_MAX         (0x3FFFFFFFU)

/* I_L1_PM_EXT_CAP_HDR */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_PECID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_PECID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_PECID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_CV_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_CV_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_CV_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_NCO_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_NCO_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_EXT_CAP_HDR_NCO_MAX (0x00000FFFU)

/* I_L1_PM_CAP */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PML12SUPP_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PML12SUPP_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PML12SUPP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PML11SUPP_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PML11SUPP_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PML11SUPP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1ASPML12SUPP_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1ASPML12SUPP_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1ASPML12SUPP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1ASPML11SUPP_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1ASPML11SUPP_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1ASPML11SUPP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PMSUPP_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PMSUPP_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PMSUPP_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PRTCMMDRESTRTIME_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PRTCMMDRESTRTIME_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PRTCMMDRESTRTIME_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PRTPVRONSCALE_MASK (0x00030000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PRTPVRONSCALE_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_L1PRTPVRONSCALE_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_R0_MASK       (0x00F80000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_R0_SHIFT      (0x00000013U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CAP_R0_MAX        (0x0000001FU)

/* I_L1_PM_CTRL_1 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1PML12EN_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1PML12EN_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1PML12EN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1PML11EN_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1PML11EN_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1PML11EN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1ASPML12EN_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1ASPML12EN_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1ASPML12EN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1ASPML11EN_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1ASPML11EN_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1ASPML11EN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1CMMDRESTRTIME_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1CMMDRESTRTIME_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1CMMDRESTRTIME_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1THRSHLDVAL_MASK (0x03FF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1THRSHLDVAL_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1THRSHLDVAL_MAX (0x000003FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1THRSHLDSC_MASK (0xE0000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1THRSHLDSC_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_1_L1THRSHLDSC_MAX (0x00000007U)

/* I_L1_PM_CTRL_2 */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_2_L1PWRONSC_MASK (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_2_L1PWRONSC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_2_L1PWRONSC_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_2_L1PWRONVAL_MASK (0x000000F8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_2_L1PWRONVAL_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_L1_PM_CAP_STRUCT_I_L1_PM_CTRL_2_L1PWRONVAL_MAX (0x0000001FU)

/* I_PTM_EXTENDED_CAPABILITY_HEADER_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMCAPID_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMCAPID_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMCAPID_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMCAPVER_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMCAPVER_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMCAPVER_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMNXCAP_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMNXCAP_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_EXTENDED_CAPABILITY_HEADER_REG_PTMNXCAP_MAX (0x00000FFFU)

/* I_PTM_CAPABILITIES_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRQCAP_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRQCAP_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRQCAP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRSCAP_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRSCAP_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRSCAP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRTCAP_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRTCAP_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_PTMRTCAP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_R3_MASK     (0x000000F8U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_R3_SHIFT    (0x00000003U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_R3_MAX      (0x0000001FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_LOCCLKGR_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_LOCCLKGR_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_LOCCLKGR_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_R16_MASK    (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_R16_SHIFT   (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CAPABILITIES_REG_R16_MAX     (0x0000FFFFU)

/* I_PTM_CONTROL_REG */

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_PTMEN_MASK       (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_PTMEN_SHIFT      (0x00000000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_PTMEN_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_RTSEL_MASK       (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_RTSEL_SHIFT      (0x00000001U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_RTSEL_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_R2_MASK          (0x000000FCU)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_R2_SHIFT         (0x00000002U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_R2_MAX           (0x0000003FU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_EFFGRN_MASK      (0x0000FF00U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_EFFGRN_SHIFT     (0x00000008U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_EFFGRN_MAX       (0x000000FFU)

#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_R16_MASK         (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_R16_SHIFT        (0x00000010U)
#define CSL_PCIE_EP_CORE_EP_PF0_I_REGF_PTM_CAP_I_PTM_CONTROL_REG_R16_MAX          (0x0000FFFFU)

/* I_PL_CONFIG_0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LS_MASK         (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LS_SHIFT        (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LS_MAX          (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_NLC_MASK        (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_NLC_SHIFT       (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_NLC_MAX         (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_NS_MASK         (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_NS_SHIFT        (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_NS_MAX          (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LTD_MASK        (0x00000020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LTD_SHIFT       (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LTD_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_APER_MASK       (0x00000040U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_APER_SHIFT      (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_APER_MAX        (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_TSS_MASK        (0x00000080U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_TSS_SHIFT       (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_TSS_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_RFC_MASK        (0x0000FF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_RFC_SHIFT       (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_RFC_MAX         (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_RLID_MASK       (0x00FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_RLID_SHIFT      (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_RLID_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LTSSM_MASK      (0x3F000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LTSSM_SHIFT     (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_LTSSM_MAX       (0x0000003FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_R0_MASK         (0x40000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_R0_SHIFT        (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_R0_MAX          (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_MLE_MASK        (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_MLE_SHIFT       (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_0_REG_MLE_MAX         (0x00000001U)

/* I_PL_CONFIG_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TLI_MASK        (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TLI_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TLI_MAX         (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC1_MASK       (0x0000FF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC1_SHIFT      (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC1_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC2_MASK       (0x00FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC2_SHIFT      (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC2_MAX        (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC3_MASK       (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC3_SHIFT      (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_1_REG_TFC3_MAX        (0x000000FFU)

/* I_DLL_TMR_CONFIG_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_TSRT_MASK    (0x000001FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_TSRT_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_TSRT_MAX     (0x000001FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_R9_MASK      (0x0000FE00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_R9_SHIFT     (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_R9_MAX       (0x0000007FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_RSART_MASK   (0x01FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_RSART_SHIFT  (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_RSART_MAX    (0x000001FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_R25_MASK     (0xFE000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_R25_SHIFT    (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DLL_TMR_CONFIG_REG_R25_MAX      (0x0000007FU)

/* I_RCV_CRED_LIM_0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_PPC_MASK     (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_PPC_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_PPC_MAX      (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_PHC_MASK     (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_PHC_SHIFT    (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_PHC_MAX      (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_NPPC_MASK    (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_NPPC_SHIFT   (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_NPPC_MAX     (0x00000FFFU)

/* I_RCV_CRED_LIM_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_NPHCL_MASK   (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_NPHCL_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_NPHCL_MAX    (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_CPC_MASK     (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_CPC_SHIFT    (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_CPC_MAX      (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_R2_MASK      (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_R2_SHIFT     (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_R2_MAX       (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_CHC_MASK     (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_CHC_SHIFT    (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_CHC_MAX      (0x000000FFU)

/* I_TRANSM_CRED_LIM_0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_PPC_MASK  (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_PPC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_PPC_MAX   (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_PHC_MASK  (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_PHC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_PHC_MAX   (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_NPPC_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_NPPC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_NPPC_MAX  (0x00000FFFU)

/* I_TRANSM_CRED_LIM_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_NPHC_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_NPHC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_NPHC_MAX  (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_CPC_MASK  (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_CPC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_CPC_MAX   (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_R3_MASK   (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_R3_SHIFT  (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_R3_MAX    (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_CHC_MASK  (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_CHC_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_CHC_MAX   (0x000000FFU)

/* I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG_MPUI_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG_MPUI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG_MPUI_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG_MNUI_MASK (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG_MNUI_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_0_REG_MNUI_MAX (0x0000FFFFU)

/* I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG_CUI_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG_CUI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG_CUI_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG_MUI_MASK (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG_MUI_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_UPDATE_INT_CONFIG_1_REG_MUI_MAX (0x0000FFFFU)

/* I_L0S_TIMEOUT_LIMIT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L0S_TIMEOUT_LIMIT_REG_LT_MASK   (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L0S_TIMEOUT_LIMIT_REG_LT_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L0S_TIMEOUT_LIMIT_REG_LT_MAX    (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L0S_TIMEOUT_LIMIT_REG_R4_MASK   (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L0S_TIMEOUT_LIMIT_REG_R4_SHIFT  (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L0S_TIMEOUT_LIMIT_REG_R4_MAX    (0x0000FFFFU)

/* I_TRANSMIT_TLP_COUNT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_COUNT_REG_TTC_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_COUNT_REG_TTC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_COUNT_REG_TTC_MAX  (0xFFFFFFFFU)

/* I_TRANSMIT_TLP_PAYLOAD_DWORD_COUNT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_PAYLOAD_DWORD_COUNT_REG_TTPBC_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_PAYLOAD_DWORD_COUNT_REG_TTPBC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSMIT_TLP_PAYLOAD_DWORD_COUNT_REG_TTPBC_MAX (0xFFFFFFFFU)

/* I_RECEIVE_TLP_COUNT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_COUNT_REG_RTC_MASK  (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_COUNT_REG_RTC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_COUNT_REG_RTC_MAX   (0xFFFFFFFFU)

/* I_RECEIVE_TLP_PAYLOAD_DWORD_COUNT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_PAYLOAD_DWORD_COUNT_REG_RTPDC_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_PAYLOAD_DWORD_COUNT_REG_RTPDC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_TLP_PAYLOAD_DWORD_COUNT_REG_RTPDC_MAX (0xFFFFFFFFU)

/* I_COMPLN_TMOUT_LIM_0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_0_REG_CTL_MASK (0x00FFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_0_REG_CTL_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_0_REG_CTL_MAX  (0x00FFFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_0_REG_R5_MASK  (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_0_REG_R5_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_0_REG_R5_MAX   (0x000000FFU)

/* I_COMPLN_TMOUT_LIM_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_1_REG_CTL_MASK (0x0FFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_1_REG_CTL_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_1_REG_CTL_MAX  (0x0FFFFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_1_REG_R6_MASK  (0xF0000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_1_REG_R6_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_COMPLN_TMOUT_LIM_1_REG_R6_MAX   (0x0000000FU)

/* I_L1_ST_REENTRY_DELAY_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L1_ST_REENTRY_DELAY_REG_L1RD_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L1_ST_REENTRY_DELAY_REG_L1RD_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_L1_ST_REENTRY_DELAY_REG_L1RD_MAX (0xFFFFFFFFU)

/* I_VENDOR_ID_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_ID_REG_VID_MASK          (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_ID_REG_VID_SHIFT         (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_ID_REG_VID_MAX           (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_ID_REG_SVID_MASK         (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_ID_REG_SVID_SHIFT        (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_ID_REG_SVID_MAX          (0x0000FFFFU)

/* I_ASPM_L1_ENTRY_TMOUT_DELAY_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_L1T_MASK (0x000FFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_L1T_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_L1T_MAX (0x000FFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_R7_MASK (0x7FF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_R7_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_R7_MAX (0x000007FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_DISLNRXCHK_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_DISLNRXCHK_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ASPM_L1_ENTRY_TMOUT_DELAY_REG_DISLNRXCHK_MAX (0x00000001U)

/* I_PME_TURNOFF_ACK_DELAY_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_TURNOFF_ACK_DELAY_REG_PTOAD_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_TURNOFF_ACK_DELAY_REG_PTOAD_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_TURNOFF_ACK_DELAY_REG_PTOAD_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_TURNOFF_ACK_DELAY_REG_R7_MASK (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_TURNOFF_ACK_DELAY_REG_R7_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_TURNOFF_ACK_DELAY_REG_R7_MAX (0x0000FFFFU)

/* I_LINKWIDTH_CONTROL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R1_MASK   (0x0001FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R1_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R1_MAX    (0x0001FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG2SC_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG2SC_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG2SC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG3SC_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG3SC_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG3SC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG4SC_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG4SC_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_DSAG4SC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R20_MASK  (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R20_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R20_MAX   (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_EPTLS_MASK (0x03000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_EPTLS_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_EPTLS_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R2_MASK   (0x7C000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R2_SHIFT  (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_R2_MAX    (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_EPLSCRL_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_EPLSCRL_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINKWIDTH_CONTROL_REG_EPLSCRL_MAX (0x00000001U)

/* I_PL_CONFIG_2_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_LK_TRN_MASK     (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_LK_TRN_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_LK_TRN_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_DQMDC_MASK      (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_DQMDC_SHIFT     (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_DQMDC_MAX       (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_R3_MASK         (0xFFFFFFF8U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_R3_SHIFT        (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PL_CONFIG_2_REG_R3_MAX          (0x1FFFFFFFU)

/* I_MULTI_VC_CONROL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_DMAAM_MASK  (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_DMAAM_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_DMAAM_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_WAIT_4_ALL_VC_CC_RDY_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_WAIT_4_ALL_VC_CC_RDY_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_WAIT_4_ALL_VC_CC_RDY_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_RES2_MASK   (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_RES2_SHIFT  (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_RES2_MAX    (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_RES4_MASK   (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_RES4_SHIFT  (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_RES4_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_R31_MASK    (0xFFFFFFE0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_R31_SHIFT   (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_MULTI_VC_CONROL_REG_R31_MAX     (0x07FFFFFFU)

/* I_SRIS_CONTROL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SRIS_CONTROL_REG_SRISE_MASK     (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SRIS_CONTROL_REG_SRISE_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SRIS_CONTROL_REG_SRISE_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SRIS_CONTROL_REG_R31_MASK       (0xFFFFFFFEU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SRIS_CONTROL_REG_R31_SHIFT      (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SRIS_CONTROL_REG_R31_MAX        (0x7FFFFFFFU)

/* I_RCV_CRED_LIM_0_REG_VC1 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_PPC_MASK (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_PPC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_PPC_MAX  (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_PHC_MASK (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_PHC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_PHC_MAX  (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_NPPC_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_NPPC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC1_NPPC_MAX (0x00000FFFU)

/* I_RCV_CRED_LIM_1_REG_VC1 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_NPHCL_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_NPHCL_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_NPHCL_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_CPC_MASK (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_CPC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_CPC_MAX  (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_R2_MASK  (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_R2_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_R2_MAX   (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_CHC_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_CHC_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC1_CHC_MAX  (0x000000FFU)

/* I_TRANSM_CRED_LIM_0_REG_VC1 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_PPC_MASK (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_PPC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_PPC_MAX (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_PHC_MASK (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_PHC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_PHC_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_NPPC_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_NPPC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC1_NPPC_MAX (0x00000FFFU)

/* I_TRANSM_CRED_LIM_1_REG_VC1 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_NPHC_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_NPHC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_NPHC_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_CPC_MASK (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_CPC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_CPC_MAX (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_R3_MASK (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_R3_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_R3_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_CHC_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_CHC_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC1_CHC_MAX (0x000000FFU)

/* I_RCV_CRED_LIM_0_REG_VC2 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_PPC_MASK (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_PPC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_PPC_MAX  (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_PHC_MASK (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_PHC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_PHC_MAX  (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_NPPC_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_NPPC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC2_NPPC_MAX (0x00000FFFU)

/* I_RCV_CRED_LIM_1_REG_VC2 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_NPHCL_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_NPHCL_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_NPHCL_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_CPC_MASK (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_CPC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_CPC_MAX  (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_R2_MASK  (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_R2_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_R2_MAX   (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_CHC_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_CHC_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC2_CHC_MAX  (0x000000FFU)

/* I_TRANSM_CRED_LIM_0_REG_VC2 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_PPC_MASK (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_PPC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_PPC_MAX (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_PHC_MASK (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_PHC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_PHC_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_NPPC_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_NPPC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC2_NPPC_MAX (0x00000FFFU)

/* I_TRANSM_CRED_LIM_1_REG_VC2 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_NPHC_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_NPHC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_NPHC_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_CPC_MASK (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_CPC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_CPC_MAX (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_R3_MASK (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_R3_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_R3_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_CHC_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_CHC_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC2_CHC_MAX (0x000000FFU)

/* I_RCV_CRED_LIM_0_REG_VC3 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_PPC_MASK (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_PPC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_PPC_MAX  (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_PHC_MASK (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_PHC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_PHC_MAX  (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_NPPC_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_NPPC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_0_REG_VC3_NPPC_MAX (0x00000FFFU)

/* I_RCV_CRED_LIM_1_REG_VC3 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_NPHCL_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_NPHCL_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_NPHCL_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_CPC_MASK (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_CPC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_CPC_MAX  (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_R2_MASK  (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_R2_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_R2_MAX   (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_CHC_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_CHC_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RCV_CRED_LIM_1_REG_VC3_CHC_MAX  (0x000000FFU)

/* I_TRANSM_CRED_LIM_0_REG_VC3 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_PPC_MASK (0x00000FFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_PPC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_PPC_MAX (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_PHC_MASK (0x000FF000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_PHC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_PHC_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_NPPC_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_NPPC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_0_REG_VC3_NPPC_MAX (0x00000FFFU)

/* I_TRANSM_CRED_LIM_1_REG_VC3 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_NPHC_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_NPHC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_NPHC_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_CPC_MASK (0x000FFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_CPC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_CPC_MAX (0x00000FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_R3_MASK (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_R3_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_R3_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_CHC_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_CHC_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_TRANSM_CRED_LIM_1_REG_VC3_CHC_MAX (0x000000FFU)

/* I_FC_INIT_DELAY_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_FC_INIT_DELAY_REG_FCINITDLY_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_FC_INIT_DELAY_REG_FCINITDLY_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_FC_INIT_DELAY_REG_FCINITDLY_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_FC_INIT_DELAY_REG_R4_MASK       (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_FC_INIT_DELAY_REG_R4_SHIFT      (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_FC_INIT_DELAY_REG_R4_MAX        (0x0000FFFFU)

/* I_SHDW_HDR_LOG_0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_0_REG_SHDW_HDR_LOG_0_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_0_REG_SHDW_HDR_LOG_0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_0_REG_SHDW_HDR_LOG_0_MAX (0xFFFFFFFFU)

/* I_SHDW_HDR_LOG_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_1_REG_SHDW_HDR_LOG_1_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_1_REG_SHDW_HDR_LOG_1_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_1_REG_SHDW_HDR_LOG_1_MAX (0xFFFFFFFFU)

/* I_SHDW_HDR_LOG_2_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_2_REG_SHDW_HDR_LOG_2_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_2_REG_SHDW_HDR_LOG_2_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_2_REG_SHDW_HDR_LOG_2_MAX (0xFFFFFFFFU)

/* I_SHDW_HDR_LOG_3_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_3_REG_SHDW_HDR_LOG_3_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_3_REG_SHDW_HDR_LOG_3_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_HDR_LOG_3_REG_SHDW_HDR_LOG_3_MAX (0xFFFFFFFFU)

/* I_SHDW_FUNC_NUM_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_FUNC_NUM_REG_SHDW_FUNC_NUM_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_FUNC_NUM_REG_SHDW_FUNC_NUM_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_FUNC_NUM_REG_SHDW_FUNC_NUM_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_FUNC_NUM_REG_R0_MASK       (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_FUNC_NUM_REG_R0_SHIFT      (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_FUNC_NUM_REG_R0_MAX        (0x00FFFFFFU)

/* I_SHDW_UR_ERR_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_P_UR_ERR_MASK   (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_P_UR_ERR_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_P_UR_ERR_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_NP_UR_ERR_MASK  (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_NP_UR_ERR_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_NP_UR_ERR_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_R0_MASK         (0xFFFFFFFCU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_R0_SHIFT        (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_SHDW_UR_ERR_REG_R0_MAX          (0x3FFFFFFFU)

/* I_PM_CLK_FREQUENCY_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PM_CLK_FREQUENCY_REG_PMCLKFRQ_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PM_CLK_FREQUENCY_REG_PMCLKFRQ_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PM_CLK_FREQUENCY_REG_PMCLKFRQ_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PM_CLK_FREQUENCY_REG_R0_MASK    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PM_CLK_FREQUENCY_REG_R0_SHIFT   (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PM_CLK_FREQUENCY_REG_R0_MAX     (0x00FFFFFFU)

/* I_DEBUG_DLLP_COUNT_GEN1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN1_REG_DLLPCNT1_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN1_REG_DLLPCNT1_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN1_REG_DLLPCNT1_MAX (0xFFFFFFFFU)

/* I_DEBUG_DLLP_COUNT_GEN2_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN2_REG_DLLPCNT2_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN2_REG_DLLPCNT2_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN2_REG_DLLPCNT2_MAX (0xFFFFFFFFU)

/* I_DEBUG_DLLP_COUNT_GEN3_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN3_REG_DLLPCNT3_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN3_REG_DLLPCNT3_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_DLLP_COUNT_GEN3_REG_DLLPCNT3_MAX (0xFFFFFFFFU)

/* I_VENDOR_DEFINED_MESSAGE_TAG_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_DEFINED_MESSAGE_TAG_REG_VDMTAG_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_DEFINED_MESSAGE_TAG_REG_VDMTAG_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_VENDOR_DEFINED_MESSAGE_TAG_REG_VDMTAG_MAX (0x000000FFU)

/* I_NEGOTIATED_LANE_MAP_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_NLM_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_NLM_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_NLM_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_R70_MASK (0x0000FFFEU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_R70_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_R70_MAX (0x00007FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_LRS_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_LRS_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_LRS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_R71_MASK (0xFFFE0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_R71_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_NEGOTIATED_LANE_MAP_REG_R71_MAX (0x00007FFFU)

/* I_RECEIVE_FTS_COUNT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_RFC5S_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_RFC5S_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_RFC5S_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_RFC8S_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_RFC8S_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_RFC8S_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_R16_MASK  (0x00FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_R16_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_R16_MAX   (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_R24_MASK  (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_R24_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RECEIVE_FTS_COUNT_REG_R24_MAX   (0x000000FFU)

/* I_DEBUG_MUX_CONTROL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_MS_MASK   (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_MS_SHIFT  (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_MS_MAX    (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R6_MASK   (0x00000060U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R6_SHIFT  (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R6_MAX    (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R77_MASK  (0x00000080U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R77_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R77_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DIDBOC_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DIDBOC_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DIDBOC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_MSIVCMS_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_MSIVCMS_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_MSIVCMS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DRXNPSP_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DRXNPSP_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DRXNPSP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1111_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1111_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1111_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1212_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1212_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1212_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1313_MASK (0x00002000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1313_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_R1313_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DSSPLM_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DSSPLM_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DSSPLM_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_FDS_MASK  (0x00008000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_FDS_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_FDS_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_AWRPRI_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_AWRPRI_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_AWRPRI_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_HPRSUPP_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_HPRSUPP_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_HPRSUPP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DOASFC_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DOASFC_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DOASFC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DIOAEFC_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DIOAEFC_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DIOAEFC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DCIVMC_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DCIVMC_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DCIVMC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DSHEC_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DSHEC_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DSHEC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DLRFE_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DLRFE_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DLRFE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DLUC_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DLUC_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DLUC_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_EFLT_MASK (0x01000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_EFLT_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_EFLT_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_ESPC_MASK (0x02000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_ESPC_SHIFT (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_ESPC_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_IEDPPE_MASK (0x04000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_IEDPPE_SHIFT (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_IEDPPE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DGLUS_MASK (0x08000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DGLUS_SHIFT (0x0000001BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DGLUS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DEI_MASK  (0x10000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DEI_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DEI_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DFCUT_MASK (0x20000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DFCUT_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DFCUT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DOC_MASK  (0x40000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DOC_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_DOC_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_EFSRTCA_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_EFSRTCA_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_REG_EFSRTCA_MAX (0x00000001U)

/* I_LOCAL_ERROR_STATUS_REGISTER */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PRFPE_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PRFPE_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PRFPE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CRFPE_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CRFPE_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CRFPE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RRPE_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RRPE_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RRPE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PRFO_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PRFO_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PRFO_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CRFO_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CRFO_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CRFO_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RT_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RT_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RTR_MASK (0x00000040U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RTR_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_RTR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PE_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PE_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_PE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MTR_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MTR_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MTR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_UCR_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_UCR_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_UCR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_FCE_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_FCE_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_FCE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CT_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CT_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_CT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R12_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R12_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R12_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R13_MASK (0x0001E000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R13_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R13_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_EEPE_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_EEPE_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_EEPE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_UTC_MASK (0x00040000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_UTC_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_UTC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MMVC_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MMVC_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MMVC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R22_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R22_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R22_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_HAWCD_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_HAWCD_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_HAWCD_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R23_1_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R23_1_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R23_1_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R24_MASK (0x01800000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R24_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R24_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MSIXMSKST_MASK (0x02000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MSIXMSKST_SHIFT (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_MSIXMSKST_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R27_MASK (0x0C000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R27_SHIFT (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_R27_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXIMASTER_DIB_ER_UN_MASK (0x10000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXIMASTER_DIB_ER_UN_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXIMASTER_DIB_ER_UN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXIMASTER_RFIFO_ER_UN_MASK (0x20000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXIMASTER_RFIFO_ER_UN_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXIMASTER_RFIFO_ER_UN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXISLAVE_WFIFO_ER_UN_MASK (0x40000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXISLAVE_WFIFO_ER_UN_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_AXISLAVE_WFIFO_ER_UN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_REORDER_ER_UN_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_REORDER_ER_UN_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_REGISTER_REORDER_ER_UN_MAX (0x00000001U)

/* I_LOCAL_INTRPT_MASK_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PRFPE_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PRFPE_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PRFPE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CRFPE_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CRFPE_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CRFPE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RRPE_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RRPE_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RRPE_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PRFO_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PRFO_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PRFO_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CRFO_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CRFO_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CRFO_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RT_MASK   (0x00000020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RT_SHIFT  (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RT_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RTR_MASK  (0x00000040U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RTR_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_RTR_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PE_MASK   (0x00000080U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PE_SHIFT  (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_PE_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MTR_MASK  (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MTR_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MTR_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_UCR_MASK  (0x00000200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_UCR_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_UCR_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_FCE_MASK  (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_FCE_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_FCE_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CT_MASK   (0x00000800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CT_SHIFT  (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_CT_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R12_MASK  (0x00001000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R12_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R12_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R13_MASK  (0x0001E000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R13_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R13_MAX   (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_EEPE_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_EEPE_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_EEPE_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_UTC_MASK  (0x00040000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_UTC_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_UTC_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MMVC_MASK (0x00080000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MMVC_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MMVC_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R45_MASK  (0x00100000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R45_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R45_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_HAWCD_MASK (0x00200000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_HAWCD_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_HAWCD_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R23_1_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R23_1_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R23_1_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R24_MASK  (0x01800000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R24_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R24_MAX   (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MSIXMSK_MASK (0x02000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MSIXMSK_SHIFT (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_MSIXMSK_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R27_MASK  (0x0C000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R27_SHIFT (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_R27_MAX   (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXIMASTER_DIB_ER_UN_MASK (0x10000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXIMASTER_DIB_ER_UN_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXIMASTER_DIB_ER_UN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXIMASTER_RFIFO_ER_UN_MASK (0x20000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXIMASTER_RFIFO_ER_UN_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXIMASTER_RFIFO_ER_UN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXISLAVE_WFIFO_ER_UN_MASK (0x40000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXISLAVE_WFIFO_ER_UN_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_AXISLAVE_WFIFO_ER_UN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_REORDER_ER_UN_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_REORDER_ER_UN_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_REG_REORDER_ER_UN_MAX (0x00000001U)

/* I_LCRC_ERR_COUNT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LCRC_ERR_COUNT_REG_LEC_MASK     (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LCRC_ERR_COUNT_REG_LEC_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LCRC_ERR_COUNT_REG_LEC_MAX      (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LCRC_ERR_COUNT_REG_R11_MASK     (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LCRC_ERR_COUNT_REG_R11_SHIFT    (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LCRC_ERR_COUNT_REG_R11_MAX      (0x0000FFFFU)

/* I_ECC_CORR_ERR_COUNT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_PFRCER_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_PFRCER_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_PFRCER_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_SFRCER_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_SFRCER_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_SFRCER_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_RRCER_MASK (0x00FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_RRCER_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_RRCER_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_R31_2_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_R31_2_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_R31_2_MAX (0x000000FFU)

/* I_LTR_SNOOP_LAT_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLV_MASK     (0x000003FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLV_SHIFT    (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLV_MAX      (0x000003FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLS_MASK     (0x00001C00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLS_SHIFT    (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLS_MAX      (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_R12_MASK      (0x00006000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_R12_SHIFT     (0x0000000DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_R12_MAX       (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLR_MASK     (0x00008000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLR_SHIFT    (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_NSLR_MAX      (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SLV_MASK      (0x03FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SLV_SHIFT     (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SLV_MAX       (0x000003FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SLS_MASK      (0x1C000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SLS_SHIFT     (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SLS_MAX       (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_R13_MASK      (0x60000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_R13_SHIFT     (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_R13_MAX       (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SL_MASK       (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SL_SHIFT      (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_SNOOP_LAT_REG_SL_MAX        (0x00000001U)

/* I_LTR_MSG_GEN_CTL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_MLI_MASK    (0x000003FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_MLI_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_MLI_MAX     (0x000003FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_SLM_MASK    (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_SLM_SHIFT   (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_SLM_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_TMLMET_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_TMLMET_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_TMLMET_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_TMFPSC_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_TMFPSC_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LTR_MSG_GEN_CTL_REG_TMFPSC_MAX  (0x00000001U)

/* I_PME_SERVICE_TIMEOUT_DELAY_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_PSTD_MASK (0x000FFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_PSTD_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_PSTD_MAX (0x000FFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_DPMOPS_MASK (0x00100000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_DPMOPS_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_DPMOPS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_R21_MASK (0xFFE00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_R21_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PME_SERVICE_TIMEOUT_DELAY_REG_R21_MAX (0x000007FFU)

/* I_ROOT_PORT_REQUESTOR_ID_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ROOT_PORT_REQUESTOR_ID_REG_RPRI_MASK (0x0000FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ROOT_PORT_REQUESTOR_ID_REG_RPRI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ROOT_PORT_REQUESTOR_ID_REG_RPRI_MAX (0x0000FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ROOT_PORT_REQUESTOR_ID_REG_R0_MASK (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ROOT_PORT_REQUESTOR_ID_REG_R0_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ROOT_PORT_REQUESTOR_ID_REG_R0_MAX (0x0000FFFFU)

/* I_EP_BUS_DEVICE_NUMBER_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_EPDN_MASK (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_EPDN_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_EPDN_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_R5_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_R5_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_R5_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_EPBN_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_EPBN_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_EPBN_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_R16_MASK (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_R16_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EP_BUS_DEVICE_NUMBER_REG_R16_MAX (0x0000FFFFU)

/* I_DEBUG_MUX_CONTROL_2_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DLFFS_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DLFFS_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DLFFS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_EXTSNP_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_EXTSNP_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_EXTSNP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DISSDSCHK_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DISSDSCHK_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DISSDSCHK_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_ENLNCHK_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_ENLNCHK_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_ENLNCHK_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R4_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R4_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R4_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_BLKALNCHK_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_BLKALNCHK_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_BLKALNCHK_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_BLKALNWIN_MASK (0x000000C0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_BLKALNWIN_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_BLKALNWIN_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R8_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R8_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R8_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_MSIPIMS_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_MSIPIMS_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_MSIPIMS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_PSNADV_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_PSNADV_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_PSNADV_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_CMPTOADV_MASK (0x00000800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_CMPTOADV_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_CMPTOADV_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_AXINPSPEN_RSVD_MASK (0x00001000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_AXINPSPEN_RSVD_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_AXINPSPEN_RSVD_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_MAXNPREQ_MASK (0x007FE000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_MAXNPREQ_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_MAXNPREQ_MAX (0x000003FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_VARCCLKEN_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_VARCCLKEN_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_VARCCLKEN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R24_MASK (0x01000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R24_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R24_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R25_MASK (0x02000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R25_SHIFT (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R25_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R26_MASK (0x04000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R26_SHIFT (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R26_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DTAE2EP_MASK (0x08000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DTAE2EP_SHIFT (0x0000001BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DTAE2EP_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DFLRTRB_MASK (0x10000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DFLRTRB_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DFLRTRB_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DRXRMFR_MASK (0x20000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DRXRMFR_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_DRXRMFR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R30_MASK (0x40000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R30_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_R30_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_HRLT_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_HRLT_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_2_REG_HRLT_MAX (0x00000001U)

/* I_PHY_STATUS_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_TLPPHYER_MASK  (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_TLPPHYER_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_TLPPHYER_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSASKP_MASK    (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSASKP_SHIFT   (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSASKP_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_ILOSEDS_MASK   (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_ILOSEDS_SHIFT  (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_ILOSEDS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_DATEDS_MASK    (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_DATEDS_SHIFT   (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_DATEDS_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSWOEDS_MASK   (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSWOEDS_SHIFT  (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSWOEDS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_G3FRERR_MASK   (0x00000020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_G3FRERR_SHIFT  (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_G3FRERR_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSAFSDS_MASK   (0x00000040U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSAFSDS_SHIFT  (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_OSAFSDS_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_INVSYNHR_MASK  (0x00000080U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_INVSYNHR_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_INVSYNHR_MAX   (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_LOSBLKALN_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_LOSBLKALN_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_LOSBLKALN_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_R31_MASK       (0xFFFFFE00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_R31_SHIFT      (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PHY_STATUS_1_REG_R31_MAX        (0x007FFFFFU)

/* I_DEBUG_MUX_CONTROL_3_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R0_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R0_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R0_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R1_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R1_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R1_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_DLTE_MASK (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_DLTE_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_DLTE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_DSDES_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_DSDES_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_DSDES_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R4_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R4_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R4_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R2_MASK (0xFFFFFFE0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R2_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_DEBUG_MUX_CONTROL_3_REG_R2_MAX  (0x07FFFFFFU)

/* I_PF_0_BAR_CONFIG_0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0A_MASK (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0A_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0A_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0C_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0C_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR0C_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1A_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1A_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1A_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1C_MASK (0x0000E000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1C_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR1C_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2A_MASK (0x001F0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2A_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2A_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2C_MASK (0x00E00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2C_SHIFT (0x00000015U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR2C_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3A_MASK (0x1F000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3A_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3A_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3C_MASK (0xE0000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3C_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_0_REG_BAR3C_MAX (0x00000007U)

/* I_PF_0_BAR_CONFIG_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR4A_MASK (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR4A_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR4A_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR4C_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR4C_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR4C_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR5A_MASK (0x00001F00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR5A_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR5A_MAX (0x0000001FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR5C_MASK (0x0000E000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR5C_SHIFT (0x0000000DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_BAR5C_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_R16_MASK  (0x00FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_R16_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_R16_MAX   (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_R24_MASK  (0x7F000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_R24_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_R24_MAX   (0x0000007FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_ERBC_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_ERBC_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_0_BAR_CONFIG_1_REG_ERBC_MAX  (0x00000001U)

/* I_PF_CONFIG_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_CONFIG_REG_F0E_MASK          (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_CONFIG_REG_F0E_SHIFT         (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_CONFIG_REG_F0E_MAX           (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_CONFIG_REG_R_MASK            (0xFFFFFFFEU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_CONFIG_REG_R_SHIFT           (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PF_CONFIG_REG_R_MAX             (0x7FFFFFFFU)

/* I_GEN3_DEFAULT_PRESET_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_GDTXP_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_GDTXP_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_GDTXP_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_GDRXPH_MASK (0x00000070U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_GDRXPH_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_GDRXPH_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_R7_MASK (0x00000080U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_R7_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_R7_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_S8GPR_MASK (0x0007FF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_S8GPR_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_S8GPR_MAX (0x000007FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_R31_MASK (0xFFF80000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_R31_SHIFT (0x00000013U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_DEFAULT_PRESET_REG_R31_MAX (0x00001FFFU)

/* I_GEN3_LINK_EQ_TIMEOUT_2MS_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_LEQT2MS_MASK (0x0FFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_LEQT2MS_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_LEQT2MS_MAX (0x0FFFFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_R28_MASK (0x10000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_R28_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_R28_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_RXEQABD_MASK (0x20000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_RXEQABD_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_RXEQABD_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_RXEQABM_MASK (0xC0000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_RXEQABM_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_TIMEOUT_2MS_REG_RXEQABM_MAX (0x00000003U)

/* I_PIPE_FIFO_LATENCY_CTRL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPTFCE_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPTFCE_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPTFCE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPRFLR_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPRFLR_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPRFLR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_R14_MASK (0x00007FFCU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_R14_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_R14_MAX (0x00001FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPTFWF_MASK (0x00008000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPTFWF_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_DPTFWF_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_R31_MASK (0xFFFF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_R31_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PIPE_FIFO_LATENCY_CTRL_REG_R31_MAX (0x0000FFFFU)

/* I_GEN3_LINK_EQ_CTRL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_MXECC_MASK (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_MXECC_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_MXECC_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES3_MASK (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES3_SHIFT (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES3_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_EP8GRE_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_EP8GRE_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_EP8GRE_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES5_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES5_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES5_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES6_MASK (0x000000C0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES6_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES6_MAX  (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_QG8GT_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_QG8GT_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_QG8GT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES9_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES9_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES9_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES10_MASK (0x00000C00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES10_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES10_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_MX8GERL_MASK (0x0000F000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_MX8GERL_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_MX8GERL_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES19_MASK (0x000F0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES19_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES19_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES20_MASK (0xFFF00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES20_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_CTRL_REG_RES20_MAX (0x00000FFFU)

/* I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXPR_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXPR_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXPR_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXPRV_MASK (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXPRV_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXPRV_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_RES75_MASK (0x000000E0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_RES75_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_RES75_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXCO_MASK (0x03FFFF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXCO_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_LEQTXCO_MAX (0x0003FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_RES3126_MASK (0xFC000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_RES3126_SHIFT (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_GEN3_LINK_EQ_DEBUG_STATUS_REG_LANE0_RES3126_MAX (0x0000003FU)

/* I_ECC_CORR_ERR_COUNT_REG_AXI */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_REORDER_CER_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_REORDER_CER_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_REORDER_CER_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_SLAVE_WFIFO_CER_MASK (0x0000FF00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_SLAVE_WFIFO_CER_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_SLAVE_WFIFO_CER_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_MASTER_RFIFO_CER_MASK (0x00FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_MASTER_RFIFO_CER_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_MASTER_RFIFO_CER_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_MASTER_DIB_CER_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_MASTER_DIB_CER_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_ECC_CORR_ERR_COUNT_REG_AXI_AXI_MASTER_DIB_CER_MAX (0x000000FFU)

/* LOW_POWER_DEBUG_AND_CONTROL0 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1XDELAY_MASK (0x00FFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1XDELAY_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1XDELAY_MAX (0x00FFFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1DBRI_MASK (0x01000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1DBRI_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1DBRI_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1EM_MASK (0x06000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1EM_SHIFT (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1EM_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1DLEUP_MASK (0x08000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1DLEUP_SHIFT (0x0000001BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL0_L1DLEUP_MAX (0x00000001U)

/* LOW_POWER_DEBUG_AND_CONTROL1 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL1_L1ER_MASK (0x000000FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL1_L1ER_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL1_L1ER_MAX (0x000000FFU)

/* LOW_POWER_DEBUG_AND_CONTROL2 */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1TWROI_MASK (0x00FFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1TWROI_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1TWROI_MAX (0x00FFFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1EOC_MASK (0x02000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1EOC_SHIFT (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1EOC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1ERC_MASK (0x04000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1ERC_SHIFT (0x0000001AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1ERC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1PS_MASK (0x08000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1PS_SHIFT (0x0000001BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1PS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1TROW_MASK (0x10000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1TROW_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1TROW_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1DAET_MASK (0x20000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1DAET_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1DAET_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1CSC_MASK (0x40000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1CSC_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1CSC_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1UPACR_MASK (0x80000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1UPACR_SHIFT (0x0000001FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_LOW_POWER_DEBUG_AND_CONTROL2_L1UPACR_MAX (0x00000001U)

/* TL_INTERNAL_CONTROL */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_TL_INTERNAL_CONTROL_ECFLR_MASK    (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_TL_INTERNAL_CONTROL_ECFLR_SHIFT   (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_TL_INTERNAL_CONTROL_ECFLR_MAX     (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_TL_INTERNAL_CONTROL_DOOC_MASK     (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_TL_INTERNAL_CONTROL_DOOC_SHIFT    (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_TL_INTERNAL_CONTROL_DOOC_MAX      (0x00000001U)

/* I_LOCAL_ERROR_STATUS_2_REGISTER */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R01_MASK (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R01_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R01_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R23_MASK (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R23_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R23_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R4_MASK (0x000000F0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R4_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R4_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_NFTSTOS_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_NFTSTOS_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_NFTSTOS_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_PTMCNTAINV_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_PTMCNTAINV_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_PTMCNTAINV_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R10_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R10_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R10_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R13_11_MASK (0x00003800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R13_11_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R13_11_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_LEQRQIN_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_LEQRQIN_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_LEQRQIN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R31_MASK (0xFFFF8000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R31_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_ERROR_STATUS_2_REGISTER_R31_MAX (0x0001FFFFU)

/* I_LOCAL_INTRPT_MASK_2_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R01_MASK (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R01_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R01_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R23_MASK (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R23_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R23_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R4_MASK (0x000000F0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R4_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R4_MAX  (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_NFTSTOM_MASK (0x00000100U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_NFTSTOM_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_NFTSTOM_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_PCAIM_MASK (0x00000200U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_PCAIM_SHIFT (0x00000009U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_PCAIM_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R10_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R10_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R10_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R13_11_MASK (0x00003800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R13_11_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R13_11_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_LEQRQINM_MASK (0x00004000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_LEQRQINM_SHIFT (0x0000000EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_LEQRQINM_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R31_MASK (0xFFFF8000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R31_SHIFT (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LOCAL_INTRPT_MASK_2_REG_R31_MAX (0x0001FFFFU)

/* I_LD_CTRL */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_LDTIMER_MASK            (0x00FFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_LDTIMER_SHIFT           (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_LDTIMER_MAX             (0x00FFFFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_AUTO_EN_MASK            (0x01000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_AUTO_EN_SHIFT           (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_AUTO_EN_MAX             (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_R7_MASK                 (0xFE000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_R7_SHIFT                (0x00000019U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LD_CTRL_R7_MAX                  (0x0000007FU)

/* RX_ELEC_IDLE_FILTER_CONTROL */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLD_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLD_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLD_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_RSVGFLD_MASK (0x0000FFFEU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_RSVGFLD_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_RSVGFLD_MAX (0x00007FFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLCC_MASK (0x00FF0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLCC_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLCC_MAX (0x000000FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLCP_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLCP_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_RX_ELEC_IDLE_FILTER_CONTROL_GFLCP_MAX (0x000000FFU)

/* I_PTM_LOCAL_CONTROL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRQM_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRQM_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRQM_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRQEN_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRQEN_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRQEN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES2_MASK (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES2_SHIFT (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES2_MAX  (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRFRSC_MASK (0x000000F0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRFRSC_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRFRSC_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRFRVL_MASK (0x00000F00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRFRVL_SHIFT (0x00000008U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRFRVL_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRINT_MASK (0x0000F000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRINT_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRINT_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRSM_MASK (0x00010000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRSM_SHIFT (0x00000010U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRSM_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRSEN_MASK (0x00020000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRSEN_SHIFT (0x00000011U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_PTMRSEN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES18_MASK (0x07FC0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES18_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES18_MAX (0x000001FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_INVPTMCNT_MASK (0x08000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_INVPTMCNT_SHIFT (0x0000001BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_INVPTMCNT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_DAINVCNT_MASK (0x10000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_DAINVCNT_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_DAINVCNT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES29_MASK (0xE0000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES29_SHIFT (0x0000001DU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_CONTROL_REG_RES29_MAX (0x00000007U)

/* I_PTM_LOCAL_STATUS_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_STATUS_REG_PTMCNST_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_STATUS_REG_PTMCNST_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_STATUS_REG_PTMCNST_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_STATUS_REG_RES3_MASK  (0xFFFFFFF0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_STATUS_REG_RES3_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LOCAL_STATUS_REG_RES3_MAX   (0x0FFFFFFFU)

/* I_PTM_LATENCY_PARAMETERS_INDEX_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_INDEX_REG_PTMLATIN_MASK (0x0000000FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_INDEX_REG_PTMLATIN_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_INDEX_REG_PTMLATIN_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_INDEX_REG_RES4_MASK (0xFFFFFFF0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_INDEX_REG_RES4_SHIFT (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_INDEX_REG_RES4_MAX (0x0FFFFFFFU)

/* I_PTM_LATENCY_PARAMETERS_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_PTMTXLAT_MASK (0x000003FFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_PTMTXLAT_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_PTMTXLAT_MAX (0x000003FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_PTMRXLAT_MASK (0x000FFC00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_PTMRXLAT_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_PTMRXLAT_MAX (0x000003FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_RES20_MASK (0x00F00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_RES20_SHIFT (0x00000014U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_RES20_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_TXDLTUN_MASK (0x0F000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_TXDLTUN_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_TXDLTUN_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_RXDLTUN_MASK (0xF0000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_RXDLTUN_SHIFT (0x0000001CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_LATENCY_PARAMETERS_REG_RXDLTUN_MAX (0x0000000FU)

/* I_PTM_CONTEXT_1_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_1_REG_PTMT1T2_MASK  (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_1_REG_PTMT1T2_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_1_REG_PTMT1T2_MAX   (0xFFFFFFFFU)

/* I_PTM_CONTEXT_2_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_2_REG_PTMT1T2U_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_2_REG_PTMT1T2U_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_2_REG_PTMT1T2U_MAX  (0xFFFFFFFFU)

/* I_PTM_CONTEXT_3_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_3_REG_PTMT4T3_MASK  (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_3_REG_PTMT4T3_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_3_REG_PTMT4T3_MAX   (0xFFFFFFFFU)

/* I_PTM_CONTEXT_4_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_4_REG_PTMT4T3U_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_4_REG_PTMT4T3U_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_4_REG_PTMT4T3U_MAX  (0xFFFFFFFFU)

/* I_PTM_CONTEXT_5_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_5_REG_PTMT1KT2K_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_5_REG_PTMT1KT2K_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_5_REG_PTMT1KT2K_MAX (0xFFFFFFFFU)

/* I_PTM_CONTEXT_6_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_6_REG_PTMT1KT2KU_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_6_REG_PTMT1KT2KU_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_6_REG_PTMT1KT2KU_MAX (0xFFFFFFFFU)

/* I_PTM_CONTEXT_7_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_7_REG_PTMT4KT3K_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_7_REG_PTMT4KT3K_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_7_REG_PTMT4KT3K_MAX (0xFFFFFFFFU)

/* I_PTM_CONTEXT_8_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_8_REG_PTMT4KT3KU_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_8_REG_PTMT4KT3KU_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_8_REG_PTMT4KT3KU_MAX (0xFFFFFFFFU)

/* I_PTM_CONTEXT_9_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_9_REG_PTMT3MT2_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_9_REG_PTMT3MT2_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_9_REG_PTMT3MT2_MAX  (0xFFFFFFFFU)

/* I_PTM_CONTEXT_10_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_10_REG_PTMMSTT1T_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_10_REG_PTMMSTT1T_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_10_REG_PTMMSTT1T_MAX (0xFFFFFFFFU)

/* I_PTM_CONTEXT_11_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_11_REG_PTMMSTT1TU_MASK (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_11_REG_PTMMSTT1TU_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_PTM_CONTEXT_11_REG_PTMMSTT1TU_MAX (0xFFFFFFFFU)

/* I_EQ_DEBUG_MON_CONTROL_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CLRCAPT_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CLRCAPT_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CLRCAPT_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTLNSEL_MASK (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTLNSEL_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTLNSEL_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTPHSEL_MASK (0x00000060U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTPHSEL_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTPHSEL_MAX (0x00000003U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTSPDSEL_MASK (0x00000380U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTSPDSEL_SHIFT (0x00000007U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTSPDSEL_MAX (0x00000007U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTBEH_MASK (0x00000400U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTBEH_SHIFT (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_CAPTBEH_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_R1_MASK (0xFFFFF800U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_R1_SHIFT (0x0000000BU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_CONTROL_REG_R1_MAX (0x001FFFFFU)

/* I_EQ_DEBUG_MON_STATUS0_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_LCLFS_MASK (0x0000003FU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_LCLFS_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_LCLFS_MAX (0x0000003FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_LCLLF_MASK (0x00000FC0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_LCLLF_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_LCLLF_MAX (0x0000003FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_REMFS_MASK (0x0003F000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_REMFS_SHIFT (0x0000000CU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_REMFS_MAX (0x0000003FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_REMLF_MASK (0x00FC0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_REMLF_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_REMLF_MAX (0x0000003FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_R1_MASK (0xFF000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_R1_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS0_REG_R1_MAX (0x000000FFU)

/* I_EQ_DEBUG_MON_STATUS_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQCOEFF_MASK (0x0003FFFFU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQCOEFF_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQCOEFF_MAX (0x0003FFFFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPRE_MASK (0x003C0000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPRE_SHIFT (0x00000012U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPRE_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPREVD_MASK (0x00400000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPREVD_SHIFT (0x00000016U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPREVD_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_COEFFREJ_MASK (0x00800000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_COEFFREJ_SHIFT (0x00000017U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_COEFFREJ_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_DIRFED_MASK (0x3F000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_DIRFED_SHIFT (0x00000018U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_DIRFED_MAX (0x0000003FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPHASE_MASK (0xC0000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPHASE_SHIFT (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_EQ_DEBUG_MON_STATUS_REG_EQPHASE_MAX (0x00000003U)

/* I_AXI_FEATURE_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_R0_MASK         (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_R0_SHIFT        (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_R0_MAX          (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_SLVERRCTRL_MASK (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_SLVERRCTRL_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_SLVERRCTRL_MAX  (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_R30_MASK        (0xFFFFFFFCU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_R30_SHIFT       (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_AXI_FEATURE_REG_R30_MAX         (0x3FFFFFFFU)

/* I_LINK_EQ_CONTROL_2_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3PRRMEN_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3PRRMEN_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3PRRMEN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3RMTXPR_MASK (0x0000001EU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3RMTXPR_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3RMTXPR_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3OVRREN_MASK (0x00000020U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3OVRREN_SHIFT (0x00000005U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3OVRREN_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3OVRRPR_MASK (0x000003C0U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3OVRRPR_SHIFT (0x00000006U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_G3OVRRPR_MAX (0x0000000FU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_R1_MASK   (0x001FFC00U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_R1_SHIFT  (0x0000000AU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_R1_MAX    (0x000007FFU)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_R2_MASK   (0xFFE00000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_R2_SHIFT  (0x00000015U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_LINK_EQ_CONTROL_2_REG_R2_MAX    (0x000007FFU)

/* I_CORE_FEATURE_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R0_MASK        (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R0_SHIFT       (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R0_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_APBCTRL_MASK   (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_APBCTRL_SHIFT  (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_APBCTRL_MAX    (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R2_MASK        (0x00000004U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R2_SHIFT       (0x00000002U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R2_MAX         (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R30_MASK       (0xFFFFFFF8U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R30_SHIFT      (0x00000003U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_CORE_FEATURE_REG_R30_MAX        (0x1FFFFFFFU)

/* I_RX_INVERT_POLARITY_REG */

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RX_INVERT_POLARITY_REG_RIPR_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RX_INVERT_POLARITY_REG_RIPR_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RX_INVERT_POLARITY_REG_RIPR_MAX (0x00000001U)

#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RX_INVERT_POLARITY_REG_R30_MASK (0xFFFFFFFEU)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RX_INVERT_POLARITY_REG_R30_SHIFT (0x00000001U)
#define CSL_PCIE_EP_CORE_LM_I_REGF_LM_PCIE_BASE_I_RX_INVERT_POLARITY_REG_R30_MAX  (0x7FFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_0_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_1_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_2_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_3_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_4_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_5_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_6_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_7_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_8_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_RSVD_MASK                         (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_RSVD_SHIFT                        (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_RSVD_MAX                          (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC0_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC0_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC0_DATA_MAX                          (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC1_DATA_MAX                          (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC3_DATA_MASK                         (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC3_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC3_DATA_MAX                          (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC3_RSVD_MASK                         (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC3_RSVD_SHIFT                        (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_DESC3_RSVD_MAX                          (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_REGION_SIZE_MASK              (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_REGION_SIZE_SHIFT             (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_REGION_SIZE_MAX               (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_RSVD_MASK                     (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_RSVD_SHIFT                    (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_RSVD_MAX                      (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_DATA_MASK                     (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_DATA_SHIFT                    (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR0_DATA_MAX                      (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR1_DATA_MASK                     (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR1_DATA_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_9_AXI_ADDR1_DATA_MAX                      (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_10_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_11_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_12_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_13_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_14_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_15_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_16_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_17_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_18_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_19_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_20_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_21_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_22_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_23_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_24_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_25_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_26_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_27_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_28_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_29_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_30_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_NUM_BITS_MASK                    (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_NUM_BITS_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_NUM_BITS_MAX                     (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_RSVD_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_RSVD_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_RSVD_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_DATA_MASK                        (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_DATA_SHIFT                       (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR0_DATA_MAX                         (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_ADDR1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC0_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC0_DATA_MAX                         (0xFFFFFFFFU)

/* DESC1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC1_DATA_MASK                        (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC1_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC1_DATA_MAX                         (0xFFFFFFFFU)

/* DESC3 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC3_DATA_MASK                        (0x007FFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC3_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC3_DATA_MAX                         (0x007FFFFFU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC3_RSVD_MASK                        (0xFF800000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC3_RSVD_SHIFT                       (0x00000017U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_DESC3_RSVD_MAX                         (0x000001FFU)

/* AXI_ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_REGION_SIZE_MASK             (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_REGION_SIZE_SHIFT            (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_REGION_SIZE_MAX              (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_RSVD_MASK                    (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_RSVD_SHIFT                   (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_RSVD_MAX                     (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_DATA_MASK                    (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_DATA_SHIFT                   (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR0_DATA_MAX                     (0x00FFFFFFU)

/* AXI_ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR1_DATA_MASK                    (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR1_DATA_SHIFT                   (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_OB_31_AXI_ADDR1_DATA_MAX                     (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_RSVD0_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_RSVD0_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_RSVD0_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_0_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_RSVD0_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_RSVD0_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_RSVD0_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_1_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_NUM_BITS_MASK                     (0x0000003FU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_NUM_BITS_SHIFT                    (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_NUM_BITS_MAX                      (0x0000003FU)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_RSVD0_MASK                        (0x000000C0U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_RSVD0_SHIFT                       (0x00000006U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_RSVD0_MAX                         (0x00000003U)

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_DATA_MASK                         (0xFFFFFF00U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_DATA_SHIFT                        (0x00000008U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR0_DATA_MAX                          (0x00FFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR1_DATA_MASK                         (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR1_DATA_SHIFT                        (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_WRAPPER_IB_7_ADDR1_DATA_MAX                          (0xFFFFFFFFU)

/* C0 */

#define CSL_PCIE_EP_CORE_ATU_CREDIT_THRESHOLD_C0_DATA_MASK                        (0x00000FFFU)
#define CSL_PCIE_EP_CORE_ATU_CREDIT_THRESHOLD_C0_DATA_SHIFT                       (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_CREDIT_THRESHOLD_C0_DATA_MAX                         (0x00000FFFU)

#define CSL_PCIE_EP_CORE_ATU_CREDIT_THRESHOLD_C0_HEADER_MASK                      (0x000FF000U)
#define CSL_PCIE_EP_CORE_ATU_CREDIT_THRESHOLD_C0_HEADER_SHIFT                     (0x0000000CU)
#define CSL_PCIE_EP_CORE_ATU_CREDIT_THRESHOLD_C0_HEADER_MAX                       (0x000000FFU)

/* L0 */

#define CSL_PCIE_EP_CORE_ATU_LINK_DOWN_INDICATOR_BIT_L0_CLEAR_LINK_DOWN_BIT_TO_PROCEED_MASK (0x00000001U)
#define CSL_PCIE_EP_CORE_ATU_LINK_DOWN_INDICATOR_BIT_L0_CLEAR_LINK_DOWN_BIT_TO_PROCEED_SHIFT (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_LINK_DOWN_INDICATOR_BIT_L0_CLEAR_LINK_DOWN_BIT_TO_PROCEED_MAX (0x00000001U)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_0_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_1_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_2_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_3_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_4_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_5_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_6_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR0 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR0_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR0_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR0_DATA_MAX                 (0xFFFFFFFFU)

/* ADDR1 */

#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR1_DATA_MASK                (0xFFFFFFFFU)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR1_DATA_SHIFT               (0x00000000U)
#define CSL_PCIE_EP_CORE_ATU_FUNC0_WRAPPER_IB_EP_7_ADDR1_DATA_MAX                 (0xFFFFFFFFU)

#ifdef __cplusplus
}
#endif
#endif
