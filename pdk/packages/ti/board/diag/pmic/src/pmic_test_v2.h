/**
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   pmic_test_v2.h
 */

#ifndef _PMIC_TEST_V2_H
#define _PMIC_TEST_V2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/csl/soc.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/pmic/pmic.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/board/src/devices/common/common.h>

#include "board.h"
#include "board_cfg.h"
#include "board_utils.h"
#include "diag_common_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_DIAG_TPS6594X_PMICA_VOLTAGE_VAL    (1700U)/* 1.7V */
#define BOARD_DIAG_TPS6594X_PMICB_VOLTAGE_VAL    (850U)/* 0.85V */
#define BOARD_DIAG_LP8764X_PMICB_VOLTAGE_VAL     (1700U)/* 1.7V */

/**
 * Structure having Pmic data.  
 */
typedef struct BoardDiag_PmicData_s
{
	uint8_t deviceType;
	uint8_t intfI2cType;
    uint8_t slaveAddress;
	uint8_t qaSlaveAddress;
	uint16_t pwrRsrc;
    uint16_t voltVal;
} BoardDiag_PmicData_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PMIC_TEST_V2_H */
