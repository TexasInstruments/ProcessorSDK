/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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

/* Protection against multiple inclusion */
#ifndef BOOT_APP_PRIV_H_
#define BOOT_APP_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* files related to SBL */
#include <ti/boot/sbl/src/rprc/sbl_rprc_parse.h>
#include <ti/boot/sbl/src/mmcsd/sbl_mmcsd.h>
#include <ti/boot/sbl/src/emmc/sbl_emmc.h>
#include <ti/boot/sbl/src/ospi/sbl_ospi.h>
#include <ti/boot/sbl/soc/sbl_soc.h>
#include <ti/boot/sbl/soc/k3/sbl_slave_core_boot.h>
#include <ti/boot/sbl/soc/k3/sbl_soc_cfg.h>

/* files related to CSL */
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/soc.h>
#include <ti/csl/cslr.h>

/*files related to borad */
#include <ti/board/board.h>
#include <ti/board/board_cfg.h>
#include <ti/board/src/flash/include/board_flash.h>
#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_control.h>
#include <soc/j721e/boot_core_defs.h>
#endif
#if defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_control.h>
#include <soc/j7200/boot_core_defs.h>
#endif
#if defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_control.h>
#include <soc/j721s2/boot_core_defs.h>
#endif
#if defined(SOC_J784S4) 
#include <ti/board/src/j784s4_evm/include/board_control.h>
#include <soc/j784s4/boot_core_defs.h>
#endif
#if defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_control.h>
#include <soc/j784s4/boot_core_defs.h>
#endif

/* files related to osal */
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

/* files related to uart */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>

#include <ti/drv/spi/soc/SPI_soc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#if defined(SAFETY_CHECKER_LOOP_ENABLED)
/* Safety checkers timer ID */
#define BOOT_APP_SAFETY_CHECKERS_TIMER_ID                    (2U)
/* Safety checkers verification time period, timer ISR execution takes 118us */
#define BOOT_APP_SAFETY_CHECKERS_TIMER_PERIOD                (1000U)
/* Number of timer interrupts for the safety checkers execution*/
#define BOOT_APP_SAFETY_CHECKERS_TIMER_MAX_INTERRUPTS        (10U)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern const sblSlaveCoreInfo_t sbl_late_slave_core_info[];

extern const sblSlaveCoreInfo_t sbl_late_slave_core_stages_info[NUM_BOOT_STAGES]
                                                               [MAX_CORES_PER_STAGE];

extern const uint32_t ospi_main_domain_flash_rtos_images[NUM_BOOT_STAGES];

extern TCHAR mmcsd_main_domain_rtos_image_name[NUM_BOOT_STAGES]
                                              [MAX_APPIMAGE_NAME_LEN];

/* Defines boot order for the first stage of the Main Domain
 * boot sequence for J721E SOC */
extern cpu_core_id_t boot_order_first_stage[];

/* Defines boot order for the second stage of the Main Domain
 * boot sequence for J721E SOC */
extern cpu_core_id_t boot_order_second_stage[];

/* Points to boot order arrays for each of the boot stages */
extern cpu_core_id_t *boot_array_stage[];

/* Defines number of cores booted in each stage */
extern uint8_t num_cores_per_boot_stage[];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if defined(MPU1_HLOS_BOOT_ENABLED)
/* Function to clean the MCU R5 cache for a given start address and given memory size */
void BootApp_McuDCacheClean(void *addr, uint32_t size);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BOOT_APP_PRIV_H_ */
