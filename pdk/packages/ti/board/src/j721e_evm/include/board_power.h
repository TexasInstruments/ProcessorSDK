/******************************************************************************
 * Copyright (c) 2019-2025 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/

/**
 *  \file   board_power.h
 *
 *  \brief  Board functions for PMIC
 *
 */

#ifndef _BOARD_POWER_H_
#define _BOARD_POWER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Include Files                                                             *
 *****************************************************************************/
#include <ti/csl/csl_types.h>
#include <ti/csl/cslr_device.h>

#include <ti/board/board.h>
#include <ti/csl/tistdtypes.h>
#include <stdio.h>
#include <stdbool.h>


#define BOARD_PMIC_LP87561_I2C_SLAVE_ADDR  (0x61U)
#define BOARD_PMIC_LP87561_I2C_INSTANCE    (0x00U)
#define BOARD_PMIC_LP87561_BUCK0_VOUT_REG  (0x0AU)
#define BOARD_PMIC_LP87561_VOLTAGE_0V9_VAL (0x39U)

#define BOARD_PMIC_I2C_SLAVE_ADDR          (0x58U)
#define BOARD_PMIC_I2C_INSTANCE            (0)
#define BOARD_PMIC_LDO1_CTRL_REG           (0x50U)
#define BOARD_PMIC_LDO1_VOLTAGE_REG        (0x51U)
#define BOARD_PMIC_LDO1_VSEL_3V3           (0x31U)
#define BOARD_PMIC_LDO1_VSEL_1V8           (0x13U)

/* GPIO port and pin numbers for SDIO 1V8 enable */
#define BOARD_SDIO_1V8_EN_PIN_NUM          (117U)


/* PMIC Register offsets */
#define TPS6594X_REG_DEV_REV                     (0x01U)
#define TPS6594X_GPIO4_CONF                      (0x34U)
#define TPS6594X_MASK_GPIO1_8_FALL               (0x4FU)
#define TPS6594X_INT_GPIO1_8                     (0x64U)
#define TPS6594X_INT_STARTUP                     (0x65U)
#define TPS6594X_INT_MISC                        (0x66U)
#define TPS6594X_CONFIG_1                        (0x7DU)
#define TPS6594X_FSM_I2C_TRIGGERS                (0x85U)
#define TPS6594X_FSM_NSLEEP_TRIGGERS             (0x86U)
#define TPS6594X_RTC_CTRL_2                      (0xC3U)

#define TPS6594X_DEV_REV_DEV_ID                  (0xFFU)

#define TPS6594X_INT_STARTUP_NPWRON_START_INT    (1 << 0)
#define TPS6594X_INT_STARTUP_ENABLE_INT          (1 << 1)
#define TPS6594X_INT_STARTUP_RTC_INT             (1 << 2)
#define TPS6594X_INT_STARTUP_FSD_INT             (1 << 4)
#define TPS6594X_INT_STARTUP_SOFT_REBOOT_INT     (1 << 5)

#define TPS6594X_INT_MISC_BIST_PASS_INT          (1 << 0)
#define TPS6594X_INT_MISC_EXT_CLK_INT            (1 << 1)
#define TPS6594X_INT_MISC_TWARN_INT              (1 << 3)

#define TPS6594X_CONFIG_NSLEEP1_MASK             (1 << 6)
#define TPS6594X_CONFIG_NSLEEP2_MASK             (1 << 7)

#define TPS6594X_FSM_I2C_TRIGGERS_I2C0           (1 << 0)

#define TPS6594X_FSM_NSLEEP_NSLEEP1B             (1 << 0)
#define TPS6594X_FSM_NSLEEP_NSLEEP2B             (1 << 1)

#define TPS6594X_RTC_CTRL_REG_GET_TIME           (1 << 6)
#define TPS6594X_RTC_CTRL_REG_STOP_RTC           (1 << 0)
#define TPS6594X_RTC_INTERRUPTS_REG_IT_ALARM     (1 << 3)

#define TPS6594X_GPIO4_CONF_LP_WKUP1             (6 << 5)

#define TPS6594X_RTC_CTRL_2_LP_STANDBY_SEL       (1 << 3)

#define TPS6594X_INT_GPIO1_8_GPIO4_INT           (1 << 3)

#define TPS6594X_I2C_WR_DELAY                    (20000U)

/**
 *  \brief    Function to power off the PMIC.
 *
 *  \param slaveAddr [IN] PMIC slave address
 *
 *  \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_pmPowerOff(uint32_t slaveAddr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BOARD_POWER_H_ */
