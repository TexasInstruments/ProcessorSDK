/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2015-2023, Texas Instruments Incorporated
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

#ifndef CLK_PLL_H
#define CLK_PLL_H

#include <types/sbool.h>
#include <types/devgrps.h>
#include <limits.h>
#include <clk.h>
#include <clock_limits.h>

/*
 * Indexes into const PLL table entries are currently u8s
 * terminated by UCHAR_MAX. This allows for up to 255 entries.
 */
#define PLL_TABLE_LAST  ((u8) UCHAR_MAX)

/**
 * \brief PLL table entry
 *
 * This describes a precomputed PLL setting. This should be preferred
 * over brute force calculated settings. Additionally, PLL table entries
 * can be outside spec'd PLL parameters such as minimum VCO frequency.
 *
 * Note that the input frequency is not specified but the input frequency
 * range can be calculated from the given parameters.
 *
 * Current data type sizes are chosen based on currently supported PLLs.
 * If support for an additional PLL with a larger range is added, then
 * the data sizes will either need to increase or be selected at compile
 * time.
 */
struct pll_table_entry {
	/** Maximum output frequency */
	u32	freq_min_hz;

	/** Minimum output frequency */
	u32	freq_max_hz;

	/** Multiplier setting */
	u16	pllm;

	/** Input divider setting */
	u8	plld;

	/** Output divider setting */
	u8	clkod;

	/** Fractional multiplier part */
	u32	pllfm;
};

/** The SoC specific table of precomputed PLL settings */
extern const struct pll_table_entry soc_pll_table[SOC_PLL_TABLE_RANGE_ID_MAX];

/** PLL specific const clock data. */
struct clk_data_pll {
	/** Common const clock data */
	struct clk_drv_data data;


	/**
	 * \brief Precomputed PLL entries
	 *
	 * Points to an array of indexes into the PLL table. Table
	 * is terminated by PLL_TABLE_LAST. NULL if there are no
	 * entries.
	 */
	const u8 *pll_entries;

	/**
	 * \brief VCO range.
	 *
	 * This points to the valid VCO frequency range in the range table.
	 */
	u8 vco_range_idx;

	/**
	 * \brief VCO input range.
	 *
	 * This points to the valid VCO input frequency range in the range
	 * table.
	 */
	u8 vco_in_range_idx;

	/**
	 * \brief Default frequency.
	 *
	 * This points to an index in the default frequency table. This
	 * allows the PLL to be programmed to a specific frequency at boot
	 * time. A table entry of 0 indicates that the current state of
	 * the hardware should be used.
	 */
	u8 default_freq_idx;


	/**
	 * \brief Device Group.
	 *
	 * Device group(s) this PLL is contained in or DEVGRP_ALL to
	 * indicate unspecified.
	 */
	devgrp_t devgrp;

	/**
	 *
	 * \brief Enable fractional support.
	 *
	 * True to enable fractional support if supported by this PLL. If
	 * this is false, fractional frequencies can still be read, but
	 * new fractional frequencies cannot be programmed.
	 */
	sbool fractional_support;
};

/** Const PLL data for use by the pll_calc function. */
struct pll_data {
	/** Maximum PLL divider. */
	u32	plld_max;

	/** Maximum PLL multiplier. */
	u32	pllm_max;

	/** Bits of fractional PLL multiplier. */
	u32	pllfm_bits;

	/** Maximum PLL post-divider. */
	u32	clkod_max;

	/**
	 * \brief Indicate if a divider is valid.
	 *
	 * Some PLLs cannot use all values in their range, such as odd or
	 * even values. This callback allows the pll_calc function to skip
	 * over invalid values.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param plld
	 * The PLL divider value to test.
	 *
	 * \return
	 * True if divider can be used, SFALSE otherwise.
	 */
	sbool (*plld_valid)(struct clk *clkp, u32 plld);

	/**
	 * \brief Indicate if a multiplier is valid.
	 *
	 * Some PLLs cannot use all values in their range, such as odd or
	 * even values. This callback allows the pll_calc function to skip
	 * over invalid values.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param pllm
	 * The PLL multiplier value to test.
	 *
	 * \param is_frac
	 * A fractional multiplier is used.
	 * Note: If a given pllm value is valid for is_frac == true, it
	 * *must* also be valid for pllm == false. This is to ease complexity
	 * in the pll_calc code paths.
	 *
	 * \return
	 * True if multiplier can be used, SFALSE otherwise.
	 */
	sbool (*pllm_valid)(struct clk *clkp, u32 pllm, sbool is_frac);

	/**
	 * \brief Stride value for a given pllm.
	 *
	 * Some PLLs have additional multipliers enabled to reach certain
	 * pllm values. Normally, these are handled by treating the values
	 * inbetween possible values as invalid. However, when dealing with
	 * fractional multipliers the pll_calc function must understand that
	 * the fractional multiplier will also be multiplied by this value.
	 *
	 * Provide the extra multiplier value that gets applied at a given
	 * pllm value.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param pllm
	 * The PLL multiplier value to use.
	 *
	 * \return
	 * The stride size for the given pllm, or 1 if there is no stride.
	 */
	u32 (*pllm_stride)(struct clk *clkp, u32 pllm);

	/**
	 * \brief Indicate if a post-divider is valid.
	 *
	 * Some PLLs cannot use all values in their range, such as odd or
	 * even values. This callback allows the pll_calc function to skip
	 * over invalid values.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param clkod
	 * The PLL post-divider value to test.
	 *
	 * \return
	 * True if post-divider can be used, SFALSE otherwise.
	 */
	sbool (*clkod_valid)(struct clk *clkp, u32 clkod);

	/**
	 * \brief Indicate a bin for this setting combination.
	 *
	 * Some PLLs prefer certain combinations or ranges of settings over
	 * others, such as a multiplier below 512. Binning allows pll_calc
	 * to act on that preference. This function returns a bin number for
	 * each pllm/plld/clkod combination passed. pll_calc will always
	 * prefer a setting with a higher bin value over a lower bin value.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param plld
	 * The PLL divider value to test.
	 *
	 * \param is_frac
	 * A fractional multiplier is used.
	 *
	 * \param clkod
	 * The PLL post-divider value to test.
	 *
	 * \return
	 * The bin number. 0 for least preference, INT_MAX for greatest
	 * preference. Negative for disallowed combinations.
	 */
	s32 (*bin)(struct clk *clkp, u32 plld, u32 pllm, sbool is_frac, u32 clkod);

	/**
	 * \brief Try to find a larger pllm value that exists in a better bin
	 *
	 * The pll calculation code functions by testing every allowable plld
	 * and clkod combination. For each combination, it tests a pllm value
	 * that produces a frequency at or below the target, and a pllm value
	 * that produces a frequency above the target. However, this may skip
	 * certain plld/pllm/clkod combinations that are in a better bin.
	 * This function allows the pll driver code to return a larger pllm
	 * value that is in a better bin than the current pllm value. It
	 * will be called by the pll calculation code until it returns 0.
	 *
	 * Note that the pll calculation function will assume the pllm value
	 * returned is valid and will not call pllm_valid to test it.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param plld
	 * The PLL divider value to test.
	 *
	 * \param pllm
	 * The PLL multiplier value to test.
	 *
	 * \param clkod
	 * The PLL post-divider value to test.
	 *
	 * \return
	 * A pllm setting above the passed pllm value that gives a
	 * higher bin number for the new plld/pllm/clkod combination. 0 if
	 * no such value exists.
	 */
	u32 (*bin_next_pllm)(struct clk *clkp, u32 plld, u32 pllm, u32 clkod);

	/**
	 * \brief Try to find a smaller pllm value that exists in a better bin
	 *
	 * The pll calculation code functions by testing every allowable plld
	 * and clkod combination. For each combination, it tests a pllm value
	 * that produces a frequency at or below the target, and a pllm value
	 * that produces a frequency above the target. However, this may skip
	 * certain plld/pllm/clkod combinations that are in a better bin.
	 * This function allows the pll driver code to return a smaller pllm
	 * value that is in a better bin than the current pllm value. It
	 * will be called by the pll calculation code until it returns 0.
	 *
	 * Note that the pll calculation function will assume the pllm value
	 * returned is valid and will not call pllm_valid to test it.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param plld
	 * The PLL divider value to test.
	 *
	 * \param pllm
	 * The PLL multiplier value to test.
	 *
	 * \param clkod
	 * The PLL post-divider value to test.
	 *
	 * \return
	 * A pllm setting below the passed pllm value that gives a
	 * higher bin number for the new plld/pllm/clkod combination. 0 if
	 * no such value exists.
	 */
	u32 (*bin_prev_pllm)(struct clk *clkp, u32 plld, u32 pllm, u32 clkod);

	/**
	 * \brief Return fitness value based on VCO frequency.
	 *
	 * While PLLs already have a maximum and minimum allowable VCO
	 * frequeny, many prefer certain VCO frequencies over others. If the
	 * bin and frequency delta of a given pllm/plld/clkod combination are
	 * identical, the vco_fitness function is used to pick a preferred
	 * combination.
	 *
	 * \param clk
	 * The clk value passed to pll_calc.
	 *
	 * \param vco
	 * The VCO frequency in Hz.
	 *
	 * \param is_frac
	 * A fractional multiplier is used.
	 *
	 * \return
	 * The "fitness" of the VCO frequency. 0 for minimum fitness, UINT_MAX
	 * for maximum fitness.
	 */
	u32 (*vco_fitness)(struct clk *clkp, u32 vco, sbool is_frac);
};

/**
 * \brief Calculate ideal PLL settings.
 *
 * This calculates the ideal settings that can be used to generate an output
 * frequenccy given an input frequency and PLL properties. It iterates through
 * possible divider, multiplier, and post-divider values to find the best
 * combination. Preferences are sorted by:
 * - bin
 * - frequency delta
 * - VCO fitness
 * Where bin is the bin value returned by the bin callback, frequency delta
 * is the difference between the desired frequency and generated frequency,
 * and VCO fitness is the fitness value returned by the vco_fitness callback.
 *
 * \param clk
 * The PLL to calculate settings for.
 *
 * \param pll_data
 * The const parameters of the PLL that give allowable settings and
 * preferences.
 *
 * \param input
 * The input frequency in Hz.
 *
 * \param output
 * The desired output frequency in Hz.
 *
 * \param min
 * The minimum acceptable output frequency in Hz.
 *
 * \param max
 * The maximum acceptable output frequency in Hz.
 *
 * \param plld
 * Storage for generated divider value.
 *
 * \param pllm
 * Storage for generated multiplier value.
 *
 * \param pllfm
 * Storage for generated fractional multiplier value.
 *
 * \param clkod
 * Storage for generated post-divider value.
 *
 * \return
 * Returns frequency that would be produced with the calculated
 * plld/pllm/clkod values. 0 if no combination of settings could produce an
 * output between min and max.
 */
u32 pll_calc(struct clk *clkp, const struct pll_data *pll_d, u32 input, u32 output, u32 min, u32 max, u32 *plld, u32 *pllm, u32 *pllfm, u32 *clkod);

/**
 * \brief Base PLL initialization function
 *
 * This contains the common PLL initialization code. This includes setting
 * the default frequency if applicable.
 *
 * \param clk
 * The PLL to calculate settings for.
 *
 * \return
 * SUCCESS on success, error code on error.
 */
s32 pll_init(struct clk *clkp);

#endif
