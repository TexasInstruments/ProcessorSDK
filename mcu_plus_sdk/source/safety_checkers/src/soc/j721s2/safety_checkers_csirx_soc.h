/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \ingroup  CSIRX_SAFETY_CHECKERS
 *  \defgroup CSIRX_SAFETY_CHECKERS_SOC CSIRX Safety Checkers Library SOC defines
 *
 *  @{
 */

/**
 *  \file     safety_checkers_csirx_soc.h
 *
 *  \brief    This file contains data structures for CSIRX safety checker module
 *
 */

#ifndef SAFETY_CHECKERS_CSIRX_SOC_H_
#define SAFETY_CHECKERS_CSIRX_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cslr_soc.h"
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/csirx/csirx.h>
#include "safety_checkers_csirx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup SAFETY_CHECKERS_CSIRX
 *
 * \defgroup SAFETY_CHECKERS_CSIRX_MACROS  SAFETY_CHECKERS_CSIRX CSIRX safety checkers macro definition
 *  @{
 */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Maximum number of CSIRX instances */
#define SAFETY_CHECKERS_CSIRX_INSTANCES_MAX                           (0x2U)

/** \brief Stream control register addresses */
#define SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_BASE_ADDRESS_0           (CSL_CSI_RX_IF0_VBUS2APB_WRAP_VBUSP_APB_CSI2RX_BASE)
#define SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_BASE_ADDRESS(i)          (SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_BASE_ADDRESS_0 + (0x10000U * (uint32_t)i))
#define SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_LENGTH                   (0x14U)

/** \brief DPHY config register addresses */
#define SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_BASE_ADDRESS_0         (CSL_CSI_RX_IF0_VBUS2APB_WRAP_VBUSP_APB_CSI2RX_BASE)
#define SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_BASE_ADDRESS(i)        (SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_BASE_ADDRESS_0 + (0x10000U * (uint32_t)i))
#define SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_LENGTH                 (0x2U)

/** \brief DPHY PLL register addresses */
#define SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_BASE_ADDRESS_0            (CSL_DPHY_RX0_VBUS2APB_WRAP_VBUSP_K3_DPHY_RX_BASE)
#define SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_BASE_ADDRESS(i)           (SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_BASE_ADDRESS_0 + (0x10000U * (uint32_t)i))
#define SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_LENGTH                    (0x6U)

/** \brief DPHY Lane config register addresses */
#define SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_BASE_ADDRESS_0         (CSL_DPHY_RX0_MMR_SLV_K3_DPHY_WRAP_BASE)
#define SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_BASE_ADDRESS(i)        (SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_BASE_ADDRESS_0 + (0x10000U * (uint32_t)i))
#define SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_REGS_LENGTH            (0x1U)


/** \brief Virtual channel config register addresses */
#define SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_BASE_ADDRESS_0   (CSL_CSI_RX_IF0_VBUS2APB_WRAP_VBUSP_APB_CSI2RX_BASE)
#define SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_BASE_ADDRESS(i)  (SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_BASE_ADDRESS_0 + (0x10000U * (uint32_t)i))
#define SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_REGS_LENGTH      (0x4U)

/** \brief Data type and framesize register addresses */
#define SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_BASE_ADDRESS_0       (CSL_CSI_RX_IF0_RX_SHIM_VBUSP_MMR_CSI2RXIF_BASE)
#define SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_BASE_ADDRESS(i)      (SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_BASE_ADDRESS_0 + (0x10000U * (uint32_t)i))
#define SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_REGS_LENGTH          (0x20U)

/** \brief Vim Base address of MAIN R5 */
#define SAFETY_CHECKERS_CSIRX_UDMA_CSI_VIM_CONFIG_BASE_ADDRESS        (CSL_MAIN_DOMAIN_VIM_BASE_ADDR0)

/** \brief QoS settings register base address */
#define SAFETY_CHECKERS_CSIRX_QOS_BASE_ADDRESS_0                      (CSL_NAVSS0_BCDMA0_CFG_RCHAN_BASE)
/** \brief QoS configuration registers */  
#define SAFETY_CHECKERS_CSIRX_QOS_RCFG                                ((uint32_t)0x0U)
#define SAFETY_CHECKERS_CSIRX_QOS_RPRICTRL                            ((uint32_t)0x64U)
/** \brief QoS channel configuration register-set size */  
#define SAFETY_CHECKERS_CSIRX_QOS_CH_REG_SIZE                         ((uint32_t)0x100U)

/** \brief DPHY configuration related macros: Register offsets */
#define SAFETY_CHECKERS_CSIRX_DPHYRX_WRAP_REGS_LANE                   ((uint32_t)0U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_PCS_TX_DIG_TBIT0                 ((uint32_t)0xB00U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_PCS_TX_DIG_TBIT2                 ((uint32_t)0xB08U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_PCS_TX_DIG_TBIT3                 ((uint32_t)0xB0CU)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_CMN0_CMN_DIG_TBIT2               ((uint32_t)0x20U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_ISO_PHY_ISO_CL_CNTRL_L           ((uint32_t)0xC10U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_ISO_PHY_ISO_DL_CTRL_L0           ((uint32_t)0xC14U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_ISO_PHY_ISO_DL_CTRL_L1           ((uint32_t)0xC20U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_ISO_LDD_PHY_ISO_DL_CTRL_L2       ((uint32_t)0xC30U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_ISO_LDD_PHY_ISO_DL_CTRL_L3       ((uint32_t)0xC3CU)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_CMN0_CMN_DIG_TBIT56              ((uint32_t)0xF0U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_CMN0_CMN_DIG_TBIT35              ((uint32_t)0xA4U)
#define	SAFETY_CHECKERS_CSIRX_DPHYRX_CMN0_CMN_DIG_TBIT36              ((uint32_t)0xA8U)
#define SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONTROL                       ((uint32_t)0x40U)
#define	SAFETY_CHECKERS_CSIRX_DPHY_ERR_IRQ_MASK_CFG                   ((uint32_t)0x50U)

/** \brief Stream configuration related macros: Register offsets */  
#define SAFETY_CHECKERS_CSIRX_STREAM0_CTRL                            ((uint32_t)0x100U)
#define SAFETY_CHECKERS_CSIRX_STREAM0_DATA_CFG                        ((uint32_t)0x108U)
#define SAFETY_CHECKERS_CSIRX_STREAM0_CFG                             ((uint32_t)0x10CU)
#define SAFETY_CHECKERS_CSIRX_STREAM0_MONITOR_CTRL                    ((uint32_t)0x110U)
#define SAFETY_CHECKERS_CSIRX_STERAM0_TIMER                           ((uint32_t)0x11CU)
#define SAFETY_CHECKERS_CSIRX_STREAM0_FCC_CFG                         ((uint32_t)0x120U)
#define SAFETY_CHECKERS_CSIRX_STREAM1_CTRL                            ((uint32_t)0x200U)
#define SAFETY_CHECKERS_CSIRX_STREAM1_DATA_CFG                        ((uint32_t)0x208U)
#define SAFETY_CHECKERS_CSIRX_STREAM1_CFG                             ((uint32_t)0x20CU)
#define SAFETY_CHECKERS_CSIRX_STREAM1_MONITOR_CTRL                    ((uint32_t)0x210U)
#define SAFETY_CHECKERS_CSIRX_STERAM1_TIMER                           ((uint32_t)0x21CU)
#define SAFETY_CHECKERS_CSIRX_STREAM1_FCC_CFG                         ((uint32_t)0x220U)
#define SAFETY_CHECKERS_CSIRX_STREAM2_CTRL                            ((uint32_t)0x300U)
#define SAFETY_CHECKERS_CSIRX_STREAM2_DATA_CFG                        ((uint32_t)0x308U)
#define SAFETY_CHECKERS_CSIRX_STREAM2_CFG                             ((uint32_t)0x30CU)
#define SAFETY_CHECKERS_CSIRX_STREAM2_MONITOR_CTRL                    ((uint32_t)0x310U)
#define SAFETY_CHECKERS_CSIRX_STERAM2_TIMER                           ((uint32_t)0x31CU)
#define SAFETY_CHECKERS_CSIRX_STREAM2_FCC_CFG                         ((uint32_t)0x320U)
#define SAFETY_CHECKERS_CSIRX_STREAM3_CTRL                            ((uint32_t)0x400U)
#define SAFETY_CHECKERS_CSIRX_STREAM3_DATA_CFG                        ((uint32_t)0x408U)
#define SAFETY_CHECKERS_CSIRX_STREAM3_CFG                             ((uint32_t)0x40CU)
#define SAFETY_CHECKERS_CSIRX_STREAM3_MONITOR_CTRL                    ((uint32_t)0x410U)
#define SAFETY_CHECKERS_CSIRX_STERAM3_TIMER                           ((uint32_t)0x41CU)
#define SAFETY_CHECKERS_CSIRX_STREAM3_FCC_CFG                         ((uint32_t)0x420U)

/** \brief Mask for DPHY lane configuration registers */  
#define SAFETY_CHECKERS_CSIRX_MASK_NONE                               ((uint32_t)0xFFFFFFFFU)
#define SAFETY_CHECKERS_CSIRX_MASK_LANE_CONFIG                        ((uint32_t)0x05FF0FFFU)

/** @} */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/**
 *
 * \brief     This defines the array holding register offset values for each of
 *            the stream control registers
 *
 */
static uint32_t gSafetyCheckers_StrmCtrlRegOffset [] =
{
    SAFETY_CHECKERS_CSIRX_STREAM0_CTRL, SAFETY_CHECKERS_CSIRX_STREAM0_CFG,
    SAFETY_CHECKERS_CSIRX_STREAM0_MONITOR_CTRL, SAFETY_CHECKERS_CSIRX_STERAM0_TIMER,
    SAFETY_CHECKERS_CSIRX_STREAM0_FCC_CFG, SAFETY_CHECKERS_CSIRX_STREAM1_CTRL,
    SAFETY_CHECKERS_CSIRX_STREAM1_CFG, SAFETY_CHECKERS_CSIRX_STREAM1_MONITOR_CTRL,
    SAFETY_CHECKERS_CSIRX_STERAM1_TIMER, SAFETY_CHECKERS_CSIRX_STREAM1_FCC_CFG,
    SAFETY_CHECKERS_CSIRX_STREAM2_CTRL, SAFETY_CHECKERS_CSIRX_STREAM2_CFG,
    SAFETY_CHECKERS_CSIRX_STREAM2_MONITOR_CTRL, SAFETY_CHECKERS_CSIRX_STERAM2_TIMER,
    SAFETY_CHECKERS_CSIRX_STREAM2_FCC_CFG, SAFETY_CHECKERS_CSIRX_STREAM3_CTRL,
    SAFETY_CHECKERS_CSIRX_STREAM3_CFG, SAFETY_CHECKERS_CSIRX_STREAM3_MONITOR_CTRL,
    SAFETY_CHECKERS_CSIRX_STERAM3_TIMER, SAFETY_CHECKERS_CSIRX_STREAM3_FCC_CFG
};

/**
 *
 * \brief     This defines the array holding register offset values for each of
 *            the DPHY configuration registers
 *
 */
static uint32_t gSafetyCheckers_DphyConfigRegOffset [] =
{
    SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONTROL, SAFETY_CHECKERS_CSIRX_DPHY_ERR_IRQ_MASK_CFG
};

/**
 *
 * \brief     This defines the array holding register offset values for each of
 *            the DPHY Pll configuration registers
 *
 */
static uint32_t gSafetyCheckers_DphyPllRegOffset [] =
{
    SAFETY_CHECKERS_CSIRX_DPHYRX_WRAP_REGS_LANE, SAFETY_CHECKERS_CSIRX_DPHYRX_PCS_TX_DIG_TBIT0, SAFETY_CHECKERS_CSIRX_DPHYRX_CMN0_CMN_DIG_TBIT2, 
    SAFETY_CHECKERS_CSIRX_DPHYRX_CMN0_CMN_DIG_TBIT35, SAFETY_CHECKERS_CSIRX_DPHYRX_PCS_TX_DIG_TBIT2,SAFETY_CHECKERS_CSIRX_DPHYRX_PCS_TX_DIG_TBIT3
};

/**
 *
 * \brief     This defines the array holding register offset values for each of
 *            the DPHY Lane configuration registers
 *
 */
static uint32_t gSafetyCheckers_DphyLaneConfigRegOffset [] =
{
    SAFETY_CHECKERS_CSIRX_DPHYRX_WRAP_REGS_LANE
};

/**
 *
 * \brief     This defines the array holding register offset values for each of
 *            the Virtual channel configuration registers
 *
 */
static uint32_t gSafetyCheckers_VirtualChannelConfigRegOffset [] =
{
    SAFETY_CHECKERS_CSIRX_STREAM0_DATA_CFG, SAFETY_CHECKERS_CSIRX_STREAM1_DATA_CFG,
    SAFETY_CHECKERS_CSIRX_STREAM2_DATA_CFG, SAFETY_CHECKERS_CSIRX_STREAM3_DATA_CFG
};

/**
 *
 * \brief     This defines the array holding register offset values for each of
 *            the DMA configuration registers
 *
 */
static uint32_t gSafetyCheckers_DataTypeFrameSizeRegOffset [] =
{
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x0U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x1U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x2U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x3U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x4U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x5U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x6U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x7U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x8U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x9U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0xAU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0xBU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0xCU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0xDU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0xEU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0xFU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x10U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x11U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x12U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x13U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x14U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x15U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x16U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x17U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x18U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x19U),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x1AU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x1BU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x1CU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x1DU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x1EU),
    CSL_CSI_RX_IF_CNTX_CNTL_DMACNTX(0x1FU),
};

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Structure to hold VIM register configuration for CSIRX module
 *        module registers
 */
typedef struct
{
    CSL_vimRegs *pRegs;
    /**< Pointer to VIM registers */
    uint32_t intrNum;
    /**< Core interrupt number */
    uint32_t pri;
    /**< Priority of the interrupt*/
    CSL_VimIntrMap intrMap;
    /**< Enumerator to define if interrupt is IRQ/FIQ*/
    CSL_VimIntrType intrType;
    /**< Level/Pulse interrupt type*/
    uint32_t vecAddr;
    /**< Vector address*/
} SafetyCheckers_CsirxVimCfg;

/**
 * \brief Structure to hold QoS register settings of CSIRX module
 *        module registers
 */
typedef struct
{
    uint8_t chanType; 
    /**< Channel type. Refer \ref tisci_msg_rm_udmap_rx_ch_cfg_req::rx_chan_type */
    uint8_t priority;
    /**< 3-bit priority value (0=highest, 7=lowest) */
    uint8_t busOrderId;
    /**< 4-bit orderid value */
} SafetyCheckers_CsirxQoSSettings;

/**
 *  struct CsirxI2cTrxnCfg
 *  \brief Structure to store I2C transaction configuration
 */
typedef struct
{
    uint8_t regAddr;
    /**< I2C register offset address */
    uint8_t *regData;
    /**< I2C register data buffer */
    uint8_t numOfBytes;
    /**< Receive data width */
    uint32_t i2cTimeout;
    /**< I2C driver timeout value */
} SafetyCheckers_CsirxI2cTrxnCfg;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Function to get vim register configuration
 *
 *  \param intrNum  Core interrupt number
 *  \param vimCfg   Pointer to vim register configuration
 *
 *  \return SAFETY_CHECKERS_SOK if successful
 *          SAFETY_CHECKERS_FAIL if NULL params are passed
 *
 */
int32_t SafetyCheckers_csirxGetVimRegCfgIntrNum(uint32_t intrNum, SafetyCheckers_CsirxVimCfg *vimCfg);

/**
 *  \brief Function to get vim register configuration
 *
 *  \param vimCfg  Pointer to vim register configuration
 *
 *  \return SAFETY_CHECKERS_SOK if successful
 *          SAFETY_CHECKERS_FAIL if NULL params are passed
 *          SAFETY_CHECKERS_REG_DATA_MISMATCH  if verification is failed
 *
 */
int32_t SafetyCheckers_csirxVerifyVimRegCfgIntrNum(SafetyCheckers_CsirxVimCfg *vimCfg);

/**
 *  \brief Function to get CSIRX QoS settings
 *
 *  \param qosSettings  Pointer to QoS settings
 *  \param drvHandle    Fvid2 driver handle
 *
 *  \return SAFETY_CHECKERS_SOK if successful
 *          SAFETY_CHECKERS_FAIL if NULL params are passed
 *
 */
int32_t SafetyCheckers_csirxGetQoSCfg(SafetyCheckers_CsirxQoSSettings *qosSettings, void *drvHandle);

/**
 *  \brief Function to verify CSIRX QoS settings
 *
 *  \param qosSettings  Pointer to QoS settings
 *  \param drvHandle    Fvid2 driver handle
 *
 *  \return SAFETY_CHECKERS_SOK if successful
 *          SAFETY_CHECKERS_FAIL if NULL params are passed
 *          SAFETY_CHECKERS_REG_DATA_MISMATCH  if verification is failed
 *
 */
int32_t SafetyCheckers_csirxVerifyQoSCfg(SafetyCheckers_CsirxQoSSettings *qosSettings, void *drvHandle);

/**
 * \brief  Function to read 8-bit register via I2C
 *
 * This function is used to read the 8-bit data from the i2c
 * device registers
 *
 * \param   handle      Low level driver handle
 * \param   slaveAddr   I2C slave address
 * \param   i2cTrxnCfg      I2C transaction configuration
 *
 * \return  SAFETY_CHECKERS_SOK in case of success or appropriate error code.
 */
int32_t SafetyCheckers_csirxi2c8BitRegRd(void   *handle,
                                         uint32_t slaveAddr,
                                         const SafetyCheckers_CsirxI2cTrxnCfg* i2cTrxnCfg);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef SAFETY_CHECKERS_CSIRX_SOC_H_ */
/** @} */
