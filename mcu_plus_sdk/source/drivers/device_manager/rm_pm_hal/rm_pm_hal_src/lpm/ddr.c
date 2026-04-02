/*
 * DM Stub Firmware
 *
 * LPM DDR driver
 *
 * Copyright (C) 2021-2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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

#include <lib/bitops.h>
#include <types/errno.h>
#include <types/short_types.h>
#include <types/sbool.h>
#include <baseaddress.h>
#include <wkup_ctrl_mmr.h>
#include "ddr.h"
#include "timeout.h"
#include <pll.h>
#ifdef CONFIG_LPM_32_BIT_DDR
#include "cdns_ddr_reg_config.h"
#include "lpm_io.h"
#include "cslr_emif.h"
#include "DDRSS_addr_map_sfr_offs_ew_32bit.h"
#else
#include <lib/io.h>
#endif

#define DDR_RETRAIN_TIMEOUT 10000000U

/* Cadence DDR registers */
#define CDNS_DENALI_PHY_1333                                    0x54d4U
#define CDNS_DENALI_PHY_1333_PHY_CAL_START_0                    0x01000000U

#define CDNS_DENALI_PHY_1336                                    0x54e0U
#define CDNS_DENALI_PHY_1336_PHY_CAL_RESULT_OBS_0               (BIT(23))

#define CDNS_DENALI_PHY_1364                                    0x5550U
#define CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_MASK        0x7U
#define CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_SHIFT       0x8U

#define CDNS_DENALI_PHY_1369                                    0x5564U
#define CDNS_DENALI_PHY_1369_PHY_UPDATE_MASK                    0x1U

#define CDNS_DENALI_PI_13                                       0x2034U
#define CDNS_DENALI_PI_13_CS_MAP_MASK                           0x00030000U
#define CDNS_DENALI_PI_13_CS_MAP_SHIFT                          16U

#define CDNS_DENALI_PI_23                                       0x205CU
#define CDNS_DENALI_PI_23_WRLVL_REQ                             0x01000000U

#define CDNS_DENALI_PI_24                                       0x2060U
#define CDNS_DENALI_PI_24_WRLVL_CS                              0x00000100U

#define CDNS_DENALI_PI_25                                       0x2064U
#define CDNS_DENALI_PI_25_WRLVL_AUTO_REQ                        0x00010000U

#define CDNS_DENALI_PI_43                                       0x20ACU
#define CDNS_DENALI_PI_43_RDLVL_AUTO_REQ                        0x00000100U
#define CDNS_DENALI_PI_43_RDLVL_GATE_AUTO_REQ                   0x01000000U

#define CDNS_DENALI_PI_55                                       0x20DCU
#define CDNS_DENALI_PI_55_CALVL_AUTO_REQ                        0x00000100U

#define CDNS_DENALI_PI_33                                       0x2084U
#define CDNS_DENALI_PI_33_RDLVL_GATE_REQ                        0x01000000U
#define CDNS_DENALI_PI_33_RDLVL_REQ                             0x00010000U

#define CDNS_DENALI_PI_34                                       0x2088U
#define CDNS_DENALI_PI_34_RDLVL_CS                              0x00000100U

#define CDNS_DENALI_PI_83                                       0x214CU
#define CDNS_DENALI_PI_83_LVL_DONE_BIT                          0x2000U

#define CDNS_DENALI_PI_84                                       0x2150U
#define CDNS_DENALI_PI_84_INT_ACK_REG_MASK                      0x1FFFFFFFU

#define CDNS_DENALI_CTL_0                                       0x0000U
#define CDNS_DENALI_CTL_0_DRAM_CLASS_MASK                       0x00000F00U
#define CDNS_DENALI_CTL_0_DRAM_CLASS_SHIFT                      0x00000008U

#define CDNS_DENALI_CTL_0_DRAM_CLASS_DDR4                       0xAU
#define CDNS_DENALI_CTL_0_DRAM_CLASS_LPDDR4                     0xBU

#define CDNS_DENALI_CTL_158                                     0x278U
#define CDNS_DENALI_CTL_158_LP_CMD_MASK                         0xFF00U
#define CDNS_DENALI_CTL_158_LP_CMD_SUSPEND                      0x5100U
#define CDNS_DENALI_CTL_158_LP_CMD_RESUME                       0x0200U

#define CDNS_DENALI_CTL_336                                     0x540U
#define CDNS_DENALI_CTL_336_NO_TIMEOUT_ERROR                    0x0U

#define CDNS_DENALI_PHY_1306                                    0x5468U
#define CDNS_DENALI_PHY_1306_PHY_SET_DFI_INPUT_0                0x1U

#ifdef CONFIG_LPM_32_BIT_DDR

#define DDRSS_PI_REGISTER_BLOCK__OFFS   0x2000U
#define DDRSS_Data_Slice_0_REGISTER_BLOCK__OFFS 0x4000U
#define DDRSS_Data_Slice_1_REGISTER_BLOCK__OFFS 0x4400U
#define DDRSS_Data_Slice_2_REGISTER_BLOCK__OFFS 0x4800U
#define DDRSS_Data_Slice_3_REGISTER_BLOCK__OFFS 0x4c00U
#define DDRSS_Address_Slice_0_REGISTER_BLOCK__OFFS      0x5000U
#define DDRSS_Address_Slice_1_REGISTER_BLOCK__OFFS      0x5400U
#define DDRSS_Address_Slice_2_REGISTER_BLOCK__OFFS      0x5800U
#define DDRSS_PHY_Core_REGISTER_BLOCK__OFFS     0x5c00U

#define SDRAM_IDX  0x12
#define REGION_IDX 0x12
#define CSL_EMIF_SSCFG_V2A_CTL_REG                                             (0x00000020U)

struct emif_handle_s Emifhandle;

static void configure_sdram_region_idx(struct emif_handle_s *h, u32 sdram_idx, u32 region_idx)
{
	u32 rd_val;

	rd_val = SOC_read32(h->ss_cfg_base_addr + CSL_EMIF_SSCFG_V2A_CTL_REG);
	rd_val = (rd_val & 0xFFFFFC00U);
	rd_val = rd_val | (sdram_idx << 5) | (region_idx);
	SOC_write32((h->ss_cfg_base_addr + CSL_EMIF_SSCFG_V2A_CTL_REG), rd_val); /* Programming the region_idx and sdram_idx fields for address mapping [Set 9:5 and 4:0 to 0x11 for 8GB] */
}

static void start_PI_CTL_init(struct emif_handle_s *h)
{
	u32 wr_init_val;
	u32 i;

	if (h->is_ddr4_mem == 1U) {
		wr_init_val = ((DDR4_DRAM_CLASS_REG_VALUE << 8U) | 0x1U);
	} else { wr_init_val = ((LPDDR4_DRAM_CLASS_REG_VALUE << 8U) | 0x1U); }                                                  /* Decide init value based on memory type */
	SOC_write32(h->ctl_cfg_base_addr + (u32) DDRSS_PI_REGISTER_BLOCK__OFFS + (u32) DENALI_PI_0__SFR_OFFS, wr_init_val);     /* Set START bit in register for PI module */
	for (i = 0; i < 500U; i++) {
		delay_1us();
	}
	SOC_write32(h->ctl_cfg_base_addr + (u32) DENALI_CTL_0__SFR_OFFS, wr_init_val); /* Set START bit in register for controller */
}

static void poll_for_init_completion(struct emif_handle_s *h)
{
#if defined(CTL_INIT_ONLY)
	while (((SOC_read32(h->ctl_cfg_base_addr + DENALI_CTL_350__SFR_OFFS)) & 0x02000000) != 0x02000000) { /* Poll for CTL Init completion */
	}
#elif defined(PI_INIT_ONLY)
	while (((SOC_read32(h->ctl_cfg_base_addr + DDRSS_PI_REGISTER_BLOCK__OFFS + DENALI_PI_87__SFR_OFFS)) & 0x1) != 0x1) { /* Poll for PI Init completion */
	}
#else
	while (((SOC_read32(h->ctl_cfg_base_addr + (u64) DDRSS_PI_REGISTER_BLOCK__OFFS + (u64) DENALI_PI_87__SFR_OFFS)) & 0x1U) != 0x1U) {      /* Poll for PI Init completion */
	}
	while (((SOC_read32(h->ctl_cfg_base_addr + (u64) DENALI_CTL_350__SFR_OFFS)) & 0x02000000U) != 0x02000000U) {                            /* Poll for CTL Init completion */
	}
#endif
}

static void put_ddrss_in_data_retention_thru_wkup_mmr(u32 enable)
{
	u32 val = 0U;

	/* Write into data_retention MMR to activate or deactivate DDR data retention */
	writel(enable, WKUP_CTRL_MMR_BASE + DDR16SS_PMCTRL);

	/* Write `1' into data_ret_ld[31] MMR to generate a LD signal to latch the retention signal */
	writel((((DDR16SS_DATA_RET_LD_OPEN << DDR16SS_DATA_RET_LD_BIT) | enable)), WKUP_CTRL_MMR_BASE + DDR16SS_PMCTRL);

	val = readl(WKUP_CTRL_MMR_BASE + DDR16SS_PMCTRL);
	while (val != ((DDR16SS_DATA_RET_LD_OPEN << DDR16SS_DATA_RET_LD_BIT) | enable)) {
		val = readl(WKUP_CTRL_MMR_BASE + DDR16SS_PMCTRL);
	}

	/* Writes `0' into data_ret_ld[31] to close the latch */
	writel((((DDR16SS_DATA_RET_LD_CLOSE << DDR16SS_DATA_RET_LD_BIT) | enable)), WKUP_CTRL_MMR_BASE + DDR16SS_PMCTRL);
}

static void enter_lpm_self_refresh(void)
{
	u32 lp_status = 0;

	/* Program Self Refresh mode */
	writel((LP_MODE_LONG_SELF_REFRESH << 8), DDRSS0_CTRL_BASE + (u32) DENALI_CTL_160__SFR_OFFS);

	while (lp_status != STATUS_SR_LONG_ENTERED) {
		lp_status = (readl(DDRSS0_CTRL_BASE + (u32) DENALI_CTL_169__SFR_OFFS) & 0x7F00U);
	}
}

static void emif_instance_select(u32 instance, struct emif_handle_s *h)
{
	/* Assign Instance number */
	h->dinum = instance;

	/* Assign memory type */
#if defined(DDR4_MEM)
	h->is_ddr4_mem = 1;
#else
	h->is_ddr4_mem = 0;
#endif

	switch (instance) {
	case 0:
		/* Global Base Addresses [2GB, 2GB, 4GB, 8GB] */
		h->glb_low_base_0 = (u64) DDRSS0_MEM_BASE;
		h->glb_hi0_base_0 = (u64) DDRSS0_HI0_MEM_BASE;
		h->glb_hi1_base_0 = (u64) DDRSS0_HI1_MEM_BASE;
		h->glb_hi2_base_0 = (u64) DDRSS0_HI2_MEM_BASE;

		/* Local Base Addresses [2GB, 2GB, 4GB, 8GB] */
		h->lcl_low_base_0 = (u64) DDRSS0_MEM_BASE;
#if ((defined A53CORE) || (defined C71XCORE))
		h->lcl_hi0_base_0 = (u64) DDRSS0_HI0_MEM_BASE;
		h->lcl_hi1_base_0 = (u64) DDRSS0_HI1_MEM_BASE;
		h->lcl_hi2_base_0 = (u64) DDRSS0_HI2_MEM_BASE;
#else
		h->lcl_hi0_base_0 = (u64) ddrss0_rat_hi0_mem_base;
		h->lcl_hi1_base_0 = (u64) ddrss0_rat_hi1_mem_base;
		h->lcl_hi2_base_0 = (u64) ddrss0_rat_hi2_mem_base;
#endif

		/* Config Addresses */
		h->ss_cfg_base_addr = (u64) (DDRSS0_SS_BASE);
		h->ctl_cfg_base_addr = (u64) (DDRSS0_CTRL_BASE);

		/* Default Access Path */
#if (defined(DEFAULT_RT_PATH))
		h->default_access_data_path = 1;
#else
		h->default_access_data_path = 0;
#endif

		break;
	default:
		break;
	}
}

static s32 fsp_shift(void)
{
	s32 ret = SUCCESS;
	u32 timeout = 0;
	u32 val = 0;

	/* Request freq change */
	val = (readl(WKUP_CTRL_MMR_BASE + CHNG_DDR4_FSP_REQ)) & (~CHNG_DDR4_FSP_REQ_TYPE_MASK);
	val |= CHNG_DDR4_FSP_REQ_TYPE_FSP0;
	writel(val, WKUP_CTRL_MMR_BASE + CHNG_DDR4_FSP_REQ);
	val |= CHNG_DDR4_FSP_REQ_SET;
	writel(val, WKUP_CTRL_MMR_BASE + CHNG_DDR4_FSP_REQ);

	/* Poll for freq change request to be set */
	timeout = TIMEOUT_10_MS;
	while ((timeout > 0U) && (((readl(WKUP_CTRL_MMR_BASE + DDR4_FSP_CLKCHNG_REQ) &
				    DDR4_FSP_CLKCHNG_REQ_SET) == DDR4_FSP_CLKCHNG_REQ_SET) == SFALSE)) {
		--timeout;
	}
	if (timeout == 0U) {
		ret = -EFAIL;
	}

	/* Set the PLL frequency to the requested frequency */
	val = (readl(WKUP_CTRL_MMR_BASE + DDR4_FSP_CLKCHNG_REQ)) & DDR4_FSP_CLKCHNG_REQ_TYPE_MASK;
	if (val == DDR4_FSP_CLKCHNG_REQ_TYPE_FSP0) {
		pll_bypass(&main_pll12, STRUE);
	} else {
		ret = -EFAIL;
	}

	/* Set the FSP ack bit */
	writel(DDR4_FSP_CLKCHNG_REQ_ACK, (WKUP_CTRL_MMR_BASE + DDR4_FSP_CLKCHNG_ACK));

	/* Wait for request to go away */
	timeout = TIMEOUT_10_MS;
	while ((timeout > 0U) && (((readl(WKUP_CTRL_MMR_BASE + DDR4_FSP_CLKCHNG_REQ) &
				    DDR4_FSP_CLKCHNG_REQ_SET) == DDR4_FSP_CLKCHNG_REQ_CLR) == SFALSE)) {
		--timeout;
	}
	if (timeout == 0U) {
		ret = -EFAIL;
	}

	/* Clear the ACK bit */
	val = readl(WKUP_CTRL_MMR_BASE + DDR4_FSP_CLKCHNG_ACK);
	val &= ~DDR4_FSP_CLKCHNG_REQ_ACK;
	writel(val, (WKUP_CTRL_MMR_BASE + DDR4_FSP_CLKCHNG_ACK));

	/* Poll for CHNG_DDR4_FSP_ACK bit to be 1 */
	timeout = TIMEOUT_10_MS;
	while ((timeout > 0U) && (((readl(WKUP_CTRL_MMR_BASE + CHNG_DDR4_FSP_ACK) &
				    (CHNG_DDR4_FSP_CHNG_ACK)) == CHNG_DDR4_FSP_CHNG_ACK) == SFALSE)) {
		--timeout;
	}
	if (timeout == 0U) {
		ret = -EFAIL;
	}

	/* De assert request */
	writel(0, (WKUP_CTRL_MMR_BASE + CHNG_DDR4_FSP_ACK));

	val = readl(WKUP_CTRL_MMR_BASE + CHNG_DDR4_FSP_REQ);
	val &= ~CHNG_DDR4_FSP_REQ_SET;
	writel(val, (WKUP_CTRL_MMR_BASE + CHNG_DDR4_FSP_REQ));

	return ret;
}

#endif

/**
 * \brief Return type of DRAM class from DDR registers
 */
static u32 ddr_read_ddr_type(void)
{
	u32 dram_class, val;

	val = readl(DDR_CTRL_BASE + CDNS_DENALI_CTL_0);
	dram_class = (val & CDNS_DENALI_CTL_0_DRAM_CLASS_MASK) >>
		     CDNS_DENALI_CTL_0_DRAM_CLASS_SHIFT;

	return dram_class;
}

s32 ddr_enter_low_power_mode(void)
{
	u32 val;
	s32 ret = SUCCESS;

	switch (ddr_read_ddr_type()) {
#ifndef CONFIG_LPM_32_BIT_DDR
	case CDNS_DENALI_CTL_0_DRAM_CLASS_LPDDR4:
		/*
		 * Enable auto training for WRLVL, RDLVL, CALVL
		 * Assumption: RDLVL_GATE, CALVL auto trainings enabled by bootloader
		 */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_25);
		val |= CDNS_DENALI_PI_25_WRLVL_AUTO_REQ;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_25);

		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_43);
		val |= CDNS_DENALI_PI_43_RDLVL_AUTO_REQ;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_43);

		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_55);
		val |= CDNS_DENALI_PI_55_CALVL_AUTO_REQ;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_55);

		/* Maintain reset signal throughout deep sleep */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1306);
		val |= CDNS_DENALI_PHY_1306_PHY_SET_DFI_INPUT_0;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1306);

		/* Set CDNS_DENALI_PHY_1369:PHY UPDATE MASK */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);
		val |= CDNS_DENALI_PHY_1369_PHY_UPDATE_MASK;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);

		/* Clear 0x7 in CDNS_DENALI_PHY_1364:PHY_INIT_UPDATE_CONFIG */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);
		val &= ~(CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_MASK <<
			 CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_SHIFT);
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);

		/* Enter Self refresh with ctrl clk gating in deep sleep */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_CTL_158);
		val &= ~CDNS_DENALI_CTL_158_LP_CMD_MASK;
		val |= CDNS_DENALI_CTL_158_LP_CMD_SUSPEND;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_CTL_158);

		break;
	case CDNS_DENALI_CTL_0_DRAM_CLASS_DDR4:

		/* Set CDNS_DENALI_PHY_1369:PHY UPDATE MASK */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);
		val |= CDNS_DENALI_PHY_1369_PHY_UPDATE_MASK;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);

		/* Clear 0x7 in CDNS_DENALI_PHY_1364:PHY_INIT_UPDATE_CONFIG */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);
		val &= ~(CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_MASK <<
			 CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_SHIFT);
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);

		break;
#else
	case CDNS_DENALI_CTL_0_DRAM_CLASS_LPDDR4:
		emif_instance_select(0, &Emifhandle);
		save_registers_optimized(&Emifhandle);
		enter_lpm_self_refresh();
		put_ddrss_in_data_retention_thru_wkup_mmr(DDR16SS_RETENTION_EN);
		break;
#endif
	default:
		ret = -EFAIL;
		break;
	}

	return ret;
}

s32 ddr_enter_io_ddr_mode(void)
{
	s32 ret = SUCCESS;

	switch (ddr_read_ddr_type()) {
#ifndef CONFIG_LPM_32_BIT_DDR
	case CDNS_DENALI_CTL_0_DRAM_CLASS_LPDDR4:
	case CDNS_DENALI_CTL_0_DRAM_CLASS_DDR4:
		break;
#else
	case CDNS_DENALI_CTL_0_DRAM_CLASS_LPDDR4:
		/* Disable self refresh auto entry and exit */
		Write_MMR_Field(DDR_CTRL_BASE + DENALI_CTL_169__SFR_OFFS, 0, 4, 16);
		Write_MMR_Field(DDR_CTRL_BASE + DENALI_CTL_169__SFR_OFFS, 0, 4, 24);

		/* Set valid data for FSP F0 and F2 mr_fsp_data_valid_fN to initiate DFS request */
		Write_MMR_Field(DDR_CTRL_BASE + DENALI_CTL_279__SFR_OFFS, 1, 1, 24);
		Write_MMR_Field(DDR_CTRL_BASE + DENALI_CTL_280__SFR_OFFS, 1, 1, 8);

		/* Shift to boot frequency */
		fsp_shift();

		/* If shift is not successful, then return fail */
		if (((readl(DDR_CTRL_BASE + DENALI_CTL_179__SFR_OFFS) & 0x3000000U) >> 24U) != 0U) {
			ret = -EFAIL;
		} else {
			enter_lpm_self_refresh();
			put_ddrss_in_data_retention_thru_wkup_mmr(DDR16SS_RETENTION_EN);
		}

		break;
#endif
	default:
		ret = -EFAIL;
		break;
	}

	return ret;
}

s32 ddr_exit_low_power_mode(void)
{
	u32 val;
	s32 ret = SUCCESS;

#ifndef CONFIG_LPM_32_BIT_DDR
	switch (ddr_read_ddr_type()) {
	case CDNS_DENALI_CTL_0_DRAM_CLASS_LPDDR4:

		/* Exit self refresh */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_CTL_158);
		val &= ~CDNS_DENALI_CTL_158_LP_CMD_MASK;
		val |= CDNS_DENALI_CTL_158_LP_CMD_RESUME;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_CTL_158);

		/* Clear CDNS_DENALI_PHY_1369:PHY UPDATE MASK */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);
		val &= ~CDNS_DENALI_PHY_1369_PHY_UPDATE_MASK;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);

		/* Set 0x7 in CDNS_DENALI_PHY_1364:PHY_INIT_UPDATE_CONFIG */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);
		val |= (CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_MASK <<
			CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_SHIFT);
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);

		/* Ensure no timeout errors during self refresh entry or exit */
		if (readl(DDR_CTRL_BASE + CDNS_DENALI_CTL_336) != CDNS_DENALI_CTL_336_NO_TIMEOUT_ERROR) {
			ret = -EFAIL;
		}

		/* Remove reset signal and return to pre deep sleep state */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1306);
		val &= ~CDNS_DENALI_PHY_1306_PHY_SET_DFI_INPUT_0;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1306);

		break;
	case CDNS_DENALI_CTL_0_DRAM_CLASS_DDR4:

		/* Clear CDNS_DENALI_PHY_1369:PHY UPDATE MASK */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);
		val &= ~CDNS_DENALI_PHY_1369_PHY_UPDATE_MASK;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1369);

		/* Set 0x7 in CDNS_DENALI_PHY_1364:PHY_INIT_UPDATE_CONFIG */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);
		val |= (CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_MASK <<
			CDNS_DENALI_PHY_1364_PHY_INIT_UPDATE_CONFIG_SHIFT);
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1364);

		break;
	default:
		ret = -EFAIL;
		break;
	}
#else
	u32 wr_val;
	u32 i;
	u32 rd_val;
	u32 ctl_addr = (Emifhandle.ctl_cfg_base_addr);

	configure_sdram_region_idx(&Emifhandle, SDRAM_IDX, REGION_IDX);
	configure_CTL_registers(&Emifhandle);           /* Configure Controller registers */
	configure_PI_registers(&Emifhandle);            /* Configure PI registers */
	configure_PHY_registers(&Emifhandle);           /* Configure PHY registers */
	restore_registers_optimized(&Emifhandle);       /* Restore register values before LPM */

	/* PHY_SET_DFI_INPUT_3:RW_D:24:4:=0x00 PHY_SET_DFI_INPUT_2:RW_D:16:4:=0x00 PHY_SET_DFI_INPUT_1:RW_D:8:4:=0x00 PHY_SET_DFI_INPUT_0:RW_D:0:4:=0x00 */
	rd_val = SOC_read32(ctl_addr + CSL_EMIF_CTLCFG_DENALI_PHY_1820);
	rd_val = (rd_val | 0x40000U);
	SOC_write32((ctl_addr + CSL_EMIF_CTLCFG_DENALI_PHY_1820), rd_val);

	/* PI_TCMD_GAP:RW:16:16:=0x0000 PI_NOTCARE_PHYUPD:RW:8:2:=0x00 PI_INIT_LVL_EN:RW:0:1:=0x00 */
	rd_val = SOC_read32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_4);
	rd_val = (rd_val & 0xFFFFFF00U) | (0x0U);
	SOC_write32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_4, rd_val);

	/* PHY_INDEP_TRAIN_MODE:RW:24:1:=0x01 ODT_VALUE:RW:16:2:=0x01 NO_MRW_INIT:RW:8:1:=0x00 DFI_CMD_RATIO:RD:0:1:=0x00 */
	rd_val = SOC_read32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_20);
	rd_val = (rd_val & 0x00FFFFFFU) | (0x1U << 24);
	SOC_write32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_20, rd_val);

	/* DFIBUS_FREQ_F1:RW:24:5:=0x01 DFIBUS_FREQ_F0:RW:16:5:=0x00 PHY_INDEP_INIT_MODE:RW:8:1:=0x01 TSREF2PHYMSTR:RW:0:6:=0x10 */
	rd_val = SOC_read32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_21);
	rd_val = (rd_val & 0xFFFF00FFU) | (0x1U << 8);
	SOC_write32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_21, rd_val);

	/* PI_DLL_RST_DELAY:RW:16:16:=0x0000 PI_DRAM_INIT_EN:RW:8:1:=0x00 PI_DLL_RST:RW:0:1:=0x00 */
	rd_val = SOC_read32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_150);
	rd_val = (rd_val & 0xFFFF0000U) | (0x101U);
	SOC_write32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_150, rd_val);

	/* SREFRESH_EXIT_NO_REFRESH:RW:24:1:=0x00 PWRUP_SREFRESH_EXIT:RW:16:1:=0x00 TCMDCKE_F2:RW:8:5:=0x03 TCMDCKE_F1:RW:0:5:=0x03 */
	rd_val = SOC_read32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_106);
	rd_val = (rd_val & 0xFF00FFFFU) | (0x0U << 16);
	SOC_write32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_106, rd_val);

	/* PI_SREF_ENTRY_REQ:WR:24:1:=0x00 PI_SREFRESH_EXIT_NO_REFRESH:RW:16:1:=0x00 PI_PWRUP_SREFRESH_EXIT:RW+:8:1:=0x01 PI_MC_PWRUP_SREFRESH_EXIT:RW+:0:1:=0x00 */
	rd_val = SOC_read32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_146);
	rd_val = (rd_val & 0xFFFF00FFU) | (0x1U << 8);
	SOC_write32(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_146, rd_val);

	/* PI_DRAM_INIT_EN=1 */
	Write_MMR_Field(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_150, 0x1, 1, 8);

	/*
	 * Set following equal to the frequency used for low-power retention entry
	 * DFIBUS_BOOT_FREQ, INIT_FREQ, PI_FREQ_RETENTION_NUM, PI_INIT_WORK_FREQ
	 */
	Write_MMR_Field(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_180, 0x2, 2, 8);      /* DENALI_CTL_180 DFIBUS_BOOT_FREQ bits 9:8 */
	Write_MMR_Field(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_CTL_180, 0x2, 2, 0);      /* DENALI_CTL_180 INIT_FREQ bits 1:0 */
	Write_MMR_Field(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_165, 0x2, 5, 16);      /* DENALI_PI_165 PI_FREQ_RETENTION_NUM bits 20:16 */
	Write_MMR_Field(Emifhandle.ctl_cfg_base_addr + CSL_EMIF_CTLCFG_DENALI_PI_11, 0x2, 5, 0);        /* DENALI_PI_11 PI_INIT_WORK_FREQ bits 4:0 */

	/* De-asserting data retention pin and wake Control bits */
	put_ddrss_in_data_retention_thru_wkup_mmr(DDR16SS_RETENTION_DIS);

	/* Wait for reg values to set */
	for (i = 0; i < 1000U; i++) {
		delay_1us();
	}

	/* Start Initialization [PI_START=1 and START=1] */
	start_PI_CTL_init(&Emifhandle);

	poll_for_init_completion(&Emifhandle); /* Poll for init completion */
#endif

	return ret;
}

s32 ddr_deepsleep_exit_training(void)
{
	s32 ret = SUCCESS;

	switch (ddr_read_ddr_type()) {
	case CDNS_DENALI_CTL_0_DRAM_CLASS_LPDDR4:
		ret = SUCCESS;

		break;
	case CDNS_DENALI_CTL_0_DRAM_CLASS_DDR4:
	{
		u32 timeout = DDR_RETRAIN_TIMEOUT;
		u32 val;
		u32 ddr_rank;
		u8 curr_cs = 0;

		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_13);
		ddr_rank = (val & CDNS_DENALI_PI_13_CS_MAP_MASK) >>
			   CDNS_DENALI_PI_13_CS_MAP_SHIFT;

		/* Write phy_cal_start_0 = 1 */
		val = readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1333);
		val |= CDNS_DENALI_PHY_1333_PHY_CAL_START_0;
		writel(val, DDR_CTRL_BASE + CDNS_DENALI_PHY_1333);

		/* Poll on phy_cal_result_obs_0 bit 23 = 1 */
		while (((timeout > 0U) && ((readl(DDR_CTRL_BASE + CDNS_DENALI_PHY_1336) &
					    (CDNS_DENALI_PHY_1336_PHY_CAL_RESULT_OBS_0)) == CDNS_DENALI_PHY_1336_PHY_CAL_RESULT_OBS_0)) == SFALSE) {
			--timeout;
		}
		if (timeout == 0U) {
			ret = -EFAIL;
		}

		/* Program PI_INT_ACK={`ddr32_ew_PI_NUM_INT_SOURCES{1'b1}} */
		writel(CDNS_DENALI_PI_84_INT_ACK_REG_MASK, DDR_CTRL_BASE + CDNS_DENALI_PI_84);

		/* Software trigger read gate level training */
		if (ret == 0) {
			for (u32 i = ddr_rank; i > 0U; i >>= 1) {
				/* Program PI_RDLVL_CS = 0 for first rank and 1 for second rank */
				val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_34);
				if (curr_cs == 0U) {
					val &= ~CDNS_DENALI_PI_34_RDLVL_CS;
				} else {
					val |= CDNS_DENALI_PI_34_RDLVL_CS;
				}

				writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_34);

				/* Program PI_RDLVL_GATE_REQ=1 */
				val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_33);
				val |= CDNS_DENALI_PI_33_RDLVL_GATE_REQ;
				writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_33);

				/* Polling PI_INT_STATUS[`ddr32_ew_PI_LVL_DONE_BIT]=1 */
				timeout = DDR_RETRAIN_TIMEOUT;
				while (((timeout > 0U) && ((readl(DDR_CTRL_BASE + CDNS_DENALI_PI_83) &
							    (CDNS_DENALI_PI_83_LVL_DONE_BIT)) == CDNS_DENALI_PI_83_LVL_DONE_BIT)) == SFALSE) {
					--timeout;
				}
				if (timeout == 0U) {
					ret = -EFAIL;
				}

				/* Program PI_INT_ACK={`ddr32_ew_PI_NUM_INT_SOURCES{1'b1}} */
				writel(CDNS_DENALI_PI_84_INT_ACK_REG_MASK, DDR_CTRL_BASE + CDNS_DENALI_PI_84);
				curr_cs++;
			}
		}

		/* Software trigger read level training */
		if (ret == 0) {
			curr_cs = 0;

			for (u32 i = ddr_rank; i > 0U; i >>= 1) {
				/* Program PI_RDLVL_CS = 0 for first rank and 1 for second rank */
				val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_34);
				if (curr_cs == 0U) {
					val &= ~CDNS_DENALI_PI_34_RDLVL_CS;
				} else {
					val |= CDNS_DENALI_PI_34_RDLVL_CS;
				}

				writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_34);

				/* Program PI_RDLVL_REQ=1 */
				val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_33);
				val |= CDNS_DENALI_PI_33_RDLVL_REQ;
				writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_33);

				/* Polling PI_INT_STATUS[`ddr32_ew_PI_LVL_DONE_BIT]=1 */
				timeout = DDR_RETRAIN_TIMEOUT;
				while (((timeout > 0U) && ((readl(DDR_CTRL_BASE + CDNS_DENALI_PI_83) &
							    (CDNS_DENALI_PI_83_LVL_DONE_BIT)) == CDNS_DENALI_PI_83_LVL_DONE_BIT)) == SFALSE) {
					--timeout;
				}
				if (timeout == 0U) {
					ret = -EFAIL;
				}

				/* Program PI_INT_ACK={`ddr32_ew_PI_NUM_INT_SOURCES{1'b1}} */
				writel(CDNS_DENALI_PI_84_INT_ACK_REG_MASK, DDR_CTRL_BASE + CDNS_DENALI_PI_84);
				curr_cs++;
			}
		}

		/* Software trigger write level training */
		if (ret == 0) {
			curr_cs = 0;

			for (u32 i = ddr_rank; i > 0U; i >>= 1) {
				/* Program PI_RDLVL_CS = 0 for first rank and 1 for second rank */
				val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_24);
				if (curr_cs == 0U) {
					val &= ~CDNS_DENALI_PI_24_WRLVL_CS;
				} else {
					val |= CDNS_DENALI_PI_24_WRLVL_CS;
				}

				writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_24);

				/* Program PI_WRLVL_REQ=1 */
				val = readl(DDR_CTRL_BASE + CDNS_DENALI_PI_23);
				val |= CDNS_DENALI_PI_23_WRLVL_REQ;
				writel(val, DDR_CTRL_BASE + CDNS_DENALI_PI_23);

				/* Polling PI_INT_STATUS[`ddr32_ew_PI_LVL_DONE_BIT]=1 */
				timeout = DDR_RETRAIN_TIMEOUT;
				while (((timeout > 0U) && ((readl(DDR_CTRL_BASE + CDNS_DENALI_PI_83) &
							    (CDNS_DENALI_PI_83_LVL_DONE_BIT)) == CDNS_DENALI_PI_83_LVL_DONE_BIT)) == SFALSE) {
					--timeout;
				}
				if (timeout == 0U) {
					ret = -EFAIL;
				}

				/* Program PI_INT_ACK={`ddr32_ew_PI_NUM_INT_SOURCES{1'b1}} */
				writel(CDNS_DENALI_PI_84_INT_ACK_REG_MASK, DDR_CTRL_BASE + CDNS_DENALI_PI_84);
			}
		}

		break;
	}
	default:
		ret = -EFAIL;
		break;
	}

	return ret;
}

void ddr_enable_retention(void)
{
	writel(DDR16SS_RETENTION_EN, WKUP_CTRL_MMR_BASE + DDR16SS_PMCTRL);
}

void ddr_disable_retention(void)
{
	writel(DDR16SS_RETENTION_DIS, WKUP_CTRL_MMR_BASE + DDR16SS_PMCTRL);
}
