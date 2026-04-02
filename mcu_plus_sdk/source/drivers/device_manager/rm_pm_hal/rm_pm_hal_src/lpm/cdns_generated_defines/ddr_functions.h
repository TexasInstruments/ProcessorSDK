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

#ifndef _DDR_FUNCTIONS_H_
#define _DDR_FUNCTIONS_H_

/*
 * -----------------------------------------------------------------------------
 * Set these based on DDR memory [16GB] - Value of 0x11 sets 16GB
 * -----------------------------------------------------------------------------
 */
#define SDRAM_IDX  0x12
#define REGION_IDX 0x12

/*
 * -----------------------------------------------------------------------------
 * DDR RAT defines for processors whose output is 32 bit
 * -----------------------------------------------------------------------------
 */

#ifdef M4CORE
/*
 * Config already programs region 1 for access of 0xf000_0000 to higher ddr
 * Overwrite that region based on what is needed
 * M4 memory map dictates SRAM regions, if execute is needed it should be within that
 * 0x2000_0000-0x3FFF_FFFF is an SRAM region
 * Config Remaps 0xA000_0000 - 0xBFFF_FFFF for 0x2000_0000-0x3FFF_FFFF - so essentially that region is free
 */
#define DDR_RAT_REGION_TO_USE 1U
#define DDR_RAT_ADDR_TO_USE m4_rat_cfg_base(DDNUM)
#define ddrss0_rat_hi0_mem_base 0x8f000000U
#define ddrss0_rat_hi1_mem_base 0x8f400000U
#define ddrss0_rat_hi2_mem_base 0x8f800000U
#define DDR_RAT_REGION_SIZE 20U
#elif defined(R5FCORE)
/* Config uses region 1 for DDR HI */
#define DDR_RAT_REGION_TO_USE 1U
#define DDR_RAT_ADDR_TO_USE rat_cfg_base
#define ddrss0_rat_hi0_mem_base 0xF0000000U
#define ddrss0_rat_hi1_mem_base 0xF4000000U
#define ddrss0_rat_hi2_mem_base 0xF8000000U
#define DDR_RAT_REGION_SIZE 24U
#else
#define DDR_RAT_REGION_TO_USE 0U
#define ddrss0_rat_hi0_mem_base 0xF0000000U
#define ddrss0_rat_hi1_mem_base 0xF4000000U
#define ddrss0_rat_hi2_mem_base 0xF8000000U
#define DDR_RAT_REGION_SIZE 24U
#endif

/*
 * -----------------------------------------------------------------------------
 * Define DDR4 or LPDDR4
 * -----------------------------------------------------------------------------
 */
#ifndef DDR4_MEM
#define LPDDR4_MEM
#endif

/*
 * -----------------------------------------------------------------------------
 * Memory Type for Init and Self Refresh LP Mode
 * -----------------------------------------------------------------------------
 */
#define DDR4_DRAM_CLASS_REG_VALUE    0xAU
#define LPDDR4_DRAM_CLASS_REG_VALUE  0xBU
#define LP_MODE_SELF_REFRESH         0x29U
#define LP_MODE_LONG_SELF_REFRESH    0x31U /* 0x51 AU */
#define LP_MODE_PRECHARGE_POWER_DOWN 0x3U
#define LP_MODE_NONE                 0x2U
#define STATUS_SR_ENTERED            0x4B00U
#define STATUS_SR_SHORT_ENTERED      0x4500U
#define STATUS_SR_LONG_ENTERED       0x4E00U
#define STATUS_SR_ENTERED_MASK       0xFF00U


/*
 * -----------------------------------------------------------------------------
 * This is to enable reuse of AM62 functions [AM62A has updated and cleaned up funtions]
 * -----------------------------------------------------------------------------
 */
#if defined(DDR4_MEM)
#define ddr4_setup ddr_setup
#else
#define lpddr4_setup ddr_setup
#endif

/*
 * -----------------------------------------------------------------------------
 * DDR EMIF Handle
 * -----------------------------------------------------------------------------
 */
typedef struct emif_handle_s {
	/* Instance number */
	uint32_t	dinum;

	/* Memory Type */
	uint32_t	is_ddr4_mem;

	/* PSC Variables */
	uint32_t	local_pdctl;
	uint32_t	local_mdctl;
	uint32_t	cfg_pdctl;
	uint32_t	cfg_mdctl;
	uint32_t	data_pdctl;
	uint32_t	data_mdctl;

	/* Global and Local Base addresses for 8GB region (0->2GB, 2GB->4GB, 4GB->8GB, 8GB->12GB, 12GB->16GB) */
	uint64_t	glb_low_base_0, lcl_low_base_0;
	uint64_t	glb_hi0_base_0, lcl_hi0_base_0;
	uint64_t	glb_hi1_base_0, lcl_hi1_base_0;
	uint64_t	glb_hi2_base_0, lcl_hi2_base_0;
	uint64_t	glb_hi3_base_0, lcl_hi3_base_0;

	/* Configuration address base */
	uint64_t	ss_cfg_base_addr;
	uint64_t	ctl_cfg_base_addr;

	/* Default Access Path */
	uint32_t	default_access_data_path;
} emif_handle_t;

#endif
