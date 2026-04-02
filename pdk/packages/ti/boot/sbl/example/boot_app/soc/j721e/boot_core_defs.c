/*
 *  Copyright (c) Texas Instruments Incorporated 2022
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

/**
 *  \file     boot_core_defs.c
 *
 *  \brief    This file defines available main domain slave cores and order of booting
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/boot/sbl/src/rprc/sbl_rprc_parse.h>
#include <ti/boot/sbl/soc/k3/sbl_soc_cfg.h>
#include "boot_core_defs.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Definition of available Main Domain cores that can be booted
 * by the sample application for J721E SOC */
const sblSlaveCoreInfo_t sbl_late_slave_core_info[] =
{
    /* MPU1_CPU0 info */
    {
    SBL_PROC_ID_MPU1_CPU0,
    SBL_DEV_ID_MPU1_CPU0,
    SBL_CLK_ID_MPU1_CPU0,
    SBL_MPU1_CPU0_FREQ_HZ,
    },
    /* MPU1_CPU1 info */
    {
    SBL_PROC_ID_MPU1_CPU1,
    SBL_DEV_ID_MPU1_CPU1,
    SBL_CLK_ID_MPU1_CPU1,
    SBL_MPU1_CPU1_FREQ_HZ,
    },
    /* MCU2_CPU0 info */
    {
    SBL_PROC_ID_MCU2_CPU0,
    SBL_DEV_ID_MCU2_CPU0,
    SBL_CLK_ID_MCU2_CPU0,
    SBL_MCU2_CPU0_FREQ_HZ,
    },
    /* MCU2_CPU1 info */
    {
    SBL_PROC_ID_MCU2_CPU1,
    SBL_DEV_ID_MCU2_CPU1,
    SBL_CLK_ID_MCU2_CPU1,
    SBL_MCU2_CPU1_FREQ_HZ,
    },
    /* MCU3_CPU0 info */
    {
    SBL_PROC_ID_MCU3_CPU0,
    SBL_DEV_ID_MCU3_CPU0,
    SBL_CLK_ID_MCU3_CPU0,
    SBL_MCU3_CPU0_FREQ_HZ,
    },
    /* MCU3_CPU1 info */
    {
    SBL_PROC_ID_MCU3_CPU1,
    SBL_DEV_ID_MCU3_CPU1,
    SBL_CLK_ID_MCU3_CPU1,
    SBL_MCU3_CPU1_FREQ_HZ,
    },
    /* DSP1_C66X info */
    {
    SBL_PROC_ID_DSP1_C66X,
    SBL_DEV_ID_DSP1_C66X,
    SBL_CLK_ID_DSP1_C66X,
    SBL_DSP1_C66X_FREQ_HZ,
    },
    /* DSP2_C66X info */
    {
    SBL_PROC_ID_DSP2_C66X,
    SBL_DEV_ID_DSP2_C66X,
    SBL_CLK_ID_DSP2_C66X,
    SBL_DSP2_C66X_FREQ_HZ,
    },
    /* DSP1_C7X info */
    {
    SBL_PROC_ID_DSP1_C7X,
    SBL_DEV_ID_DSP1_C7X,
    SBL_CLK_ID_DSP1_C7X,
    SBL_DSP1_C7X_FREQ_HZ,
    },
};

const sblSlaveCoreInfo_t sbl_late_slave_core_stages_info[NUM_BOOT_STAGES][MAX_CORES_PER_STAGE] =
{
    {
        /* MCU2_CPU0 info */
        {
        SBL_PROC_ID_MCU2_CPU0,
        SBL_DEV_ID_MCU2_CPU0,
        SBL_CLK_ID_MCU2_CPU0,
        SBL_MCU2_CPU0_FREQ_HZ,
        },
        /* MCU2_CPU1 info */
        {
        SBL_PROC_ID_MCU2_CPU1,
        SBL_DEV_ID_MCU2_CPU1,
        SBL_CLK_ID_MCU2_CPU1,
        SBL_MCU2_CPU1_FREQ_HZ,
        },
        {
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID
        },
        {
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID
        },
        {
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID
        }
    },
    /* Load remaining cores before MPU1 to avoid contention
     * for control of Flash or SD card */
    {
        /* MCU3_CPU0 info */
        {
        SBL_PROC_ID_MCU3_CPU0,
        SBL_DEV_ID_MCU3_CPU0,
        SBL_CLK_ID_MCU3_CPU0,
        SBL_MCU3_CPU0_FREQ_HZ,
        },
        /* MCU3_CPU1 info */
        {
        SBL_PROC_ID_MCU3_CPU1,
        SBL_DEV_ID_MCU3_CPU1,
        SBL_CLK_ID_MCU3_CPU1,
        SBL_MCU3_CPU1_FREQ_HZ,
        },
        /* DSP1_C66X info */
        {
        SBL_PROC_ID_DSP1_C66X,
        SBL_DEV_ID_DSP1_C66X,
        SBL_CLK_ID_DSP1_C66X,
        SBL_DSP1_C66X_FREQ_HZ,
        },
        /* DSP2_C66X info */
        {
        SBL_PROC_ID_DSP2_C66X,
        SBL_DEV_ID_DSP2_C66X,
        SBL_CLK_ID_DSP2_C66X,
        SBL_DSP2_C66X_FREQ_HZ,
        },
        /* DSP1_C7X info */
        {
        SBL_PROC_ID_DSP1_C7X,
        SBL_DEV_ID_DSP1_C7X,
        SBL_CLK_ID_DSP1_C7X,
        SBL_DSP1_C7X_FREQ_HZ,
        },
    },
    {
        /* MPU1_CPU0 info */
        {
        SBL_PROC_ID_MPU1_CPU0,
        SBL_DEV_ID_MPU1_CPU0,
        SBL_CLK_ID_MPU1_CPU0,
        SBL_MPU1_CPU0_FREQ_HZ,
        },
        {
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID
        },
        {
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID
        },
        {
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID
        },
        {
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID,
        SBL_INVALID_ID
        },
    }
};


#if defined(BOOT_OSPI)
const uint32_t ospi_main_domain_flash_rtos_images[NUM_BOOT_STAGES] = {
    MAIN_DOMAIN_APPS_FLASH_ADDR,
    /* Load remaining cores before MPU/HLOS to avoid contention for control of Flash */
    MAIN_DOMAIN_APPS_FLASH_ADDR2,
#if defined(MPU1_HLOS_BOOT_ENABLED)
    MAIN_DOMAIN_HLOS
#else
    MAIN_DOMAIN_APPS_FLASH_ADDR3
#endif
};
#endif
#if defined(BOOT_MMCSD) || defined(BOOT_EMMC_UDA)
TCHAR mmcsd_main_domain_rtos_image_name[NUM_BOOT_STAGES][MAX_APPIMAGE_NAME_LEN] = {
    "0:/lateapp1",
    /* Load remaining cores before MPU/HLOS to avoid contention for control of SD card */
    "0:/lateapp2",
#if defined(MPU1_HLOS_BOOT_ENABLED)
    MAIN_DOMAIN_HLOS_NAME,
#else
    "0:/lateapp3",
#endif
};
#endif

/* Defines boot order for the first stage of the Main Domain
 * boot sequence for J721E SOC */
cpu_core_id_t boot_order_first_stage[] =
{
    MCU2_CPU0_ID,
    MCU2_CPU1_ID
};

/* Defines boot order for the second stage of the Main Domain
 * boot sequence for J721E SOC */
cpu_core_id_t boot_order_second_stage[] =
{
    /* Load remaining cores before MPU/HLOS to avoid contention
     * for control of Flash or SD card */
    MCU3_CPU0_ID,
    MCU3_CPU1_ID,
    DSP1_C66X_ID,
    DSP2_C66X_ID,
    DSP1_C7X_ID
};

/* Defines boot order for the third stage of the Main Domain
 * boot sequence for J721E SOC */
cpu_core_id_t boot_order_third_stage[] =
{
    MPU1_CPU0_ID
};

/* Points to boot order arrays for each of the boot stages */
cpu_core_id_t *boot_array_stage[NUM_BOOT_STAGES] =
{
    boot_order_first_stage,
    boot_order_second_stage,
    boot_order_third_stage
};

/* Defines number of cores booted in each stage */
uint8_t num_cores_per_boot_stage[NUM_BOOT_STAGES] =
{
    sizeof(boot_order_first_stage) / sizeof(cpu_core_id_t),
    sizeof(boot_order_second_stage) / sizeof(cpu_core_id_t),
    sizeof(boot_order_third_stage) / sizeof(cpu_core_id_t)
};

