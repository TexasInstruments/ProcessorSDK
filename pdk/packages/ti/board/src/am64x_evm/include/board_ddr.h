/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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

/** \file board_ddr.h
 *
 *   \brief This file contains DDR timing parameters
 */

#ifndef BOARD_DDR_H_
#define BOARD_DDR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/csl_lpddr.h>
#include <ti/csl/csl_emif.h>
#include <ti/drv/sciclient/sciclient.h>

#include "board.h"
#include "board_pll.h"
#include "board_internal.h"

#define BOARD_DDR_CTL_CFG_BASE           (CSL_DDR16SS0_CTL_CFG_BASE)

#define BOARD_DDR_FSP_CLKCHNG_REQ_ADDR   (CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_REQ)
#define BOARD_DDR_FSP_CLKCHNG_ACK_ADDR   (CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_DDR4_FSP_CLKCHNG_ACK)

#define BOARD_CTRL_MMR_PART5_LOCK0       (CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0)
#define BOARD_CTRL_MMR_PART5_LOCK1       (CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0)

#define BOARD_DDR_SRAM_MAX               (512U)

#define BOARD_DDR_CTL_REG_OFFSET         (0)

#define LPDDR4__AUTO_TEMPCHK_VAL_0__REG_OFFSET  (162U)
#define LPDDR4__AUTO_TEMPCHK_VAL_0_MASK         (0x00FFFF00U)
#define LPDDR4__AUTO_TEMPCHK_VAL_0_SHIFT        (8U)
#define LPDDR4__AUTO_TEMPCHK_OP0_MASK           (7U)

#define BOARD_MAX_TEMP_CHECK_REFRESH_RATE_VALUE           (7U)
#define BOARD_TEMP_CHECK_REFRESH_RATE_VALUE_0_25_DERATING (6U)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BOARD_DDR_H_ */
