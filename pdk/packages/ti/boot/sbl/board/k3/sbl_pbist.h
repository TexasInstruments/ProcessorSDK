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

#ifndef SBL_PBIST_H_
#define SBL_PBIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined(SOC_J784S4) || defined(SOC_J742S2)
#define NUM_BIST_TESTS                                 (12U)
#elif defined(SOC_J721S2)
#define NUM_BIST_TESTS                                 (8U)
#endif

#define KICK0_UNLOCK                                   (0x68EF3490U)
#define KICK1_UNLOCK                                   (0xD172BC5AU)

#define PLL0_LOCKKEY0                                  (0x00680010U)
#define PLL0_LOCKKEY1                                  (0x00680014U)
#define PLL1_LOCKKEY0                                  (0x00681010U)
#define PLL1_LOCKKEY1                                  (0x00681014U)
#define PLL2_LOCKKEY0                                  (0x00682010U)
#define PLL2_LOCKKEY1                                  (0x00682014U)
#define PLL3_LOCKKEY0                                  (0x00683010U)
#define PLL3_LOCKKEY1                                  (0x00683014U)
#define PLL4_LOCKKEY0                                  (0x00684010U)
#define PLL4_LOCKKEY1                                  (0x00684014U)
#define PLL5_LOCKKEY0                                  (0x00685010U)
#define PLL5_LOCKKEY1                                  (0x00685014U)
#define PLL6_LOCKKEY0                                  (0x00686010U)
#define PLL6_LOCKKEY1                                  (0x00686014U)
#define PLL7_LOCKKEY0                                  (0x00687010U)
#define PLL7_LOCKKEY1                                  (0x00687014U)
#define PLL8_LOCKKEY0                                  (0x00688010U)
#define PLL8_LOCKKEY1                                  (0x00688014U)
#define PLL9_LOCKKEY0                                  (0x00689010U)
#define PLL9_LOCKKEY1                                  (0x00689014U)
#define PLL12_LOCKKEY0                                 (0x0068C010U)
#define PLL12_LOCKKEY1                                 (0x0068C014U)
#define PLL14_LOCKKEY0                                 (0x0068E010U)
#define PLL14_LOCKKEY1                                 (0x0068E014U)
#define PLL16_LOCKKEY0                                 (0x00690010U)
#define PLL16_LOCKKEY1                                 (0x00690014U)
#define PLL17_LOCKKEY0                                 (0x00691010U)
#define PLL17_LOCKKEY1                                 (0x00691014U)
#define PLL19_LOCKKEY0                                 (0x00693010U)
#define PLL19_LOCKKEY1                                 (0x00693014U)
#define PLL25_LOCKKEY0                                 (0x00699010U)
#define PLL25_LOCKKEY1                                 (0x00699014U)
#define PLL26_LOCKKEY0                                 (0x0069A010U)
#define PLL26_LOCKKEY1                                 (0x0069A014U)
#if defined(SOC_J784S4) || defined(SOC_J742S2)
#define PLL27_LOCKKEY0                                 (0x0069B010U)
#define PLL27_LOCKKEY1                                 (0x0069B014U)
#define PLL28_LOCKKEY0                                 (0x0069C010U)
#define PLL28_LOCKKEY1                                 (0x0069C014U)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief  Function to run the PBIST Positive/Negative Tests
 *
 * \param  instanceId  Instance ID
 *
 * \return None
 */
void SBL_runPBIST(uint32_t instanceId);

/**
 * \brief  Function to reset Main Domain
 *
 * \param  None
 *
 * \return CSL_PASS for success, CSL_EFAIL for failure
 */
int32_t SBL_swResetMainDomain(void);

/**
 * \brief  Function to recover Main Domain to a state prior to reset
 *
 * \param  None
 *
 * \return None
 */
void SBL_mainDomainBootSetup(void);

/**
 * \brief  Function to perform BIST and reset Main Domain before TIFS Load
 *
 * \param  None
 *
 * \return None
 */
void SBL_bistMainDomainReset(void);

/**
 * \brief  Function to unlock PLL MMRs
 *
 * \param  None
 *
 * \return None
 */
void SBL_unlockPllMmrs(void);

/**
 * \brief  Function to initialize the MAIN domain PLL clocks with default values
 *
 * \param  None
 *
 * \return Board_STATUS
 */
extern Board_STATUS Board_PLLInitMain(void);

/**
 * \brief clock Initialization function for MAIN domain
 *
 * Enables different power domains and peripheral clocks of the SoC.
 * Enabling the power domains is mandatory before accessing using
 * board interfaces connected to those peripherals.
 *
 * \param  None
 *
 * \return Board_STATUS
 *
 */
extern Board_STATUS Board_moduleClockInitMain(void);

/**
 *  \brief  Sciclient Board Cfg PM
 *
 *
 *  \param  devGroup     SoC defined SYSFW devgrp
 *  \param  boardCfgInfo SYSFW board configurations
 *
 *  \return None
 *
 */
extern void SBL_SciclientBoardCfgPm(uint32_t devGroup, Sciclient_DefaultBoardCfgInfo_t *boardCfgInfo);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SBL_PBIST_H_ */