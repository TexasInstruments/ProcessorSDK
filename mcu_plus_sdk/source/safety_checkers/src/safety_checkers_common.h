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
 *  \file     safety_checkers_common.h
 *
 *  \brief    This file contains safety checkers common data structures.
 *
 */

#ifndef SAFETY_CHECKERS_COMMON_H_
#define SAFETY_CHECKERS_COMMON_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (SOC_AM62X) || defined (SOC_AM62AX) || defined (SOC_AM62PX) || defined (SOC_J722S)
#include <kernel/dpl/SystemP.h>
#include <kernel/dpl/DebugP.h>
#define SAFETY_CHECKERS_log                                         DebugP_log
#elif defined (SOC_J7200) || defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#define SAFETY_CHECKERS_log                                         UART_printf
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Safety checkers status OK */
#define SAFETY_CHECKERS_SOK                                          (0)
/** \brief Safety checkers status FAIL */
#define SAFETY_CHECKERS_FAIL                                         (-1)
/** \brief Safety checkers status register data mismatch */
#define SAFETY_CHECKERS_REG_DATA_MISMATCH                            (-2)
/** \brief Safety checkers status Insufficient Buffer */
#define SAFETY_CHECKERS_INSUFFICIENT_BUFF                            (-3)

/** \brief Safety checkers default ID */
#define SAFETY_CHECKERS_PM_DEFAULT_ID                                (0x0U)
/** \brief Safety checkers valid Device Request flag */
#define SAFETY_CHECKERS_PM_DFT_VAL_REQUEST_FLAG                      (TISCI_MSG_FLAG_AOP)
/** \brief Safety checkers invalid device ID */
#define SAFETY_CHECKERS_PM_INVAL_DEV_ID                              (-1)
/** \brief Safety checkers valid parameter offset range */
#define SAFETY_CHECKERS_PM_VAL_PARAM_OFFSET_RANGE                    (0x0U)
/** \brief Safety checkers invalid parameter offset range */
#define SAFETY_CHECKERS_PM_INVAL_PARAM_OFFSET_RANGE                  (0x1U)

#if defined (SOC_AM62X) || defined (SOC_AM62AX) || defined (SOC_AM62PX) || defined (SOC_J722S)
/** \brief Safety checkers valid device ID */
#define SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID                         (TISCI_DEV_MCU_MCAN1)
/** \brief Safety checkers valid Clock ID */
#define SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_ID                          (TISCI_DEV_MCU_MCAN1_MCANSS_CCLK_CLK)
/** \brief Safety checkers valid device state to turn device */
#define SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_STATE                      (TISCI_MSG_VALUE_DEVICE_SW_STATE_ON)
/** \brief Safety checkers valid Clock ID */
#define SAFETY_CHECKERS_PM_VALID_CLOCK_ID                            (SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_ID)
/** \brief Safety checkers valid Parent Clock ID */
#define SAFETY_CHECKERS_PM_VALID_PARENT_CLOCK_ID                     (SAFETY_CHECKERS_PM_VALID_CLOCK_ID + 1U)
/** \brief Safety checkers valid Device ID */
#define SAFETY_CHECKERS_PM_VALID_DEVICE_ID                           (SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID)
/** \brief Safety checkers valid Clock freq requested */
#define SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_FREQ                        (80000000U)
/** \brief Safety checkers WKUP reset status register */
#define SAFETY_CHECKERS_PM_WKUP_CTRL_MMR_CFG0_RESET_SRC_STAT         (CSL_WKUP_CTRL_MMR_CFG0_RST_SRC)
/** \brief Safety checkers warm reset masking bit */
#define SAFETY_CHECKERS_PM_WARM_RESET_MASK			                 (CSL_WKUP_CTRL_MMR_CFG0_RST_SRC_SW_MCU_WARMRST_MASK)
/** \brief Safety checker default SciClient Timeout */
#define SAFETY_CHECKERS_DEFAULT_TIMEOUT                              (SystemP_WAIT_FOREVER)
#elif defined (SOC_J7200) || defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
/** \brief Safety checkers valid device ID */
#define SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID                         (TISCI_DEV_USB0)
/** \brief Safety checkers valid Clock ID */
#define SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_ID                          (TISCI_DEV_USB0_CLK_LPM_CLK)
/** \brief Safety checkers valid device state to turn device */
#define SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_STATE                      (TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF)
/** \brief Safety checkers valid Clock ID */
#define SAFETY_CHECKERS_PM_VALID_CLOCK_ID                            (TISCI_DEV_USB0_PIPE_REFCLK)
/** \brief Safety checkers valid Device ID */
#define SAFETY_CHECKERS_PM_VALID_DEVICE_ID                           (SAFETY_CHECKERS_PM_DEFAULT_ID)
/** \brief Safety checkers valid Parent Clock ID */
#define SAFETY_CHECKERS_PM_VALID_PARENT_CLOCK_ID                     (SAFETY_CHECKERS_PM_VALID_CLOCK_ID + 1U)
/** \brief Safety checkers valid Clock freq requested */
#define SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_FREQ                        (96000000U)
/** \brief Safety checkers WKUP reset status register */
#define SAFETY_CHECKERS_PM_WKUP_CTRL_MMR_CFG0_RESET_SRC_STAT         (CSL_WKUP_CTRL_MMR_CFG0_RESET_SRC_STAT)
/** \brief Safety checkers warm reset masking bit */
#define SAFETY_CHECKERS_PM_WARM_RESET_MASK			                 (CSL_WKUP_CTRL_MMR_CFG0_RESET_SRC_STAT_SW_MCU_WARMRST_MASK)
/** \brief Safety checker default SciClient Timeout */
#define SAFETY_CHECKERS_DEFAULT_TIMEOUT                              (SCICLIENT_SERVICE_WAIT_FOREVER)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef SAFETY_CHECKERS_COMMON_H_ */
