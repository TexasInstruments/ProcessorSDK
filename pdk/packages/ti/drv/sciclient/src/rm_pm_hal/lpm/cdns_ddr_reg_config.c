/*
 * DM Stub Firmware
 *
 * CDNS DDR driver
 *
 * Copyright (C) 2023-2024, Texas Instruments Incorporated
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

#ifndef _CDNS_DDR_REG_CONFIG_C_
#define _CDNS_DDR_REG_CONFIG_C_

#include <stdio.h>
#include <stdint.h>
#include "lpm_io.h"
#include "DDRSS_addr_map_sfr_offs_ew_32bit.h"
#include "cdns_ddr_reg_config.h"

volatile uint32_t saved_reg_data[2000];
volatile uint32_t changed_reg_count;

void configure_CTL_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_REG_BASE = h->ctl_cfg_base_addr;
	uint32_t wdata_340, wdata_322, wdata_323;
	uint32_t cs_msk, cs_val_lower, cs_val_upper, i;

	for (i = 0; i <= 434U; i++) {
		SOC_write32(DDR_CTL_REG_BASE + (i * 4U), denali_ctl_data[i]);
	}

	/* Disable bank group rotation for DDR4 */
#if defined(DDR4_MEM)
	wdata_340 = DENALI_CTL_340_DATA;
	wdata_340 = wdata_340 & 0xFEFFFFFF;
	SOC_write32(DDR_CTL_REG_BASE + DENALI_CTL_340__SFR_OFFS, wdata_340);
#endif

	/*
	 * DENALI_CTL_322_DATA - BANK_DIFF_1:RW:24:2:=0x01 BANK_DIFF_0:RW:16:2:=0x01 ZQ_CAL_LATCH_MAP_1:RW_D:8:2:=0x00 ZQ_CAL_START_MAP_1:RW_D:0:2:=0x00
	 * DENALI_CTL_323_DATA - COL_DIFF_1:RW:24:4:=0x00 COL_DIFF_0:RW:16:4:=0x00 ROW_DIFF_1:RW:8:3:=0x01 ROW_DIFF_0:RW:0:3:=0x01
	 */
#if defined(DDR4_MEM)
	/* 10 col bits (diff=0), 17 row bits (diff=1), 4 bank bits(diff=0) */
	wdata_322 = DENALI_CTL_322_DATA;
	wdata_322 = wdata_322 & 0xFFFF; /* Reset 31:16 bits to 0 [BANK_DIFF_1:RW:24:2:=0x01 BANK_DIFF_0:RW:16:2:=0x01] */
	wdata_323 = 0x00000101;         /* Row difference set to 1 [17 bits for row] */
#else
	/* 10 col bits (diff=0), 17 row bits (diff=1), 3 bank bits(diff=1) */
	wdata_322 = 0x01010000;                                 /* DENALI_CTL_322_DATA; */
	wdata_322 = ((wdata_322 & 0xFFFFU) | 0x01010000U);      /* Reset 31:16 bits to 0x0101 [BANK_DIFF_1:RW:24:2:=0x01 BANK_DIFF_0:RW:16:2:=0x01] */
	wdata_323 = 0x00000101;                                 /* Row difference set to 1 [17 bits for row] */
#endif
	SOC_write32(DDR_CTL_REG_BASE + (uint32_t) DENALI_CTL_322__SFR_OFFS, wdata_322);
	SOC_write32(DDR_CTL_REG_BASE + (uint32_t) DENALI_CTL_323__SFR_OFFS, wdata_323);

	/*
	 * Chip Select bits
	 * Last bit as chip select for DDR4. For LPDDR4, default would work
	 */
#if defined(DDR4_MEM)
	cs_val_lower = 0x3fff0000;      /* cs = 0 range */
	cs_val_upper = 0x7fff4000;      /* cs = 1 range */
	SOC_write32(DDR_CTL_REG_BASE + DENALI_CTL_324__SFR_OFFS, cs_val_lower);
	SOC_write32(DDR_CTL_REG_BASE + DENALI_CTL_326__SFR_OFFS, cs_val_upper);
#endif
}

void configure_PI_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_PI_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_PI_REGISTER_BLOCK__OFFS;
	uint32_t wdata_145, i;

	for (i = 0; i <= 423U; i++) {
		SOC_write32(DDR_CTL_PI_REG_BASE + (i * 4U), denali_pi_data[i]);
	}

	/* Disable bank group rotation for DDR4 */
#if defined(DDR4_MEM)
	wdata_145 = DENALI_PI_145_DATA;
	wdata_145 = wdata_145 & 0xFFFFFEFF;
	SOC_write32(DDR_CTL_PI_REG_BASE + DENALI_PI_145__SFR_OFFS, wdata_145);
#endif
}

/*
 * -----------------------------------------------------------------------
 * PHY Address Space
 * -----------------------------------------------------------------------
 * Data Slice 0: PHY_BASE_ADDR + 0 DENALI_PHY_0
 * Data Slice 1: PHY_BASE_ADDR + 256 DENALI_PHY_256
 * Data Slice 2: PHY_BASE_ADDR + 512 DENALI_PHY_512
 * Data Slice 3: PHY_BASE_ADDR + 768 DENALI_PHY_768
 * Address Slice 0: PHY_BASE_ADDR + 1024 DENALI_PHY_1024
 * Address Slice 1: PHY_BASE_ADDR + 1280 DENALI_PHY_1280
 * Address Slice 2: PHY_BASE_ADDR + 1536 DENALI_PHY_1536
 * PHY Core: PHY_AC_BASE_ADDR DENALI_PHY_1792
 * -----------------------------------------------------------------------
 */
void configure_PHY_registers(struct emif_handle_s *h)
{
	configure_data_slice0_registers(h);
	configure_data_slice1_registers(h);
	configure_data_slice2_registers(h);
	configure_data_slice3_registers(h);
	configure_address_slice0_registers(h);
	configure_address_slice1_registers(h);
	configure_address_slice2_registers(h);
	configure_ddrphy_registers(h);
}

void configure_data_slice0_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_DATA_SLICE_0_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_0_REGISTER_BLOCK__OFFS;
	uint32_t i;

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_0_REG_BASE + (i * 4U), denali_data_slice0[i]);
		}
	}
}

void configure_data_slice1_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_DATA_SLICE_1_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_1_REGISTER_BLOCK__OFFS;
	uint32_t i;

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_1_REG_BASE + (i * 4U), denali_data_slice1[i]);
		}
	}
}

void configure_data_slice2_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_DATA_SLICE_2_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_2_REGISTER_BLOCK__OFFS;
	uint32_t i;

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_2_REG_BASE + (i * 4U), denali_data_slice2[i]);
		}
	}
}

void configure_data_slice3_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_DATA_SLICE_3_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_3_REGISTER_BLOCK__OFFS;
	uint32_t i;

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_3_REG_BASE + (i * 4U), denali_data_slice3[i]);
		}
	}
}

void configure_address_slice0_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_ADDR_SLICE_0_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_0_REGISTER_BLOCK__OFFS;
	uint32_t i;

	for (i = 0; i <= 48U; i++) {
		SOC_write32(DDR_CTL_ADDR_SLICE_0_REG_BASE + (i * 4U), denali_addr_slice0[i]);
	}
}

void configure_address_slice1_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_ADDR_SLICE_1_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_1_REGISTER_BLOCK__OFFS;
	uint32_t i;

	for (i = 0; i <= 48U; i++) {
		SOC_write32(DDR_CTL_ADDR_SLICE_1_REG_BASE + (i * 4U), denali_addr_slice1[i]);
	}
}

void configure_address_slice2_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_ADDR_SLICE_2_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_2_REGISTER_BLOCK__OFFS;
	uint32_t i;

	for (i = 0; i <= 48U; i++) {
		SOC_write32(DDR_CTL_ADDR_SLICE_2_REG_BASE + (i * 4U), denali_addr_slice2[i]);
	}
}

void configure_ddrphy_registers(struct emif_handle_s *h)
{
	uint32_t DDR_CTL_PHY_CORE_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_PHY_Core_REGISTER_BLOCK__OFFS;
	uint32_t wdata_1826, i;

	/* Program the PHY */
	for (i = 0; i < 132U; i++) {
		SOC_write32(DDR_CTL_PHY_CORE_REG_BASE + (i * 4U), denali_phy_data[i]);
	}

	/* PHY_SW_GRP0_SHIFT_0:RW+:24:5:=0x00 PHY_FREQ_SEL_INDEX:RW+:16:2:=0x00 PHY_FREQ_SEL_MULTICAST_EN:RW+:8:1:=0x01 PHY_FREQ_SEL_FROM_REGIF:RW_D:0:1:=0x00 */
	SOC_write32(DDR_CTL_PHY_CORE_REG_BASE + (uint32_t) DENALI_PHY_1793__SFR_OFFS, 0x00010000);

	/* Set pll_postdiv to 0 for LPDDR4 memory */
#if (defined(LPDDR4_MEM) || defined(POST_PLLDIV_0))
#if (!(defined(SPEED_250_MTPS)))                /* pll_postdiv should be non-zero for 250MTPS */
	wdata_1826 = 0x00041b42U & 0xFFFFF1FFU; /* Set 11:9 bits to 0 - pll_postdiv in PHY_LP4_BOOT_PLL_CTRL field //#define             DENALI_PHY_1826_DATA 0x00041b42 // */
	SOC_write32(DDR_CTL_PHY_CORE_REG_BASE + (uint32_t) DENALI_PHY_1826__SFR_OFFS, wdata_1826);
#endif
#endif
}

void save_registers_optimized(struct emif_handle_s *h)
{
	uint32_t i, count = 0;
	uint32_t rd_val = 0;
	uint32_t DDR_CTL_REG_BASE = h->ctl_cfg_base_addr;
	uint32_t DDR_CTL_PI_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_PI_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_0_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_0_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_1_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_1_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_2_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_2_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_3_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_3_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_ADDR_SLICE_0_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_0_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_ADDR_SLICE_1_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_1_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_ADDR_SLICE_2_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_2_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_PHY_CORE_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_PHY_Core_REGISTER_BLOCK__OFFS;

	count++;
	saved_reg_data[count] = SOC_read32(DDR_CTL_PHY_CORE_REG_BASE + 0x4U);                   /* DDRSS_PHY_1793__SFR_OFFS */
	count++;
	saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_0_REG_BASE + 0x14U);              /* DDRSS_PHY_5__SFR_OFFS */
	count++;
	saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_1_REG_BASE + 0x14U);              /* DDRSS_PHY_261__SFR_OFFS */
	count++;
	saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_2_REG_BASE + 0x14U);              /* DDRSS_PHY_517__SFR_OFFS */
	count++;
	saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_3_REG_BASE + 0x14U);              /* DDRSS_PHY_773__SFR_OFFS */
	count++;

	for (i = 1; i <= 434U; i++) {
		saved_reg_data[count] = SOC_read32(DDR_CTL_REG_BASE + (i * 4U));
		count++;
	}

	for (i = 1; i <= 423U; i++) {
		saved_reg_data[count] = SOC_read32(DDR_CTL_PI_REG_BASE + (i * 4U));
		count++;
	}

	for (i = 0; i <= 136U; i++) {
		{
			saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_0_REG_BASE + (i * 4U));
			count++;
		}
	}

	for (i = 0; i <= 136U; i++) {
		{
			saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_1_REG_BASE + (i * 4U));
			count++;
		}
	}

	for (i = 0; i <= 136U; i++) {
		{
			saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_2_REG_BASE + (i * 4U));
			count++;
		}
	}

	for (i = 0; i <= 136U; i++) {
		{
			saved_reg_data[count] = SOC_read32(DDR_CTL_DATA_SLICE_3_REG_BASE + (i * 4U));
			count++;
		}
	}

	for (i = 0; i <= 48U; i++) {
		saved_reg_data[count] = SOC_read32(DDR_CTL_ADDR_SLICE_0_REG_BASE + (i * 4U));
		count++;
	}

	for (i = 0; i <= 48U; i++) {
		saved_reg_data[count] = SOC_read32(DDR_CTL_ADDR_SLICE_1_REG_BASE + (i * 4U));
		count++;
	}

	for (i = 0; i <= 48U; i++) {
		saved_reg_data[count] = SOC_read32(DDR_CTL_ADDR_SLICE_2_REG_BASE + (i * 4U));
		count++;
	}

	for (i = 0; i <= 132U; i++) {
		{
			saved_reg_data[count] = SOC_read32(DDR_CTL_PHY_CORE_REG_BASE + (i * 4U));
			count++;
		}
	}

	saved_reg_data[0] = count;
}

void restore_registers_optimized(struct emif_handle_s *h)
{
	uint32_t i, count = 0;
	uint32_t DDR_CTL_REG_BASE = h->ctl_cfg_base_addr;
	uint32_t DDR_CTL_PI_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_PI_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_0_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_0_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_1_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_1_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_2_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_2_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_DATA_SLICE_3_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Data_Slice_3_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_ADDR_SLICE_0_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_0_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_ADDR_SLICE_1_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_1_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_ADDR_SLICE_2_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_Address_Slice_2_REGISTER_BLOCK__OFFS;
	uint32_t DDR_CTL_PHY_CORE_REG_BASE = (h->ctl_cfg_base_addr) + (uint32_t) DDRSS_PHY_Core_REGISTER_BLOCK__OFFS;

	count++;
	SOC_write32(DDR_CTL_PHY_CORE_REG_BASE + 0x4U, saved_reg_data[count]);                   /* DDRSS_PHY_1793__SFR_OFFS */
	count++;
	SOC_write32(DDR_CTL_DATA_SLICE_0_REG_BASE + 0x14U, saved_reg_data[count]);              /* DDRSS_PHY_5__SFR_OFFS */
	count++;
	SOC_write32(DDR_CTL_DATA_SLICE_1_REG_BASE + 0x14U, saved_reg_data[count]);              /* DDRSS_PHY_261__SFR_OFFS */
	count++;
	SOC_write32(DDR_CTL_DATA_SLICE_2_REG_BASE + 0x14U, saved_reg_data[count]);              /* DDRSS_PHY_517__SFR_OFFS */
	count++;
	SOC_write32(DDR_CTL_DATA_SLICE_3_REG_BASE + 0x14U, saved_reg_data[count]);              /* DDRSS_PHY_773__SFR_OFFS */
	count++;

	for (i = 1; i <= 434U; i++) {
		SOC_write32(DDR_CTL_REG_BASE + (i * 4U), saved_reg_data[count]);
		count++;
	}

	for (i = 1; i <= 423U; i++) {
		SOC_write32(DDR_CTL_PI_REG_BASE + (i * 4U), saved_reg_data[count]);
		count++;
	}

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_0_REG_BASE + (i * 4U), saved_reg_data[count]);
			count++;
		}
	}

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_1_REG_BASE + (i * 4U), saved_reg_data[count]);
			count++;
		}
	}

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_2_REG_BASE + (i * 4U), saved_reg_data[count]);
			count++;
		}
	}

	for (i = 0; i <= 136U; i++) {
		{
			SOC_write32(DDR_CTL_DATA_SLICE_3_REG_BASE + (i * 4U), saved_reg_data[count]);
			count++;
		}
	}

	for (i = 0; i <= 48U; i++) {
		SOC_write32(DDR_CTL_ADDR_SLICE_0_REG_BASE + (i * 4U), saved_reg_data[count]);
		count++;
	}

	for (i = 0; i <= 48U; i++) {
		SOC_write32(DDR_CTL_ADDR_SLICE_1_REG_BASE + (i * 4U), saved_reg_data[count]);
		count++;
	}

	for (i = 0; i <= 48U; i++) {
		SOC_write32(DDR_CTL_ADDR_SLICE_2_REG_BASE + (i * 4U), saved_reg_data[count]);
		count++;
	}

	for (i = 0; i <= 132U; i++) {
		{
			SOC_write32(DDR_CTL_PHY_CORE_REG_BASE + (i * 4U), saved_reg_data[count]);
			count++;
		}
	}
}

#endif
