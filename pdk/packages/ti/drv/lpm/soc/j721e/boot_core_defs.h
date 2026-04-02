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
 *  \file     boot_core_defs.h
 *
 *  \brief    Header file for slave boot core definitions
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/boot/sbl/src/mmcsd/sbl_mmcsd.h>
#if defined(SOC_J721E)
#include <ti/drv/lpm/soc/j721e/boot_stage_defs.h>
#endif
#if defined(SOC_J7200)
#include <ti/drv/lpm/soc/j7200/boot_stage_defs.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Maximum number of boot cores per boot stage */
#define MAX_CORES_PER_STAGE 5

#define MAX_APPIMAGE_NAME_LEN   (16)

/* Macros representing the offset where the App Image has to be written/Read from
   the OSPI Flash.
*/
#define OSPI_OFFSET_SI               (0x2E0000U)
#define OSPI_OFFSET_SYSFW            (0x80000U)

/* Location of ATF/OPTEE - used for both Linux and QNX */
#define OSPI_OFFSET_A72IMG1          (0x1C0000U)
/* Location of Kernel for Linux or IFS for QNX */
#define OSPI_OFFSET_A72IMG2          (0x7C0000U)
/* Location of DTB for Linux */
#define OSPI_OFFSET_A72IMG3          (0x1EC0000U)

#define MAIN_DOMAIN_APPS_FLASH_ADDR  (0x1FC0000U)
#define MAIN_DOMAIN_APPS_FLASH_ADDR2 (0x27C0000U)
#define MAIN_DOMAIN_APPS_FLASH_ADDR3 (0x37C0000U)

/* Location Address used as flag to indicate loading of
 * all HLOS appimages for OSPI */
#define MAIN_DOMAIN_HLOS             (0x1)
/* Name indicating that appimages for HLOS should be
 * loaded from MMCSD filesystem */
#define MAIN_DOMAIN_HLOS_NAME        "hlos"

/* Important RAM address macros */
#define ATF_START_RAM_ADDR          (0x70000000)

/* this whole structure fits inside sblbootbuff segment in memory */
#define SBL_MAX_BOOT_BUFF_SIZE      (0x4000000-0x10)

/* ========================================================================== */
/*                          External Dependencies                             */
/* ========================================================================== */
/* Definition of available Main Domain cores that can be booted
 * by the sample application for J721E SOC */
extern const sblSlaveCoreInfo_t sbl_late_slave_core_info[];

extern const sblSlaveCoreInfo_t sbl_late_slave_core_stages_info[NUM_BOOT_STAGES]
                                                               [MAX_CORES_PER_STAGE];

extern const uint32_t ospi_main_domain_flash_rtos_images[NUM_BOOT_STAGES];

extern TCHAR mmcsd_main_domain_rtos_image_name[NUM_BOOT_STAGES]
                                              [MAX_APPIMAGE_NAME_LEN];

/* Number of Main Domain cores that can be booted by the
 * sample application for J721E SOC */
extern uint8_t num_slave_cpus;

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

