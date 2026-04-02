/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 */

/**
 *  \defgroup SAFETY_CHECKERS Safety checkers
 * 
 *  @{
 * 
 *   The Safety Checkers Library provides productized APIs for ensuring the
 *   safety of TIFS, RM and PM modules.C provides these interfaces to assist
 *   in the development of software applications involving Functional Safety.
 * 
 *   The high-level design of the Safety checker implementation for the TIFS,
 *   RM and PM module. The primary objective of implementing the safety checker
 *   is to make sure the detection of the failure modes for TIFS, RM and PM module
 *   are handled properly. Also providing the safety examples as a reference which
 *   calls the checker APIs to verify the module operations are done properly during
 *   the boot.
 */
/** @} */

 /**
 *  \ingroup  SAFETY_CHECKERS
 *  \defgroup PM_SAFETY_CHECKERS PM Safety Checkers Library
 *
 *  @{
 *
 *    PM safety checkers library APIs are integrated into the safety application to verify
 *    the PSC/PLL configurations and validate the current PSC and PLL configurations against
 *    golden reference. Also provides the PLL lock API which is responsible for locking a PLL
 *    control module registers by preventing the stray writes.
 * 
 */

/**
 *  \file     safety_checkers_pm.h
 *
 *  \brief    This file contains PM safety checkers library interfaces and related data structures.
 *
 */

/** @} */

#ifndef SAFETY_CHECKERS_PM_H_
#define SAFETY_CHECKERS_PM_H_

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

/* None */

/**
 * @ingroup PM_SAFETY_CHECKERS
 *
 * @defgroup SAFETY_CHECKERS_PM_DATA_STRUCTURES  SAFETY_CHECKERS_PM structure definition
 *  @{
 */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *
 * \brief   Structure to hold the base address and the length of PLLs
 *
 */
typedef struct
{
    /* Base address of PLL control module registers */
    uint32_t baseAddr;
	/* Pointer to the register offset array for each PLL */
	uint32_t *regOffsetArr;
    /* Total length of each PLL registers */
    uint32_t length;
} SafetyCheckers_PmPllData;

/**
 *
 * \brief   Structure to hold the base address and the number of
 *          Module Domain(MD) stat and Power Domain(PD) stat registers of PSC
 *
 */
typedef struct
{
    /* Domain base address of PSC module registers */
    uint32_t baseAddr;
    /* Total number of PD stat registers */
    uint32_t pdStat;
    /* Total number of MD stat registers */
    uint32_t mdStat;
} SafetyCheckers_PmPscData;

/** @} */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/**
 * @ingroup PM_SAFETY_CHECKERS
 *
 * @defgroup SAFETY_CHECKERS_PM_MODULE_API  SAFETY_CHECKERS PM checkers APIs
 *  @{
 */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief    API to get register configuration for PSC module.
 *            E.g. When the PSC register config is requested, SafetyCheckers_pmGetPscRegCfg
 *            API collects the PSC configuration for devices present on the given
 *            platform. Register configuration includes PDSTAT and MDSTAT register values.
 *            The collected configuration will be stored in pscRegCfg array.
 *
 *  \param    pscRegCfg         [IN/OUT]       Pointer to store the PSC Register configuration
 *  \param    size              [IN]           Size of register configuration memory
 *
 *  \return   SAFETY_CHECKERS_SOK in case of success, else failure.
 *
 */
int32_t SafetyCheckers_pmGetPscRegCfg(uintptr_t *pscRegCfg, uint32_t size);

/**
 *  \brief    API to read back and compares the current register configuration for PSC module.
 *            E.g. When the PSC register verify is requested, SafetyCheckers_pmVerifyPscRegCfg
 *            API collects the PSC configuration for devices present on the given platform 
 *            and compares with the received register configuration.
 *
 *  \param    pscRegCfg         [IN/OUT]       Pointer to store the PSC golden register configuration
 *  \param    size              [IN]           Size of golden configuration memory
 *
 *  \return   SAFETY_CHECKERS_SOK in case of success, else failure.
 *
 */
int32_t SafetyCheckers_pmVerifyPscRegCfg(const uintptr_t *pscRegCfg, uint32_t size);

/**
 *  \brief    API to get register configuration for PLL module.
 *            E.g. When the PLL register config is requested, SafetyCheckers_pmGetPllRegCfg
 *            API collects the PLL configuration for devices present on the given platform.
 *            Register configuration includes PLL register values. The collected configuration
 *            will be stored in pllRegCfg array.
 *
 *  \param    pllRegCfg         [IN/OUT]       Pointer to store the PLL Register configuration
 *  \param    size              [IN]           Size of register configuration memory
 *
 *  \return   SAFETY_CHECKERS_SOK in case of success, else failure.
 *
 */
int32_t SafetyCheckers_pmGetPllRegCfg(uintptr_t *pllRegCfg, uint32_t size);

/**
 *  \brief    API to read back and compares the current register configuration for PLL module.
 *            E.g. When the PLL register verify is requested, SafetyCheckers_pmVerifyPllRegCfg
 *            API collects the PLL configuration for devices present on the given platform 
 *            and compares with the received register configuration.
 *
 *  \param    pllRegCfg         [IN/OUT]       Pointer to store the PLL golden register configuration
 *  \param    size              [IN]           Size of golden configuration memory
 *
 *  \return   SAFETY_CHECKERS_SOK in case of success, else failure.
 *
 */
int32_t SafetyCheckers_pmVerifyPllRegCfg(const uintptr_t *pllRegCfg, uint32_t size);

/**
 *  \brief    API to lock the PLL control module registers.
 *            E.g. When the PLL register lock is requested, SafetyCheckers_pmRegisterLock API 
 *            performs write operations to KICK registers which prevents the stray writes
 *            to PLL control module registers.
 *
 *  \return   SAFETY_CHECKERS_SOK in case of success, else failure.
 *
 */
int32_t SafetyCheckers_pmRegisterLock(void);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SAFETY_CHECKERS_PM_H_ */

/** @} */
