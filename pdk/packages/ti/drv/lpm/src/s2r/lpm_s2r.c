/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 * \file lpm_s2r.c
 *
 * \brief This file contains the code which is loaded to SRAM for LPM support.
 * This code sets the DDR in retention mode and configures the SOC and PMIC according
 * to the Low power mode. Then we ask the PMIC to powerdown the SOC.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdarg.h>
#include <ti/csl/soc.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/csl/cslr_pmmc.h>
#if defined(SOC_J7200)
#include <ti/csl/soc/j7200/src/cslr_wkup_ctrl_mmr.h>
#include <ti/csl/soc/j7200/src/csl_soc_firewalls.h>
#include <ti/board/src/j7200_evm/include/board_cfg.h>
#include <ti/board/src/j7200_evm/include/board_pll.h>
#include <ti/board/src/j7200_evm/include/board_ddr.h>
#elif defined(SOC_J784S4)
#include <ti/csl/soc/j784s4/src/cslr_wkup_ctrl_mmr.h>
#include <ti/board/src/j784s4_evm/include/board_cfg.h>
#include <ti/board/src/j784s4_evm/include/board_pll.h>
#include <ti/board/src/j784s4_evm/include/board_ddr.h>
#elif defined(SOC_J742S2)
#include <ti/csl/soc/j784s4/src/cslr_wkup_ctrl_mmr.h>
#include <ti/board/src/j742s2_evm/include/board_cfg.h>
#include <ti/board/src/j742s2_evm/include/board_pll.h>
#include <ti/board/src/j742s2_evm/include/board_ddr.h>
#endif
#include <ti/drv/lpm/include/io_retention/dmsc_cm.h>
#include <ti/drv/pmic/src/pmic_fsm_priv.h>

/* ========================================================================== */
/*                               Debug Options                                */
/* ========================================================================== */

/**
 * Uncomment the define of LPM_DEBUG_SRAM_S2R enables the debug messages during
 * suspend sequence.
 * Additional debug messages can be added using the flag LPM_DEBUG_FULL_SRAM_S2R.
 * By default debug messages are printed on MCU_UART0, but they can be
 * redirected by enabling LPM_DEBUG_UART_MAIN flag.
 * A compilation is needed for each modification.
 */
//#define LPM_DEBUG_SRAM_S2R
//#define LPM_DEBUG_FULL_SRAM_S2R
//#define LPM_DEBUG_UART_MAIN

#ifdef  LPM_DEBUG_UART_MAIN
#define LPM_UART_ADDR CSL_UART0_BASE
#else
#define LPM_UART_ADDR CSL_MCU_UART0_BASE
#endif

#if defined(LPM_DEBUG_SRAM_S2R) && defined(LPM_DEBUG_FULL_SRAM_S2R)
#define Lpm_debugFullPrintf Lpm_debugPrintf
#define Lpm_debugReadPmicA  Lpm_readPmicA
#define Lpm_debugReadPmicB  Lpm_readPmicB
#else
#define Lpm_debugFullPrintf(...) do {} while(0)
#define Lpm_debugReadPmicA(...)  do {} while(0)
#define Lpm_debugReadPmicB(...)  do {} while(0)
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define LPM_REG32_RD_MODIFY_SET(r, m)               CSL_REG32_WR(r, CSL_REG32_RD(r) | m)
#define LPM_REG32_RD_MODIFY_CLR(r, m)               CSL_REG32_WR(r, CSL_REG32_RD(r) & ~(m))
#define LPM_REG32_RD_MODIFY_SET_OFF(o, r, m)        CSL_REG32_WR_OFF(o, r, CSL_REG32_RD_OFF(o, r) | m)
#define LPM_REG32_RD_MODIFY_CLR_OFF(o, r, m)        CSL_REG32_WR_OFF(o, r, CSL_REG32_RD_OFF(o, r) & ~(m))

#define LPM_PLL0_CFG_RD(r)                          CSL_REG32_RD_OFF(CSL_PLL0_CFG_BASE, r)
#define LPM_PLL0_CFG_WR(r, v)                       CSL_REG32_WR_OFF(CSL_PLL0_CFG_BASE, r, v)
#define LPM_PLL0_CFG_RD_MODIFY_SET(r, m)            LPM_PLL0_CFG_WR(r, LPM_PLL0_CFG_RD(r) | m)
#define LPM_PLL0_CFG_RD_MODIFY_CLR(r, m)            LPM_PLL0_CFG_WR(r, LPM_PLL0_CFG_RD(r) & ~(m))

#define LPM_WKUP_CTRL_MMR0_CFG0_RD(r)               CSL_REG32_RD_OFF(CSL_WKUP_CTRL_MMR0_CFG0_BASE, r)
#define LPM_WKUP_CTRL_MMR0_CFG0_WR(r, v)            CSL_REG32_WR_OFF(CSL_WKUP_CTRL_MMR0_CFG0_BASE, r, v)
#define LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(r, m) LPM_WKUP_CTRL_MMR0_CFG0_WR(r, LPM_WKUP_CTRL_MMR0_CFG0_RD(r) | m)
#define LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_CLR(r, m) LPM_WKUP_CTRL_MMR0_CFG0_WR(r, LPM_WKUP_CTRL_MMR0_CFG0_RD(r) & ~(m))

#define LPM_MCU_CTRL_MMR0_CFG0_RD(r)                CSL_REG32_RD_OFF(CSL_MCU_CTRL_MMR0_CFG0_BASE, r)
#define LPM_MCU_CTRL_MMR0_CFG0_WR(r, v)             CSL_REG32_WR_OFF(CSL_MCU_CTRL_MMR0_CFG0_BASE, r, v)
#define LPM_MCU_CTRL_MMR0_CFG0_RD_MODIFY_SET(r, m)  LPM_MCU_CTRL_MMR0_CFG0_WR(r, LPM_MCU_CTRL_MMR0_CFG0_RD(r) | m)
#define LPM_MCU_CTRL_MMR0_CFG0_RD_MODIFY_CLR(r, m)  LPM_MCU_CTRL_MMR0_CFG0_WR(r, LPM_MCU_CTRL_MMR0_CFG0_RD(r) & ~(m))

#define LPM_WKUP_I2C0_CFG_RD(r)                     CSL_REG32_RD_OFF(CSL_WKUP_I2C0_CFG_BASE, r)
#define LPM_WKUP_I2C0_CFG_WR(r, v)                  CSL_REG32_WR_OFF(CSL_WKUP_I2C0_CFG_BASE, r, v)
#define LPM_WKUP_I2C0_CFG_RD_MODIFY_SET(r, m)       LPM_WKUP_I2C0_CFG_WR(r, LPM_WKUP_I2C0_CFG_RD(r) | m)
#define LPM_WKUP_I2C0_CFG_RD_MODIFY_CLR(r, m)       LPM_WKUP_I2C0_CFG_WR(r, LPM_WKUP_I2C0_CFG_RD(r) & ~(m))

#define LPM_DDRSS0_RD(r)                            CSL_REG32_RD_OFF(CSL_STD_FW_COMPUTE_CLUSTER0__VBUSP_DDRSS0_DDRSS0_CTLCFG_CTL_CFG_START, r)
#define LPM_DDRSS0_WR(r, v)                         CSL_REG32_WR_OFF(CSL_STD_FW_COMPUTE_CLUSTER0__VBUSP_DDRSS0_DDRSS0_CTLCFG_CTL_CFG_START, r, v)
#define LPM_DDRSS0_RD_MODIFY_SET(r, m)              LPM_DDRSS0_WR(r, LPM_DDRSS0_RD(r) | m)
#define LPM_DDRSS0_RD_MODIFY_CLR(r, m)              LPM_DDRSS0_WR(r, LPM_DDRSS0_RD(r) & ~(m))

#define LPM_CTRL_MMR0_CFG0_RD(r)                    CSL_REG32_RD_OFF(CSL_CTRL_MMR0_CFG0_BASE, r)
#define LPM_CTRL_MMR0_CFG0_WR(r, v)                 CSL_REG32_WR_OFF(CSL_CTRL_MMR0_CFG0_BASE, r, v)
#define LPM_CTRL_MMR0_CFG0_RD_MODIFY_SET(r, m)      LPM_CTRL_MMR0_CFG0_WR(r, LPM_CTRL_MMR0_CFG0_RD(r) | m)
#define LPM_CTRL_MMR0_CFG0_RD_MODIFY_CLR(r, m)      LPM_CTRL_MMR0_CFG0_WR(r, LPM_CTRL_MMR0_CFG0_RD(r) & ~(m))

#define LPM_DMSC_PWRCTRL_RD(r)                      CSL_REG32_RD_OFF(CSL_STD_FW_WKUP_DMSC0_PWRCTRL_0_DMSC_PWR_MMR_PWR_START, r)
#define LPM_DMSC_PWRCTRL_WR(r, v)                   CSL_REG32_WR_OFF(CSL_STD_FW_WKUP_DMSC0_PWRCTRL_0_DMSC_PWR_MMR_PWR_START, r, v)
#define LPM_DMSC_PWRCTRL_RD_MODIFY_SET(r, m)        LPM_DMSC_PWRCTRL_WR(r, LPM_DMSC_PWRCTRL_RD(r) | m)
#define LPM_DMSC_PWRCTRL_RD_MODIFY_CLR(r, m)        LPM_DMSC_PWRCTRL_WR(r, LPM_DMSC_PWRCTRL_RD(r) & ~(m))

/* PMIC configuration Registers */
#define LPM_GPIO2_CONF                      (0x32U)
#define LPM_GPIO3_CONF                      (0x33U)
#define LPM_GPIO6_CONF                      (0x36U)
#define LPM_GPIO5_CONF                      (0x35U)
#define LPM_INT_TOP                         (0x5AU)
#define LPM_INT_BUCK_STATUS                 (0x5BU)
#define LPM_INT_BUCK1_2_STATUS              (0x5CU)
#define LPM_INT_BUCK3_4_STATUS              (0x5DU)
#define LPM_INT_BUCK5_STATUS                (0x5EU)
#define LPM_INT_LDO_VMON_STATUS             (0x5FU)
#define LPM_INT_LDO1_2_STATUS               (0x60U)
#define LPM_INT_LDO3_4_STATUS               (0x61U)
#define LPM_INT_VMON_STATUS                 (0x62U)
#define LPM_INT_GPIO_REGADDR                (0x63U)
#define LPM_INT_GPIO1_8_REGADDR             (0x64U)
#define LPM_INT_STARTUP_REGADDR             (0x65U)
#define LPM_INT_MISC_REGADDR_STATUS         (0x66U)
#define LPM_INT_MODERATE_ERR_STATUS         (0x67U)
#define LPM_INT_SEVERE_ERR_STATUS           (0x68U)
#define LPM_INT_FSM_ERR_STATUS              (0x69U)
#define LPM_INT_COMM_ERR_STATUS             (0x6AU)
#define LPM_INT_READBACK_ERR_STATUS         (0x6BU)
#define LPM_INT_ESM_ERR_STATUS              (0x6CU)
#define LPM_MASK_GPIO1_8_FALL_REGADDR       (0x4FU)
#define LPM_MASK_GPIO1_8_RISE_REGADDR       (0x50U)
#define LPM_GPIO_IN_1_REGADDR               (0x3FU)
#define LPM_GPIO4_CONF_REGADDR              (0x34U)
#define LPM_MASK_GPIO9_11_REGADDR           (0x51U)
#define LPM_GPIO_OUT_1_REGADDR              (0x3DU)
#define LPM_FSM_I2C_TRIGGERS                (0x80U)
#define LPM_DEVICE_ID_PMICB                 (0x86U)

#define LPM_DEVICE_ID_PMICB_EVM             (0x7U)
#define LPM_I2C_DETECT_TIMEOUT              (500U)

/* PMIC configuration Signals */
#define LPM_DDR_RET_VAL                     (1U << 1U)
#define LPM_DDR_RET_CLK                     (1U << 2U)
#define LPM_EN_DDR_RET_1V1                  (1U << 5U)
#define LPM_EN_GPIO_RET_3V3                 (1U << 4U)
#define LPM_GPIO4_BIT                       (1U << 3U)
#define LPM_OD_SHIFT                        (0x1U)
#define LPM_DIR_SHIFT                       (0U)
#define LPM_SCRATCH_PAD_REG_3               (0xCBU)
#define LPM_MAGIC_SUSPEND                   (0xBAU)
#define LPM_GPIO1_8_FALL                    (0xFFU)
#define LPM_GPIO1_8_RISE                    (0xF7U)
#define LPM_DDR_RET_I2C_7                   (1U << 7U)
#define LPM_GPIO_RET_I2C_5                  (1U << 5U)
#define LPM_GPIO_RET_I2C_6                  (1U << 6U)

/* PMIC interrupt signals */
#define LPM_INT_TOP_BUCK                    (1U << 0U)
#define LPM_INT_TOP_LDO_VMON                (1U << 1U)
#define LPM_INT_TOP_GPIO                    (1U << 2U)
#define LPM_INT_TOP_STARTUP                 (1U << 3U)
#define LPM_INT_TOP_MISC                    (1U << 4U)
#define LPM_INT_TOP_MODERATE_ERR            (1U << 5U)
#define LPM_INT_TOP_SEVERE_ERR              (1U << 6U)
#define LPM_INT_TOP_FSM_ERR                 (1U << 7U)
#define LPM_INT_BUCK1_2                     (1U << 0U)
#define LPM_INT_BUCK3_4                     (1U << 1U)
#define LPM_INT_BUCK5                       (1U << 2U)
#define LPM_INT_LDO1_2                      (1U << 0U)
#define LPM_INT_LDO3_4                      (1U << 1U)
#define LPM_INT_VMON                        (1U << 2U)
#define LPM_INT_GPIO1_8                     (1U << 3U)
#define LPM_INT_COMM_ERR                    (1U << 4U)
#define LPM_INT_READBACK_ERR                (1U << 5U)
#define LPM_INT_ESM                         (1U << 6U)

/* IO_ISOLATION control signals and registers, part of PMCTRL */
#define LPM_IO_MAGIC_WORD                   (0x2AAAAAAAU << 1U)
#define LPM_FORCE_DS_MAIN                   (1U << 8U)
#define LPM_FORCE_DS_WKUP                   (0x1U)
#define LPM_WUCLK_CTRL                      (1U << 8U)
#define LPM_WUCLK_STATUS                    (1U << 9U)
#define LPM_IO_ISO_CTRL                     (1U << 24U)
#define LPM_GLOBAL_WUEN                     (1U << 16U)
#define LPM_IO_ISO_STATUS_0                 (1U << 25U)
#define LPM_MAGIC_WORD_LOAD_ENABLE          (0x1U)
#define LPM_IO_PM_CTRL_0                    (0x1U)
#define LPM_IO_PM_CTRL_1                    (1U << 1U)
#define LPM_IO_ISOBYPASS                    (1U << 6U)
#define LPM_WAKE_EN                         (1U << 16U)
#define LPM_RX_ACTIVE                       (1U << 18U)
#define LPM_TX_DIS                          (1U << 21U)
#define LPM_ISO_BYPASS_PAD                  (1U << 23U)
#define LPM_DSOUT_DIS                       (1U << 25U)

/* PMCTRL registers part of the DMSC, used for IO_ISOLATION configuration */
#define LPM_DMSC_LOCK0_KICK0                (0x20U)
#define LPM_DMSC_LOCK0_KICK1                (0x24U)
#define LPM_DMSC_FW_CTRL_OUT0_SET           (0x44U)
#define LPM_DMSC_FW_CTRL_OUT0_VAL           (0x40U)
#define LPM_DMSC_FW_CTRL_OUT1_SET           (0x54U)
#define LPM_DMSC_FW_CTRL_OUT1_VAL           (0x50U)
#define LPM_DMSC0_PWRCTRL_0_FW_STS_IN0      (0x4CU)
#define LPM_DMSC_PMCTRL_IO_1                (0x88U)
#define LPM_DMSC_KICK0_UNLOCK               (0x8A6B7CDAU)
#define LPM_DMSC_KICK1_UNLOCK               (0x823CAEF9U)

/* Low power modes */
#define LPM_SOC_OFF                         (0x5U)
#define LPM_IO_ONLY_PLUS_DDR                (0x2U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

struct Lpm_DdrInstanceConfig {
    uint32_t ddrBase;
    uint32_t chngReq;
    uint32_t chngAck;
    uint32_t clkChngReq;
    uint32_t clkChngAck;
    uint32_t pllCtrl;
    uint32_t pllHsdiv0;
    uint32_t pllFreq[3];
};

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(SOC_J784S4)
static const struct Lpm_DdrInstanceConfig gLpmDdr[] =
{
    {
        .ddrBase = CSL_STD_FW_COMPUTE_CLUSTER0_CFG_WRAP_0__VBUSP_DDRSS0_DDRSS0_CTLCFG_VBUSP_DDRSS0_CTLCFG_START,
        .chngReq = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_REQ0,
        .chngAck = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_ACK0,
        .clkChngReq = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ0,
        .clkChngAck = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK0,
        .pllCtrl = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL12_CTRL,
        .pllHsdiv0 = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL12_HSDIV_CTRL0,
        .pllFreq = { 19200000U, 1066500000U, 1066500000U }
    },
    {
        .ddrBase = CSL_STD_FW_COMPUTE_CLUSTER0_CFG_WRAP_0__VBUSP_DDRSS1_DDRSS1_CTLCFG_VBUSP_DDRSS1_CTLCFG_START,
        .chngReq = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_REQ1,
        .chngAck = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_ACK1,
        .clkChngReq = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ1,
        .clkChngAck = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK1,
        .pllCtrl = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL26_CTRL,
        .pllHsdiv0 = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL26_HSDIV_CTRL0,
        .pllFreq = { 19200000U, 1066500000U, 1066500000U }
    },
    {
        .ddrBase = CSL_STD_FW_COMPUTE_CLUSTER0_CFG_WRAP_0__VBUSP_DDRSS2_DDRSS2_CTLCFG_VBUSP_DDRSS2_CTLCFG_START,
        .chngReq = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_REQ2,
        .chngAck = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_ACK2,
        .clkChngReq = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ2,
        .clkChngAck = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK2,
        .pllCtrl = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL27_CTRL,
        .pllHsdiv0 = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL27_HSDIV_CTRL0,
        .pllFreq = { 19200000U, 1066500000U, 1066500000U }
    },
    {
        .ddrBase = CSL_STD_FW_COMPUTE_CLUSTER0_CFG_WRAP_0__VBUSP_DDRSS3_DDRSS3_CTLCFG_VBUSP_DDRSS3_CTLCFG_START,
        .chngReq = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_REQ3,
        .chngAck = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_ACK3,
        .clkChngReq = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ3,
        .clkChngAck = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK3,
        .pllCtrl = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL28_CTRL,
        .pllHsdiv0 = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL28_HSDIV_CTRL0,
        .pllFreq = { 19200000U, 1066500000U, 1066500000U }
    },
    { },
};

#elif defined (SOC_J742S2)
static const struct Lpm_DdrInstanceConfig gLpmDdr[] =
{
    {
        .ddrBase = CSL_STD_FW_COMPUTE_CLUSTER0_CFG_WRAP_0__VBUSP_DDRSS0_DDRSS0_CTLCFG_VBUSP_DDRSS0_CTLCFG_START,
        .chngReq = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_REQ0,
        .chngAck = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_ACK0,
        .clkChngReq = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ0,
        .clkChngAck = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK0,
        .pllCtrl = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL12_CTRL,
        .pllHsdiv0 = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL12_HSDIV_CTRL0,
        .pllFreq = { 19200000U, 1066500000U, 1066500000U }
    },
    {
        .ddrBase = CSL_STD_FW_COMPUTE_CLUSTER0_CFG_WRAP_0__VBUSP_DDRSS1_DDRSS1_CTLCFG_VBUSP_DDRSS1_CTLCFG_START,
        .chngReq = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_REQ1,
        .chngAck = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_ACK1,
        .clkChngReq = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ1,
        .clkChngAck = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK1,
        .pllCtrl = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL26_CTRL,
        .pllHsdiv0 = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL26_HSDIV_CTRL0,
        .pllFreq = { 19200000U, 1066500000U, 1066500000U }
    },
    { },
};
#else

static const struct Lpm_DdrInstanceConfig gLpmDdr[] =
{
    {
        .ddrBase = CSL_STD_FW_COMPUTE_CLUSTER0__VBUSP_DDRSS0_DDRSS0_CTLCFG_CTL_CFG_START,
        .chngReq = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_REQ,
        .chngAck = CSL_MAIN_CTRL_MMR_CFG0_CHNG_DDR4_FSP_ACK,
        .clkChngReq = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ,
        .clkChngAck = CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK,
        .pllCtrl = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL12_CTRL,
        .pllHsdiv0 = CSL_PLL0_CFG_BASE + CSL_MAIN_PLL_MMR_CFG_PLL12_HSDIV_CTRL0,
        .pllFreq = { 19200000U, 800000000U, 800000000U }
    },
    { },
};
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Run the suspend sequence (set DDR in retention and powerdown the SOC)
 *
 * This function is the entry point when DM-Firmware jump to SRAM to start the
 * suspend sequence. It configures the PMIC and SOC according to the Low Power mode
 *
 * We then put the DDR in self-refresh and power down the SOC
 *
 */
void Lpm_enterRetention(void);

/**
 * \brief Set the DDR in Self-refresh mode
 *
 */
static void Lpm_ddrEnterSelfRefresh(void);

/**
  * \brief Set the MCU IOs in Isolation
  *
  * Enable IO isolation by loading the magic word
  * Bypass isolation for MCU_I2C to communicate with the PMIC
  * Also bypass isolation for the BOOTMODE pins
  *
  */
static void Lpm_enableMcuIOIsolation(void);

/**
  * \brief Set the Main IOs in Isolation
  *
  * Enable IO isolation by loading the magic word
  * for main domain IOs
  *
  */
static void Lpm_enableMainIOIsolation(void);

/**
 * \brief Ask PMIC to powerdown the SOC
 *
 * \param mode  Low power mode that we are entering into
 *
 */
static void Lpm_setupPmic(uint32_t mode);

/**
 * \brief Clean R5 Caches
 *
 */
static void Lpm_cleanAllDCache(void);

/**
 * \brief Spin in a loop in case of failure
 */
static void Lpm_hangAbort(void);

/**
 * A simple UART based printf function supporting \%p, \%x, and \%X.
 *
 * \param pcString is the format string.
 * \param ... are the optional arguments, which depend on the contents of the
 * format string.
 *
 * This function is a partial copy of UART_printf from
 * ti/drv/uart/src/UART_stdio.c.
 * But only the support of \%p, \%x and \%X was kept.
 *
 * \return None.
 */
static void Lpm_debugPrintf(const char *pcString, ...);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Lpm_enterRetention(void)
{
    /* Mark r2 as clobbered, as mode is stored in that*/
    asm volatile("" ::: "r2");
    volatile uint32_t mode;

    /* Store it explicitly in memory */
    asm volatile("str r2, %0" : "=m" (mode));

    Lpm_debugPrintf("Lpm_enterRetention: Enter retention with mode = %x\n", mode);

    /* Make sure that nothing remains in cache before going to retention */
    Lpm_cleanAllDCache();

    Lpm_ddrEnterSelfRefresh();
    Lpm_debugFullPrintf("Lpm_enterRetention: DDR retention done\n");

    if (mode == LPM_IO_ONLY_PLUS_DDR)
    {
        #if defined(SOC_J7200)
        Lpm_enableMainIOIsolation();
        #elif defined(SOC_J784S4) || defined(SOC_J742S2)
        Lpm_enableMcuIOIsolation();
        #endif

        Lpm_debugPrintf("Lpm_enableMcuIOIsolation: Isolation enabled! \n");
    }

    Lpm_setupPmic(mode);
    Lpm_debugPrintf("Lpm_enterRetention: Done! Going to wait now \n");

    while(1)
    {
        /* Wait for PMIC to powerdown the SOC */
    }
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static void Lpm_hangAbort()
{
    volatile int b = 0xAU;
    while (b != 0U)
    {
        /* Spin in a loop in case of failure because we cannot abort from this stage */
    }
}

static void Lpm_cleanAllDCache(void)
{
    uint32_t set, way;

    for(way = 0; way < 4U ; way++)
    {
        for(set = 0; set < 0x200U; set++)
        {
            uint32_t val = (set << 4U) | (way << 30U);
            asm("    dmb");
            __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 2\t\n": "=r"(val));
            __asm__ __volatile__ ("mcr p15, 0, %0, c7, c6, 2\t\n": "=r"(val));
            asm("    dsb");
        }
    }
}

static void Lpm_setPllForDDR(const struct Lpm_DdrInstanceConfig *ddrInstance, uint32_t index)
{
    uint32_t PLL_freq = ddrInstance->pllFreq[index];
    uint32_t val, val1;

    if (PLL_freq <= 27500000U)
    {
        LPM_REG32_RD_MODIFY_SET(ddrInstance->pllCtrl, 0x80000000U);
    }
    else if (PLL_freq <= 133000000U)
    {
        val1 = 2133000000U / PLL_freq;
        val = CSL_REG32_RD(ddrInstance->pllHsdiv0) & ~(0x7FU << 0U);
        CSL_REG32_WR(ddrInstance->pllHsdiv0, val | (val1 - 1U));
        LPM_REG32_RD_MODIFY_CLR(ddrInstance->pllCtrl, 0x80000000U);
    }
    else if (PLL_freq <= 266500000U)
    {
        val = CSL_REG32_RD(ddrInstance->pllHsdiv0) & ~(0x7FU << 0U);
        CSL_REG32_WR(ddrInstance->pllHsdiv0, val | 0x7U);
        LPM_REG32_RD_MODIFY_CLR(ddrInstance->pllCtrl, 0x80000000U);
    }
    else if (PLL_freq <= 533000000U)
    {
        val = CSL_REG32_RD(ddrInstance->pllHsdiv0) & ~(0x7FU << 0U);
        CSL_REG32_WR(ddrInstance->pllHsdiv0, val | 0x3U);
        LPM_REG32_RD_MODIFY_CLR(ddrInstance->pllCtrl, 0x80000000U);
    }
    else if (PLL_freq <= 800000000U)
    {
        val = CSL_REG32_RD(ddrInstance->pllHsdiv0) & ~(0x7FU << 0U);
        CSL_REG32_WR(ddrInstance->pllHsdiv0, val | 0x3U);
        LPM_REG32_RD_MODIFY_CLR(ddrInstance->pllCtrl, 0x80000000U);
    }
    else if (PLL_freq <= 1066500000U)
    {
        val = CSL_REG32_RD(ddrInstance->pllHsdiv0) & ~(0x7FU << 0U);
        CSL_REG32_WR(ddrInstance->pllHsdiv0, val | 0x1U);
        LPM_REG32_RD_MODIFY_CLR(ddrInstance->pllCtrl, 0x80000000U);
    }
    else
    {
        Lpm_debugFullPrintf("Unexpected frequency for DDR: 0x%x\n", PLL_freq);
    }
}

static void Lpm_waitForSet(uint32_t address, uint32_t setval, uint32_t shift)
{
    uint32_t val;

    do
    {
        val = LPM_CTRL_MMR0_CFG0_RD(address);
    }
    while((val & (setval << shift)) != (setval << shift));
}

static void Lpm_verifiedSet(uint32_t address,
                     uint32_t  setval,
                     uint32_t  mask,
                     uint32_t  shift)
{
    uint32_t val;

    val = LPM_CTRL_MMR0_CFG0_RD(address) & ~(mask << shift);
    LPM_CTRL_MMR0_CFG0_WR(address, val | (setval << shift));

    Lpm_waitForSet(address, setval, shift);
}

static void Lpm_ddrFreqChange(const struct Lpm_DdrInstanceConfig *ddrInstance)
{
    volatile uint32_t t;
    const uint32_t fsp = 0U;

    LPM_CTRL_MMR0_CFG0_WR(CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0, KICK0_UNLOCK);
    LPM_CTRL_MMR0_CFG0_WR(CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK1, KICK1_UNLOCK);

    /* Set REQ_TYPE to fsp */
    LPM_CTRL_MMR0_CFG0_WR(ddrInstance->chngReq, fsp);
    for(t = 0; t < 100U; t++)
    {
        /* Wait for the frequency change to take affect */
    }

    /*
     * Assert req:
     * DDR will respond as if it was its idea to change the frequency
     */
    LPM_CTRL_MMR0_CFG0_RD_MODIFY_SET(ddrInstance->chngReq, 0x100U);

    /* DDR asserts a CLKCHNG_REQ */
    Lpm_waitForSet(ddrInstance->clkChngReq, 1U, 7U);

    Lpm_setPllForDDR(ddrInstance, fsp);

    /* Tells DDR that the new clock is available */
    Lpm_verifiedSet(ddrInstance->clkChngAck, 0x1U, 0x1U, 0U);

    /* Wait for the DDR to complete its change req configuration */
    Lpm_waitForSet(ddrInstance->chngAck, 1U, 7U);

    LPM_CTRL_MMR0_CFG0_WR(ddrInstance->clkChngAck, 0U);

    /* De-assert req */
    LPM_CTRL_MMR0_CFG0_RD_MODIFY_CLR(ddrInstance->chngReq, 0x103U);
}

static void Lpm_ddrUnlockPll(void)
{
    LPM_PLL0_CFG_WR(CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY0, KICK0_UNLOCK);
    LPM_PLL0_CFG_WR(CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY1, KICK1_UNLOCK);
}

static void Lpm_ddrUnlockWkupMmr(uint32_t partition)
{
    LPM_WKUP_CTRL_MMR0_CFG0_WR(8U + ((partition * 0x4000U) + 0x1000U), KICK0_UNLOCK);
    LPM_WKUP_CTRL_MMR0_CFG0_WR(0xCU + ((partition * 0x4000U) + 0x1000U), KICK1_UNLOCK);
}

static void Lpm_ddrUnlockMcuMmr(uint32_t partition)
{
    LPM_MCU_CTRL_MMR0_CFG0_WR(8U + ((partition * 0x4000U) + 0x1000U), KICK0_UNLOCK);
    LPM_MCU_CTRL_MMR0_CFG0_WR(0xCU + ((partition * 0x4000U) + 0x1000U), KICK1_UNLOCK);
}

#if defined(SOC_J7200)
static void Lpm_enableMainIOIsolation(void)
{
    volatile uint32_t timeout = 100000U;

    Lpm_ddrUnlockWkupMmr(6U);
    Lpm_ddrUnlockWkupMmr(7U);

    /* Configure PADCONFIG_73 (H_PMIC_WAKE0) = TX_ENABLE, DS_OUTPUT_ENABLE */
    uint32_t buf = LPM_CTRL_MMR0_CFG0_RD(CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG73);
    buf &= ~(LPM_DSOUT_DIS | LPM_TX_DIS | (0xFU << 0U));
    LPM_CTRL_MMR0_CFG0_WR(CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG73, buf);

    /* Unlock DMSC PWRCTRL registers */
    LPM_DMSC_PWRCTRL_WR(LPM_DMSC_LOCK0_KICK0, LPM_DMSC_KICK0_UNLOCK);
    LPM_DMSC_PWRCTRL_WR(LPM_DMSC_LOCK0_KICK1, LPM_DMSC_KICK1_UNLOCK);

    LPM_DMSC_PWRCTRL_RD_MODIFY_SET(LPM_DMSC_PMCTRL_IO_1, (LPM_GLOBAL_WUEN | LPM_IO_ISOBYPASS));

    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_DEEPSLEEP_CTRL, LPM_FORCE_DS_MAIN);
    LPM_DMSC_PWRCTRL_RD_MODIFY_SET(LPM_DMSC_FW_CTRL_OUT0_SET, 0x02U);

    /* toggle WUCLK */
    LPM_DMSC_PWRCTRL_RD_MODIFY_SET(LPM_DMSC_PMCTRL_IO_1, LPM_WUCLK_CTRL);
    while(((LPM_DMSC_PWRCTRL_RD(LPM_DMSC_PMCTRL_IO_1) & LPM_WUCLK_STATUS) >> 9U) == 0U)
    {
        /* Wait for CLK acknowledgement */
    }
    LPM_DMSC_PWRCTRL_RD_MODIFY_CLR(LPM_DMSC_PMCTRL_IO_1, LPM_WUCLK_CTRL);
    while(((LPM_DMSC_PWRCTRL_RD(LPM_DMSC_PMCTRL_IO_1) & LPM_WUCLK_STATUS) >> 9U) == 1U)
    {
        /* Wait for CLK acknowledgement */
    }

    /* Enable io_isolation */
    LPM_DMSC_PWRCTRL_RD_MODIFY_SET(LPM_DMSC_PMCTRL_IO_1, LPM_IO_ISO_CTRL);

    /* Ensure PMIC_WAKE0 is tri-stated, i.e controlled by IO daisy-chain */
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_CLR(CSL_WKUP_CTRL_MMR_CFG0_MAIN_PWR_CTRL, LPM_WAKE_EN);

    LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_CANUART_WAKE_CTRL, LPM_IO_MAGIC_WORD);
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_CANUART_WAKE_CTRL, LPM_MAGIC_WORD_LOAD_ENABLE);

    /* wait for CAN signal to be 1 in CANUART_WAKE_STAT0/1 */
    while((timeout > 0U) &&
          (LPM_WKUP_CTRL_MMR0_CFG0_RD(CSL_WKUP_CTRL_MMR_CFG0_CANUART_WAKE_STAT1) != 0x01U))
    {
        timeout--;
    }

    if (timeout == 0U)
    {
        Lpm_debugFullPrintf("FAIL: Timed Out while waiting for CANUART magic word latching\n");
        Lpm_hangAbort();
    }

    /* Wait for is_iso status to be 1 */
    while ((timeout > 0U) &&
           ((LPM_DMSC_PWRCTRL_RD(LPM_DMSC_PMCTRL_IO_1) & LPM_IO_ISO_STATUS_0) != LPM_IO_ISO_STATUS_0))
    {
        timeout--;
    }

    if (timeout == 0U)
    {
        Lpm_debugFullPrintf("FAIL: Timedout while waiting for ISOLATION status\n");
        Lpm_hangAbort();
    }
}

#elif defined(SOC_J784S4) || defined(SOC_J742S2)
static void Lpm_bypassBootModeIOs()
{
    /* BOOTMODE05 bypassed */
    LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG95, LPM_ISO_BYPASS_PAD);
    /* BOOTMODE06 bypassed */
    LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG96, LPM_ISO_BYPASS_PAD);
    /* BOOTMODE07 bypassed */
    LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG97, LPM_ISO_BYPASS_PAD);
}

static void Lpm_enableMcuIOIsolation(void)
{
    uint64_t timeout = 10000U;

    /* For WKUP_PADCONFIGS */
    Lpm_ddrUnlockWkupMmr(7U);

    /* UNLOCK partition 5 for PMCTRL_IO, FW_CTRL_OUT etc */
    Lpm_ddrUnlockWkupMmr(5U);

    /* Global IO_ISOBYPASS signal */
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_PMCTRL_IO_0, LPM_IO_ISOBYPASS);

    /* Bypass IO Isolation for WKUP_I2C io pad, We are using it to send commands to PMIC */
    /* 0x00840000 = rx_active AND iso_byp */
    LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG64, (LPM_ISO_BYPASS_PAD | LPM_RX_ACTIVE));
    LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG65, (LPM_ISO_BYPASS_PAD | LPM_RX_ACTIVE));

    /* Bypass IO Isolation for all BOOTMODE pins.
     * This is necessary else the board would not resume.
     * The reason being that the bootmode pins would still be in isolation.
     */
    Lpm_bypassBootModeIOs();

    /* Bypassing IO Isolation for MCU uart, as we want to see the prints when entering IO isolation
     * Uncomment these lines if debug messages are needed
     */
    /*
        LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG60, (LPM_ISO_BYPASS_PAD));
        LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG61, (LPM_ISO_BYPASS_PAD));
        LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG62, (LPM_ISO_BYPASS_PAD));
        LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_PADCONFIG63, (LPM_ISO_BYPASS_PAD));
    */

    /* Enable fw_ctrl_out[0] */
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_FW_CTRL_OUT0_SET, LPM_IO_PM_CTRL_0);

    /* Global DeepSleep enable only for MCU domain */
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_DEEPSLEEP_CTRL, LPM_FORCE_DS_WKUP);

    /* Set global wuen only for WKUP domain */
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_PMCTRL_IO_0, LPM_GLOBAL_WUEN);

    /* Set global isoin for WKUP domain */
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_PMCTRL_IO_0, LPM_IO_ISO_CTRL);

    /* Wait for is_iso status to be 1 */
    while ((timeout > 0U) &&
           ((LPM_WKUP_CTRL_MMR0_CFG0_RD(CSL_WKUP_CTRL_MMR_CFG0_PMCTRL_IO_0) & LPM_IO_ISO_STATUS_0) != LPM_IO_ISO_STATUS_0))
    {
        timeout--;
    }

    if (timeout == 0U) {
        Lpm_debugFullPrintf("FAIL: Timedout while waiting for ISOLATION status\n");
        Lpm_hangAbort();
    }

    /* UNLOCK partition 6 for MCU_GEN_WAKE_CTRL */
    Lpm_ddrUnlockWkupMmr(6U);

	/* Load Magic word into MCU_GEN_WAKE_CTRL register */
    LPM_WKUP_CTRL_MMR0_CFG0_WR(CSL_WKUP_CTRL_MMR_CFG0_MCU_GEN_WAKE_CTRL, LPM_IO_MAGIC_WORD);

	/* Enable the mode */
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_WKUP_CTRL_MMR_CFG0_MCU_GEN_WAKE_CTRL, LPM_MAGIC_WORD_LOAD_ENABLE);

    /* wait for CAN signal to be 1 in CSL_WKUP_CTRL_MMR_CFG0_MCU_GEN_WAKE_STAT0/1 */
    while((timeout > 0U) &&
          (LPM_WKUP_CTRL_MMR0_CFG0_RD(CSL_WKUP_CTRL_MMR_CFG0_MCU_GEN_WAKE_STAT1) != 0x01U))
    {
        timeout--;
    }

    if (timeout == 0U) {
        Lpm_debugFullPrintf("FAIL: Timed Out while waiting for MCU_GEN magic word latching\n");
        Lpm_hangAbort();
    }
}
#endif

static void Lpm_ddrEnterSelfRefresh(void)
{
    uint32_t n;
    uint32_t fspop, fspwr;

    Lpm_ddrUnlockMcuMmr(0U);
    Lpm_ddrUnlockWkupMmr(2U);
    Lpm_ddrUnlockPll();

    /* Going to a new fsp (boot freq) before entering retention */
    for (uint32_t i = 0U; gLpmDdr[i].ddrBase > 0U; i++)
    {
        Lpm_debugFullPrintf("gLpmDdr[i].ddrBase = %x\n", gLpmDdr[i].ddrBase);

        /*
         * Initialization was completed by the PI
         * Initialize the controller to match the current state of the memory
         * MR13 data within the PI is based upon CS but NOT based upon
         * frequency set point
         * MR13 is the current MR13 value
         */
        fspop = (CSL_REG32_RD_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_PI_259) & (1U << 31U)) >> 31U;
        fspwr = (CSL_REG32_RD_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_PI_259) & (1U << 30U)) >> 30U;
        LPM_REG32_RD_MODIFY_SET_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_192, ((fspwr << 24U) | (fspop << 16U)));

        /* do not allow CTL to update MR */
        LPM_REG32_RD_MODIFY_SET_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_191, (1U << 24U));
        /* do not allow PI to update MR */
        LPM_REG32_RD_MODIFY_CLR_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_PI_64, (1U << 0U));

        /*
         * WDQLVL periodic is disabled for F0
         * WDQ does not require training for F0 and therefore also does not
         * require periodic training
         * This can be changed this without synchronization because we are
         * in a different FSP, not FSP0
         */
        LPM_REG32_RD_MODIFY_CLR_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_PI_212, (2U << 8U));

        /* PI_FREQ_MAP defines supported working frequencies */
        n = CSL_REG32_RD_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_PI_12);
        if (n & (0x1U << 1U))
        {
            /* Define FSP0 and FSP1 as trained */
            LPM_REG32_RD_MODIFY_SET_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_190, ((0x1U << 16U) | (0x1U << 8U)));
        }
        if (n & (0x1U << 2U))
        {
            /* Define FSP0 and FSP2 as trained */
            LPM_REG32_RD_MODIFY_SET_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_190, ((0x1U << 24U) | (0x1U << 8U)));
        }

        Lpm_ddrFreqChange(&gLpmDdr[i]);
    }

    Lpm_debugFullPrintf("Lpm_ddrEnterSelfRefresh: Readout PHY regs in boot freq prior to entry into DDR retention\n");
    for (uint32_t i = 0; gLpmDdr[i].ddrBase; i++)
    {
        /* Disable auto entry / exit */
        LPM_REG32_RD_MODIFY_CLR_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_141, ((0xFU << 24U) | (0xFU << 16U)));
        n = CSL_REG32_RD_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_132) & ~(0x7FU << 24U);

        /* Set low power mode to 0x51 */
        CSL_REG32_WR_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_132, (n | (0x51U << 24U)));

        while((CSL_REG32_RD_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_293) & (0x1U << 10U)) == 0)
        {
            /* Wait until low power operation has been completed */
        }

        /* Clear flag by writing t to DDRSS_CTL_295[10] */
        CSL_REG32_WR_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_295, (0x1U << 10U));

        /*
         * Bit 6 / 14 -- lp_state valid
         * Bits 13:8 / 5:0 0x0F SRPD Long with Mem Clk Gating
         */
        while((CSL_REG32_RD_OFF(gLpmDdr[i].ddrBase, CSL_EMIF_CTLCFG_DENALI_CTL_141) & 0x7F7FU) != 0x4F4FU)
        {
            /* Wait for DDR to go into "SRPD Long with Mem Clk Gating" mode */
        }
    }
}

static int Lpm_i2cReadTimeout(char add, uint8_t *rxd, uint32_t timeout)
{
    uint32_t n, loop = 0U;

    /* wait BB --> 0 */
    while(((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 12U)) != 0x00U) && (loop++ < timeout))
    {
        /* Wait for busy bit signal */
    }

    if(loop >= timeout)
    {
        Lpm_debugFullPrintf("Lpm_i2cReadTimeout: timeout loop exceed %d\n", timeout);
        return(-1);
    }

    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_CNT, 1U);
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_CON) & ~0x2U;
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_CON, (n | (3U << 9U)));
    LPM_WKUP_I2C0_CFG_RD_MODIFY_SET(CSL_I2C_CON, (1U << 0U));

    /* wait XRDY --> 1 */
    loop = 0U;
    while(((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 4U)) == 0x00U) && (loop++ < timeout))
    {
        /* Wait for XRDY signal */
    }

    if(loop >= timeout)
    {
        Lpm_debugFullPrintf("Lpm_i2cReadTimeout: timeout for XRDY: loop exceed %d\n", timeout);
        return(-1);
    }

    /* write enable to register */
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_DATA, add);
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_IRQSTATUS, (0x1U << 4U));

    /* wait ARDY --> 1 */
    loop = 0U;
    while(((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 2U)) == 0x00U) && (loop++ < timeout))
    {
        /* Wait for ARDY signal */
    }

    if(loop >= timeout)
    {
        Lpm_debugFullPrintf("Lpm_i2cReadTimeout: timeout for ARDY: loop exceed %d\n", timeout);
        return(-1);
    }

    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_IRQSTATUS, LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW));

    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_CON) & ~(0x1U << 9U);
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_CON, n | (1U << 10U));
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_CNT, 1U);
    LPM_WKUP_I2C0_CFG_RD_MODIFY_SET(CSL_I2C_CON, (3U << 0U));

    /* wait RRDY --> 1 */
    loop = 0;
    while(((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 3U)) == 0x0U) && (loop++ < timeout))
    {
        /* Wait for RRDY signal */
    }

    if(loop >= timeout)
    {
        Lpm_debugFullPrintf("Lpm_i2cReadTimeout: timeout for RRDY: loop exceed %d\n", timeout);
        return(-1);
    }

    *rxd = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_DATA);

    /* wait ARDY --> 1 */
    while(((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 2U)) == 0x0U) && (loop++ < timeout))
    {
        /* Wait for ARDY signal */
    }

    if(loop >= timeout)
    {
        Lpm_debugFullPrintf("Lpm_i2cReadTimeout: timeout for ARDY: loop exceed %d\n", timeout);
        return(-1);
    }

    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_IRQSTATUS, LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW));

    return(0);
}

static uint8_t Lpm_i2cRead(char add)
{
    uint8_t rxData;

    Lpm_i2cReadTimeout(add, &rxData, 0xFFFFFFFFU);

    return(rxData);
}

static void Lpm_i2cWrite(char add, char data)
{
    uint32_t n;

    /* wait BB --> 0 */
    while((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 12U)) != 0x0U)
    {
        /* Wait for busy bit signal */
    }

    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_CNT, 2U);
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_CON) & ~0x2U;
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_CON, (n | (3U << 9U)));
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_CON);
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_CON, (n | (3U << 0U)));

    for(n = 0; n < 2U; n++)
    {
        /* wait XRDY --> 1 */
        while((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 4U)) == 0x00U)
        {
            /* Wait for XRDY signal */
        }

        if(n==0)
        {
            /* write enable to register lock */
            LPM_WKUP_I2C0_CFG_WR(CSL_I2C_DATA, add);
        }
        else
        {
            /* write enable to register lock */
            LPM_WKUP_I2C0_CFG_WR(CSL_I2C_DATA, data);
            LPM_WKUP_I2C0_CFG_WR(CSL_I2C_IRQSTATUS, (0x1U << 4U));
        }
    }

    /* wait ARDY --> 1 */
    while((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW) & (0x1U << 2U)) == 0x00U)
    {
        /* Wait for ARDY signal */
    }

    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_IRQSTATUS, LPM_WKUP_I2C0_CFG_RD(CSL_I2C_IRQSTATUS_RAW));
}

static void Lpm_i2cConfigWkup(char pmic)
{
    static char current_pmic;
    uint32_t n;

    /*
     * This is an optimization to prenvent setting again the I2C
     * when it's not needed
     */
    if (pmic == current_pmic)
        return;

    current_pmic = pmic;

    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG62, (1U << 18U));
    LPM_WKUP_CTRL_MMR0_CFG0_RD_MODIFY_SET(CSL_MAIN_CTRL_MMR_CFG0_PADCONFIG63, (1U << 18U));

    /* Reset the I2C */
    LPM_WKUP_I2C0_CFG_RD_MODIFY_SET(CSL_I2C_SYSC, (1U << 1U));
    LPM_WKUP_I2C0_CFG_RD_MODIFY_SET(CSL_I2C_SYSC, (1U << 3U));

    /* Enable I2C */
    LPM_WKUP_I2C0_CFG_RD_MODIFY_SET(CSL_I2C_CON, (1U << 15U));
    while((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_SYSS) & 0x1U) == 0x0U)
    {
        /* Wait for I2C enable done */
    }

    /* Set divider = 2 */
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_PSC) & ~0xFFU;
    /* 96/(7+1) = 12MHz */
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_PSC, (n | 0x7U));

    /* 9.6MHz/(n1 + n2) = 0.4 -- > n1 + n2 = 24 */
    /* Set SCLL 7 + 9 = 16 */
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_SCLL) & ~0xFFU;
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_SCLL, (n | 0x9U));
    /* Set SCLH 5 + 9 = 14 */
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_SCLH) & ~0xFFU;
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_SCLH, (n | 0x9U));

    LPM_WKUP_I2C0_CFG_RD_MODIFY_CLR(CSL_I2C_CON, 0xCF03U);
    LPM_WKUP_I2C0_CFG_RD_MODIFY_CLR(CSL_I2C_CNT, 0xFFFFU);
    LPM_WKUP_I2C0_CFG_RD_MODIFY_CLR(CSL_I2C_BUF, 0x8080U);

    /* Set own address = 0xB4 */
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_OA) & ~0x3FFU;
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_OA, (n | 0xB4U));

    /* Enable I2C */
    LPM_WKUP_I2C0_CFG_RD_MODIFY_SET(CSL_I2C_CON, (1U << 15U));
    while((LPM_WKUP_I2C0_CFG_RD(CSL_I2C_SYSS) & 0x1U) == 0x0U)
    {
        /* Wait for I2C enable done */
    }

    /* Set PMIC ADDRESS */
    n = LPM_WKUP_I2C0_CFG_RD(CSL_I2C_SA) & ~0x3FFU;
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_SA, (n | pmic));

    /* Set TX / RX threshold to 1 [5:0] = 1-1; [13:8] = 1-1 */
    LPM_WKUP_I2C0_CFG_WR(CSL_I2C_BUF, ((1U << 6U) | (1U << 14U)));

    /* Write enable */
    Lpm_i2cWrite(0xA1U, 0x9BU);
}

static void Lpm_writePmicA(uint8_t reg, uint8_t val)
{
    Lpm_i2cConfigWkup(BOARD_I2C_LEO_PMIC_A_ADDR);
    Lpm_i2cWrite(reg, val);
    Lpm_debugFullPrintf("Lpm_writePmicA: reg=0x%x 0x%x\n", reg, val);
}

static void Lpm_writePmicB(uint8_t reg, uint8_t val)
{
    Lpm_i2cConfigWkup(BOARD_I2C_LEO_PMIC_B_ADDR);
    Lpm_i2cWrite(reg, val);
    Lpm_debugFullPrintf("Lpm_writePmicB: reg=0x%x 0x%x\n", reg, val);
}

static uint8_t Lpm_readPmicA(uint8_t reg)
{
    uint8_t rxd;

    Lpm_i2cConfigWkup(BOARD_I2C_LEO_PMIC_A_ADDR);
    rxd = Lpm_i2cRead(reg);
    Lpm_debugFullPrintf("Lpm_readPmicA: reg=0x%x 0x%x\n", reg, rxd);

    return(rxd);
}

static uint8_t Lpm_readPmicB(uint8_t reg)
{
    uint8_t rxd;

    Lpm_i2cConfigWkup(BOARD_I2C_LEO_PMIC_B_ADDR);
    rxd = Lpm_i2cRead(reg);
    Lpm_debugFullPrintf("Lpm_readPmicB: reg=0x%x 0x%x\n", reg, rxd);

    return(rxd);
}

static uint8_t Lpm_readPmicB_timeout(uint8_t reg, uint8_t *rxData, int timeout)
{
    int ret;

    Lpm_i2cConfigWkup(BOARD_I2C_LEO_PMIC_B_ADDR);
    ret = Lpm_i2cReadTimeout(reg, rxData, timeout);
    Lpm_debugFullPrintf("Lpm_readPmicB_timeout: reg=0x%x 0x%x ret=%d timeout=%d\n",
                            reg, *rxData, ret, timeout);

    return(ret);
}

static void Lpm_ClearPmicInterrupts(void)
{
    uint8_t int_top, val, val1;

    int_top = Lpm_i2cRead(LPM_INT_TOP);

    if(!int_top)
    {
        return;
    }

    if(int_top & LPM_INT_TOP_BUCK)
    {
        val = Lpm_i2cRead(LPM_INT_BUCK_STATUS);
        Lpm_debugFullPrintf("INT_BUCK = 0x%02X\n", val);
        if (val & LPM_INT_BUCK1_2)
        {
            val1 = Lpm_i2cRead(LPM_INT_BUCK1_2_STATUS);
            Lpm_debugFullPrintf("INT_BUCK1_2 = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_BUCK1_2_STATUS, val1);
        }

        if(val & LPM_INT_BUCK3_4)
        {
            val1 = Lpm_i2cRead(LPM_INT_BUCK3_4_STATUS);
            Lpm_debugFullPrintf("INT_BUCK3_4 = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_BUCK3_4_STATUS, val1);
        }

        if(val & LPM_INT_BUCK5)
        {
            val1 = Lpm_i2cRead(LPM_INT_BUCK5_STATUS);
            Lpm_debugFullPrintf("INT_BUCK5 = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_BUCK5_STATUS, val1);
        }
    }

    if(int_top & LPM_INT_TOP_LDO_VMON)
    {
        val = Lpm_i2cRead(LPM_INT_LDO_VMON_STATUS);
        Lpm_debugFullPrintf("INT_LDO_VMON = 0x%02X\n", val);
        if(val & LPM_INT_LDO1_2)
        {
            val1 = Lpm_i2cRead(LPM_INT_LDO1_2_STATUS);
            Lpm_debugFullPrintf("INT_LDO1_2 = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_LDO1_2_STATUS, val1);
        }
        if(val & LPM_INT_LDO3_4)
        {
            val1 = Lpm_i2cRead(LPM_INT_LDO3_4_STATUS);
            Lpm_debugFullPrintf("INT_LDO3_4 = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_LDO3_4_STATUS, val1);
        }
        if(val & LPM_INT_VMON)
        {
            val1 = Lpm_i2cRead(LPM_INT_VMON_STATUS);
            Lpm_debugFullPrintf("INT_VMON = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_VMON_STATUS, val1);
        }
    }

    if(int_top & LPM_INT_TOP_GPIO)
    {
        val = Lpm_i2cRead(LPM_INT_GPIO_REGADDR);
        Lpm_debugFullPrintf("INT_GPIO = 0x%02X\n", val);
        if(val & LPM_INT_GPIO1_8)
        {
            val1 = Lpm_i2cRead(LPM_INT_GPIO1_8_REGADDR);
            Lpm_debugFullPrintf("INT_GPIO1_8 = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_GPIO1_8_REGADDR, val1);
        }
        /* Clear GPIO9, GPIO10, GPIO11 */
        Lpm_i2cWrite(LPM_INT_GPIO_REGADDR, val);
    }

    if(int_top & LPM_INT_TOP_STARTUP)
    {
        val = Lpm_i2cRead(LPM_INT_STARTUP_REGADDR);
        Lpm_debugFullPrintf("INT_STARTUP = 0x%02X\n", val);
        Lpm_i2cWrite(LPM_INT_STARTUP_REGADDR, val);
    }

    if(int_top & LPM_INT_TOP_MISC)
    {
        val = Lpm_i2cRead(LPM_INT_MISC_REGADDR_STATUS);
        Lpm_debugFullPrintf("INT_MISC = 0x%02X\n", val);
        Lpm_i2cWrite(LPM_INT_MISC_REGADDR_STATUS, val);
    }

    if(int_top & LPM_INT_TOP_MODERATE_ERR)
    {
        val = Lpm_i2cRead(LPM_INT_MODERATE_ERR_STATUS);
        Lpm_debugFullPrintf("INT_MODERATE_ERR = 0x%02X\n", val);
        Lpm_i2cWrite(LPM_INT_MODERATE_ERR_STATUS, val);
    }

    if(int_top & LPM_INT_TOP_SEVERE_ERR)
    {
        val = Lpm_i2cRead(LPM_INT_SEVERE_ERR_STATUS);
        Lpm_debugFullPrintf("INT_SEVERE_ERR = 0x%02X\n", val);
        Lpm_i2cWrite(LPM_INT_SEVERE_ERR_STATUS, val);
    }

    if(int_top & LPM_INT_TOP_FSM_ERR)
    {
        val = Lpm_i2cRead(LPM_INT_FSM_ERR_STATUS);
        Lpm_debugFullPrintf("INT_FSM_ERR = 0x%02X\n", val);
        if(val & LPM_INT_COMM_ERR)
        {
            val1 = Lpm_i2cRead(LPM_INT_COMM_ERR_STATUS);
            Lpm_debugFullPrintf("INT_COMM_ERR = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_COMM_ERR_STATUS, val1);
        }
        if(val & LPM_INT_READBACK_ERR)
        {
            val1 = Lpm_i2cRead(LPM_INT_READBACK_ERR_STATUS);
            Lpm_debugFullPrintf("INT_READBACK_ERR = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_READBACK_ERR_STATUS, val1);
        }
        if(val & LPM_INT_ESM)
        {
            val1 = Lpm_i2cRead(LPM_INT_ESM_ERR_STATUS);
            Lpm_debugFullPrintf("INT_ESM = 0x%02X\n", val1);
            Lpm_i2cWrite(LPM_INT_ESM_ERR_STATUS, val1);
        }
        /* Clear INT_FSM_ERR */
        Lpm_i2cWrite(LPM_INT_FSM_ERR_STATUS, val);
    }

    int_top = Lpm_i2cRead(LPM_INT_TOP);
    Lpm_debugFullPrintf("INT_TOP = 0x%02X\n", int_top);
}

/* Taken from Lpm_pmicStateChangeActiveToIORetention, with some changes to
 * LPM_FSM_I2C_TRIGGERS for DDRRET support. Explaination on this change is still
 * obscure. */
static void Lpm_setupPmic(uint32_t mode)
{
    /* Write 0x02 to FSM_NSLEEP_TRIGGERS register
       This should happen before clearing the interrupts */

    /* If you clear the interrupts before you write the NSLEEP bits,
     * it will transition to S2R state.
     * This is because as soon as you write NSLEEP2 to 0x0,
     * the trigger is present to move to S2R state.
     * By setting the NSLEEP bits before you clear the interrupts,
     * you can configure both NSLEEP bits before the PMIC reacts to the change.
     */

    /* Change FSM_NSLEEP_TRIGGERS: NSLEEP1=high, NSLEEP2=high */
    Lpm_writePmicA(PMIC_FSM_NSLEEP_TRIGGERS_REGADDR, 0x03U);
    Lpm_debugFullPrintf("Lpm_setupPmic: Write FSM_NSLEEP_TRIGGERS\n");
    Lpm_debugReadPmicA(PMIC_FSM_NSLEEP_TRIGGERS_REGADDR);

    /* Clear interrupts */
    Lpm_i2cConfigWkup(BOARD_I2C_LEO_PMIC_A_ADDR);
    Lpm_ClearPmicInterrupts();
    #if defined(SOC_J7200)
    Lpm_writePmicB(LPM_INT_STARTUP_REGADDR, 0x02U);
    #endif

    if (mode == LPM_SOC_OFF)
    {
        /* Change SCICLIENT_LPM_FSM_I2C_TRIGGERS */
        Lpm_writePmicA(PMIC_FSM_I2C_TRIGGERS_REGADDR, LPM_FSM_I2C_TRIGGERS);
        #if defined(SOC_J7200)
        Lpm_writePmicB(PMIC_FSM_I2C_TRIGGERS_REGADDR, LPM_FSM_I2C_TRIGGERS);
        #endif
        Lpm_debugFullPrintf("Lpm_setupPmic: Write FSM_TRIGGERS\n");
        Lpm_debugReadPmicA(PMIC_FSM_I2C_TRIGGERS_REGADDR);

    }
    else if (mode == LPM_IO_ONLY_PLUS_DDR)
    {
        #if defined(SOC_J7200)
        /* J7200 has two PMICs and GPIO_RET_I2C_6 sets the io retention state */
        Lpm_writePmicA(PMIC_FSM_I2C_TRIGGERS_REGADDR, (LPM_DDR_RET_I2C_7 | LPM_GPIO_RET_I2C_6));
        Lpm_writePmicB(PMIC_FSM_I2C_TRIGGERS_REGADDR, (LPM_DDR_RET_I2C_7 | LPM_GPIO_RET_I2C_6));
        #elif defined(SOC_J784S4)|| defined(SOC_J742S2)
        /* J784s4 has one PMIC and GPIO_RET_I2C_5 sets the io retention state */
        Lpm_writePmicA(PMIC_FSM_I2C_TRIGGERS_REGADDR, (LPM_DDR_RET_I2C_7 | LPM_GPIO_RET_I2C_5));
        #endif
    }

    /* Configure GPIO4_CONF: input, pull-down, signal LP_WKUP1 */
    Lpm_writePmicA(LPM_GPIO4_CONF_REGADDR, 0xCAU);
    Lpm_debugFullPrintf("Lpm_setupPmic: Write GPIO4_CONF\n");
    Lpm_debugReadPmicA(LPM_GPIO4_CONF_REGADDR);

    /* Configure INT_GPIO1_8 (enable GPIO4 interrupt): clear GPIO4_INT */
    Lpm_writePmicA(LPM_INT_GPIO1_8_REGADDR, LPM_GPIO4_BIT);
    Lpm_debugFullPrintf("Lpm_setupPmic: Write INT_GPIO1_8\n");
    Lpm_debugReadPmicA(LPM_INT_GPIO1_8_REGADDR);

    if (mode == LPM_SOC_OFF)
    {
        /* Configure MASK_GPIO*_RISE */
        Lpm_writePmicA(LPM_MASK_GPIO1_8_RISE_REGADDR, LPM_GPIO1_8_RISE);
        Lpm_writePmicA(0x51U, 0x3FU);

        /* Configure MASK_SCICLIENT_LPM_GPIO1_8_FALL (configure GPIO4 falling edge interrupt): enable INT on GPIO4 */
        Lpm_writePmicA(LPM_MASK_GPIO1_8_FALL_REGADDR, LPM_GPIO1_8_FALL);
        Lpm_debugReadPmicA(LPM_MASK_GPIO1_8_FALL_REGADDR);

    }
    else if (mode == LPM_IO_ONLY_PLUS_DDR)
    {
        /* Mask all rising interrupts */
        Lpm_writePmicA(LPM_MASK_GPIO1_8_RISE_REGADDR, 0xFFU);

        /* Unmask gpio4 falling interrupts */
        Lpm_writePmicA(LPM_MASK_GPIO1_8_FALL_REGADDR, 0xF7U);
    }

    /*
     * Configure DDR_RET pin
     * The sequence is different for each board.
     */
    uint8_t buf;

#if defined(SOC_J7200)
	    /* Put GPIO2 0x32 and GPIO3 0x33 as output push-pull no pull-up or pull down */
	    Lpm_writePmicB(LPM_GPIO2_CONF, (1U << LPM_DIR_SHIFT) | (0U << LPM_OD_SHIFT));
	    Lpm_writePmicB(LPM_GPIO3_CONF, (1U << LPM_DIR_SHIFT) | (0U << LPM_OD_SHIFT));
	    /* GPIO_OUT_1 */
	    buf = Lpm_readPmicB(LPM_GPIO_OUT_1_REGADDR) | LPM_DDR_RET_VAL;
	    Lpm_writePmicB(LPM_GPIO_OUT_1_REGADDR, buf);
	    Lpm_writePmicB(LPM_GPIO_OUT_1_REGADDR, buf | LPM_DDR_RET_CLK);
	    Lpm_writePmicB(LPM_GPIO_OUT_1_REGADDR, buf & ~LPM_DDR_RET_CLK);
	    Lpm_writePmicB(LPM_GPIO_OUT_1_REGADDR, buf | LPM_DDR_RET_CLK);
#else
	    /* Put GPIO6 as output push-pull no pull-up or pull down */
	    Lpm_writePmicA(LPM_GPIO6_CONF, (1U << LPM_DIR_SHIFT) | (0U << LPM_OD_SHIFT));
	    /* GPIO_OUT_1 */
	    buf = Lpm_readPmicA(LPM_GPIO_OUT_1_REGADDR) | LPM_EN_DDR_RET_1V1;
	    Lpm_writePmicA(LPM_GPIO_OUT_1_REGADDR, buf);
#endif

    /* Write magic number to scratch register to indicate the suspend */
    Lpm_writePmicA(LPM_SCRATCH_PAD_REG_3, LPM_MAGIC_SUSPEND);
    Lpm_debugReadPmicA(LPM_SCRATCH_PAD_REG_3);

    Lpm_debugReadPmicA(LPM_INT_TOP);

    /* Change FSM_NSLEEP_TRIGGERS: NSLEEP1=low, NSLEEP2=low */
    Lpm_writePmicA(PMIC_FSM_NSLEEP_TRIGGERS_REGADDR, 0x00U);
}

#ifdef LPM_DEBUG_SRAM_S2R
/**
 * \brief   This function writes a byte on the serial console
 *
 * This function is based on UART_putc from ti/drv/uart/src/UART_stdio.c.
 *
 * \param   baseAdd  Address of the UART port to use.
 *          byteTx   The byte to be transmitted.
 *
 */
static void _Lpm_debugPutC(uint32_t baseAdd, uint8_t byteTx)
{
    uint32_t lcrRegValue = CSL_REG32_RD_OFF(baseAdd, UART_LCR);

    CSL_REG32_WR_OFF(baseAdd, UART_LCR, lcrRegValue & 0x7FU);
    /*
    ** Waits indefinitely until the THR and Transmitter Shift Registers are
    ** empty.
    */
    while(((uint32_t) UART_LSR_TX_SR_E_MASK |
           (uint32_t) UART_LSR_TX_FIFO_E_MASK) !=
          (CSL_REG32_RD_OFF(baseAdd, UART_LSR) &
           ((uint32_t) UART_LSR_TX_SR_E_MASK |
            (uint32_t) UART_LSR_TX_FIFO_E_MASK)))
    {
        /* Busy wait */
    }

    CSL_REG32_WR_OFF(baseAdd, UART_THR, byteTx);
    CSL_REG32_WR_OFF(baseAdd, UART_LCR, lcrRegValue);
}

/**
 * \brief  Wrapper function to call _Lpm_debugPutC
 *
 * \param  byteTx The byte to be transmitted.
 *
 */
static void Lpm_debugPutC(uint8_t byteTx)
{
#ifndef LPM_DEBUG_UART_MAIN
    static bool Lpm_uartConfigured = false;
    if (Lpm_uartConfigured == false)
    {
        CSL_REG32_WR_OFF(CSL_MCU_UART0_BASE, UART_LCR,
                         UART_FRAME_WORD_LENGTH_8 | UART_FRAME_NUM_STB_1 | UART_PARITY_NONE);
	Lpm_uartConfigured = true;
    }
#endif
    _Lpm_debugPutC(LPM_UART_ADDR, byteTx);
}

/**
 * \brief  This function writes data from a specified buffer onto the transmitter
 *         FIFO of UART
 *
 * \param  str  Pointer to a buffer to transmit.
 *         len  Number of bytes to be transmitted.
 *
 */
static void Lpm_debugPutS(const char *str, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len && str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
           Lpm_debugPutC('\r');
        }
        Lpm_debugPutC(str[i]);
    }
}

/**
 * \brief This function calls to the common procedure in Lpm_debugPrintf.
 *        This functions writes the output in hex format.
 *
 * This function is a partial copy of UART_convertVal from
 * ti/drv/uart/src/UART_stdio.c. Only the hexadecimal support was kept.
 *
 * \param ulValue  The value of the resolved variable passed as argument
 *        ulPos    The buffer position of the printed value
 *        ulCount  Total number of character to be printed
 *        cFill    The char to be placed between number
 *        pcBuf    buffer storing all the numbers
 *
 * \return 0.
 *
 */
static int32_t Lpm_debugConvertVal(uint32_t ulValue, uint32_t ulPos, uint32_t ulCount, char cFill, char *pcBuf)
{
    const char *const g_pcHex = "0123456789abcdef";
    uint32_t ulBase = 16;
    uint32_t ulIdx;
    uint32_t count = ulCount;
    uint32_t pos = ulPos;

    for (ulIdx = 1u;
         (((ulIdx * ulBase) <= ulValue) &&
          (((ulIdx * ulBase) / ulBase) == ulIdx));
         (ulIdx = (ulIdx * ulBase)))
    {
        count--;
    }

    /* Provide additional padding at the beginning of the
     * string conversion if needed. */
    if ((count > 1u) && (count < 16u))
    {
        for (count--; count != 0U; count--)
        {
            pcBuf[pos] = cFill;
            pos++;
        }
    }

    /* Convert the value into a string. */
    for (; ulIdx != 0U; ulIdx /= ulBase)
    {
        pcBuf[pos] = g_pcHex[(ulValue / ulIdx) % ulBase];
        pos++;
    }

    /* Write the string. */
    (void)Lpm_debugPutS(pcBuf, pos);

    return 0;
}

static void Lpm_debugPrintf(const char *pcString, ...)
{
    uint32_t ulIdx, ulValue, ulPos, ulCount;
    char    pcBuf[16], cFill;
    va_list  vaArgP;
    const char *pStr = pcString;

    /* Start the varargs processing. */
    (void)va_start(vaArgP, pcString);

    /* Loop while there are more characters in the string. */
    while (*pStr != (char)0U)
    {
        /* Find the first non-% character, or the end of the string. */
        for (ulIdx = 0;
             (pStr[ulIdx] != (char) '%') &&
             (pStr[ulIdx] != (char) '\0');
             ulIdx++)
        {
            /* Scan for a null terminator or character that is not '%' */
        }

        /* Write this portion of the string. */
        (void)Lpm_debugPutS(pStr, ulIdx);

        /* Skip the portion of the string that was written. */
        pStr += ulIdx;

        /* See if the next character is a %. */
        if (*pStr == (char) '%')
        {
            /* Skip the %. */
            pStr++;

            /* Set the digit count to zero, and the fill character to space
             * (i.e. to the defaults). */
            ulCount = 0;
            cFill   = (char) ' ';

            /* Determine how to handle the next character. */ 
            while((*pStr >= (char)'0') && (*pStr <= (char)'9'))
            {
                /* If this is a zero, and it is the first digit, then the
                 * fill character is a zero instead of a space. */
                if ((pStr[-1] == (char) '0') && (ulCount == 0U))
                {
                    cFill = (char) '0';
                }

                /* Update the digit count. */
                ulCount *= 10u;
                ulCount += ((uint32_t)(*pStr)) - (uint32_t) '0';

                /* Get the next character. */
                pStr++;
            }
            switch (*pStr)
            {
                /* Handle the %x and %X commands.  Note that they are treated
                 * identically; i.e. %X will use lower case letters for a-f
                 * instead of the upper case letters is should use.  We also
                 * alias %p to %x. */
                case (char) 'x':
                case (char) 'X':
                case (char) 'p':
                {
                    /* Get the value from the varargs. */
                    ulValue = (uint32_t)va_arg(vaArgP, uint32_t);

                    /* Reset the buffer position. */
                    ulPos = 0;

                    /* Determine the number of digits in the string version of
                     * the value. */
                    (void)Lpm_debugConvertVal(ulValue, ulPos, ulCount, cFill, pcBuf);
 
                    break;
                }

                /* Handle all other commands. */
                default:
                {
                    /* Indicate an error. */
                    (void)Lpm_debugPutS("ERROR", 5u);

                    /* This command has been handled. */
                    break;
                }
            }
            pStr++;
        }
    }

    /* End the varargs processing. */
    va_end(vaArgP);
}

#else
static void Lpm_debugPrintf(const char *pcString, ...)
{
    /* Incase the LPM_DEBUG_SRAM_S2R is not defined, this function will be compiled */
}
#endif /* LPM_DEBUG_SRAM_S2R */
