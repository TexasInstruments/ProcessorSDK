/*
 * Copyright (c) 2018-2022, Texas Instruments Incorporated
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <ti/csl/cslr_device.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/src/UART_osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/src/ip/rat/V0/csl_rat.h>

#include "sbl_slave_core_boot.h"
#include "sbl_sci_client.h"
#include "sbl_ver.h"
#include "sbl_soc.h"
#include "sbl_soc_cfg.h"
#include "sbl_log.h"
#include "sbl_profile.h"

/* If BUILD_XIP is defined then SBL call SBL_enableXIPMode() to update global variable isXIPEnable */
#if defined(BUILD_XIP) || defined(OSPI_NAND_BOOT)
#include <ti/boot/sbl/src/ospi/sbl_ospi.h>
#endif

#if defined(BOOT_EMMC_BOOT0)
#include <ti/boot/sbl/src/emmc/sbl_emmc.h>
#endif

#if !defined(BOOT_PERF)
#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_info_ddr.h>
#elif defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_info_ddr.h>
#elif defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_info_ddr.h>
#elif defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_info_ddr.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_info_ddr.h>
#endif
#endif

#if defined(SBL_ENABLE_HLOS_BOOT)
#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_utils.h>
#elif defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_utils.h>
#elif defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_utils.h>
#elif defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_utils.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_utils.h>
#endif
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief  CSL Reset Vectors. 
 *
 *
 *  \param  None
 *
 *  \return None
 *
 */
void _resetvectors (void);

/**
 * \brief  Unlocks MMR registers
 *
 * \return  Board_STATUS
 */
extern Board_STATUS Board_unlockMMR(void);