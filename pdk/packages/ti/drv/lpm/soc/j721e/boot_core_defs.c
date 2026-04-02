/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
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
#include <ti/drv/lpm/soc/j721e/boot_core_defs.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

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

const uint32_t ospi_main_domain_flash_rtos_images[NUM_BOOT_STAGES] = {
    MAIN_DOMAIN_APPS_FLASH_ADDR,
    /* Load remaining cores before MPU/HLOS to avoid contention for control of Flash */
    MAIN_DOMAIN_APPS_FLASH_ADDR2,
#if defined(MPU1_HLOS_BOOT_ENABLED) || defined(MPU1_HLOS_BOOT_ONLY_ENABLED)
    MAIN_DOMAIN_HLOS
#else
    MAIN_DOMAIN_APPS_FLASH_ADDR3
#endif
};

/* Number of Main Domain cores that can be booted by the
 * sample application for J721E SOC */
uint8_t num_slave_cpus = sizeof(sbl_late_slave_core_info) / \
                         sizeof(sblSlaveCoreInfo_t);

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

