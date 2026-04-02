/*
*
* Copyright (c) 2025 Texas Instruments Incorporated
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
 *  \ingroup DRV_LPM_MODULE
 *  \defgroup DRV_LPM_BOOT_MODULE LPM Driver Boot API
 *            This is LPM driver boot related configuration parameters and
 *            API
 *
 *  @{
 */

/**
 *  \file lpm_boot.h
 *
 *  \brief LPM boot related parameters and API.
 */

#ifndef LPM_BOOT_H_
#define LPM_BOOT_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>

#include <ti/boot/sbl/soc/sbl_soc.h>
#include <ti/boot/sbl/soc/k3/sbl_slave_core_boot.h>
#include <ti/boot/sbl/soc/k3/sbl_profile.h>
#include <ti/boot/sbl/soc/k3/sbl_soc_cfg.h>
#include <ti/boot/sbl/soc/k3/sbl_qos.h>
#include <ti/boot/sbl/soc/k3/sbl_log.h>
#include <ti/boot/sbl/soc/k3/sbl_qos.h>

#include <ti/csl/cslr_gtc.h>

#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/board/board.h>
#include <ti/board/board_cfg.h>
#include <ti/board/src/flash/include/board_flash.h>
#if defined(SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_control.h>
#include <ti/drv/lpm/soc/j721e/boot_core_defs.h>
#endif
#if defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_control.h>
#include <ti/drv/lpm/soc/j7200/boot_core_defs.h>
#endif
#if defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_control.h>
#include <ti/drv/lpm/soc/j784s4/boot_core_defs.h>
#endif

#include <ti/drv/lpm/src/lpm_utils.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

/* PM Lib */
#include <ti/drv/pm/include/pm_types.h>
#include <ti/drv/pm/include/dmsc/pmlib_sysconfig.h>
#include <ti/drv/pm/include/dmsc/pmlib_clkrate.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/**
 *  \brief This structure contains boot paramters required by the SBL
 */
typedef struct Lpm_SblIncomingBootData_S
{
    uint8_t     sblBootBuff[SBL_MAX_BOOT_BUFF_SIZE+1];
    /**< LPM SBL boot buffer to hold boot information */
    uint32_t    sblBootSize;
    /**< LPM SBL boot size. */
    uint32_t    sblBootBuffIdx;
    /**< LPM SBL boot index. */
} Lpm_SblIncomingBootData;

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Boots up all main domain cores 
 * 
 * Boots R5Fs and DSPs with the RTOS/Baremtal firmwares present in Flash
 * Boots A72 core with Linux
 * 
 *
 *  \return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 */
int32_t Lpm_bootApp(void);

/**
 *  \brief Intializes the SBL and the Flash Driver
 */
void Lpm_bootAppInit(void);

/**
 *  \brief De-Intializes the SBL and the Flash Driver
 */
void Lpm_bootAppDeInit(void);

#endif /* LPM_BOOT_H_ */
