/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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

/**
 *  \file pdm_utils_soc.h
 *
 *  \brief SOC specific header for PDM utils library
 *
 */

#ifndef PDM_UTILS_SOC_H
#define PDM_UTILS_SOC_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <drivers/hw_include/csl_types.h>
#include <drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/soc/j722s/include/soc/j722s/clk_ids.h>
#if defined(BUILD_WKUP_R5)
#include <drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/include/clk_mux.h>
#include <drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/include/device_clk.h>
#include <drivers/device_manager/rm_pm_hal/rm_pm_hal_src/pm/include/psc.h>
#endif
#include <drivers/hw_include/j722s/cslr_psc.h>
#include <drivers/hw_include/j722s/cslr_soc_baseaddress.h>
#include <drivers/sciclient.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* HF Oscillator frequency for this Device */
#define PLL_HFOSC_REFCLK                             ((uint32_t)25000000U)
/* Maximum number of devices present on this Device */
#define PDM_UTILS_NUM_MAX_DEVICES                    (TISCI_DEV_TIMER7_CLKSEL_VD)
/* Maximum number of clocks present on this Device */
#define PDM_UTILS_NUM_MAX_CLOCKS                     (CLK_J722S_SAM62_PLL_CTRL_WRAP_MCU_0_CHIP_DIV24_CLK_CLK)
/* Maximum number of MAIN domain PLLs present on this Device */
#define PDM_UTILS_NUM_MAIN_PLL                       ((uint32_t)12U)
/* Maximum number of MCU domain PLLs present on this Device */
#define PDM_UTILS_NUM_MCU_PLL                        ((uint32_t)1U)
/* Maximum number of PSCs present on this Device */
#define PDM_UTILS_NUM_MAX_PSC                        ((uint32_t)2U)
/* maximum number of Main PDs present on this device */
#define PDM_UTILS_NUM_MAX_PD_MAIN                    ((uint32_t)22U)
/* maximum number of MCU PDs present on this device */
#define PDM_UTILS_NUM_MAX_PD_MCU                     ((uint32_t)2U)
/* maximum number of MCU LPSCs present on this device */
#define PDM_UTILS_NUM_MAX_LPSC_MCU                   ((uint32_t)11U)
/* maximum number of MAIN LPSCs present on this device */
#define PDM_UTILS_NUM_MAX_LPSC_MAIN                  ((uint32_t)96U)
/* Base address of configuration registers of MCU PLLs */
#define PDM_UTILS_MCU_PLL0_CFG_BASE                  (CSL_WKUP_PLL0_CFG_BASE)
/* Base address of configuration registers of MAIN PLLs */
#define PDM_UTILS_PLL0_CFG_BASE                      (CSL_PLL0_CFG_BASE)
/* Base address of configuration registers of Wakeup PSCs */
#define PDM_UTILS_PSC_WKUP_BASE_ADDR                 (CSL_WKUP_PSC0_BASE)
/* Base address of configuration registers of MAIN PSCs */
#define PDM_UTILS_PSC_MAIN_BASE_ADDR                 (CSL_PSCSS0_BASE)
/* Identifier given for Wakeup PSC */
#define J722S_PSC_INST_SAM62A_MCU_PSC_WRAP_WKUP_0    ((uint32_t)0U)
/* Identifier given for Main PSC */
#define J722S_PSC_INST_SAM67_MAIN_PSC_WRAP_MAIN_0    ((uint32_t)1U)
/* Macro to indicate system wait forever */
#define PDM_UTILS_WAIT_FOREVER                       SystemP_WAIT_FOREVER
/* Macro to define maximum number of Device groups in PM */
#define PDM_UTILS_DEVGRP_RANGE_ID_MAX                J722S_PM_DEVGRP_RANGE_ID_MAX
/* HFOSC clock ID for this device */
#define PDM_UTILS_HFOSC_CLOCK                        CLK_J722S_GLUELOGIC_HFOSC0_CLKOUT
/* Number of Clocks repersented in PET tool */
#define PDM_UTILS_PET_NUM_CLOCKS                    ((uint32_t)19U)
/* Macro for maximum clock tree depth */
#define PDM_UTILS_NUM_MAX_CLOCK_TREE_DEPTH          (16U)
/* Macro for maximum number of input clocks for a single device */
#define PDM_UTILS_NUM_MAX_DEVICE_CLOCKS             (400U)
/* Macro for length of PSC/PD/LPSC/CLOCK/DEVICE name */
#define PDM_UTILS_NAME_MAX_LENGTH                   (100U)
/* Macro for maximum devices connected to single LPSC */
#define PDM_UTILS_NUM_MAX_DEV_CONNECTED             (64U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*None*/

/* ========================================================================== */
/*                       Function Declarations                                */
/* ========================================================================== */

/**
 * \brief API to give base address of configuration registers for a given pscIndex
 *
 * \param pscIndex PSC index number
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_EFAIL
 */
uint32_t PdmUtils_getLpscBaseAddr(uint32_t pscIndex);

/**
 * \brief API to populate the given pointer to data object with number of available PSCs and PDs of
 *        the device
 *
 * \param buf  Pointer to data object to hold PSC, PD and LPSC status
 * \param size Size of the data object
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_EFAIL
 */
int32_t PdmUtils_getNumPsc(void *buf, uint32_t size);

#endif
