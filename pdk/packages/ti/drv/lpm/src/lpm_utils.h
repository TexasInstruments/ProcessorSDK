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
 *  \file     lpm_utils.h
 *
 *  \brief    This file contains common utility functions
 *
 */

#ifndef LPM_UTILS_H_
#define LPM_UTILS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdio.h>
#include <ti/csl/soc.h>

#if defined (SOC_J721E)
#include <ti/csl/soc/cslr_soc_ctrl_mmr.h>
#endif

/* SCI Client */
#include <ti/drv/sciclient/sciclient.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** \brief Address of the UART to used to display messages on console */
#define APP_UTILS_UART_CONSOLE_ADDR    (0x0U)

/** \brief Magic String to indicate successful completion */
#define APP_UTILS_STATUS_PASS   (0x50415353U)

/** \brief Magic String to indicate failures */
#define APP_UTILS_STATUS_FAIL   (0x46417376U)

/** \brief Status messages only, used limit to critical messages only */
#define APP_UTILS_PRINT_MSG_STATUS      (0x01U)
/** \brief Status messages only, used limit to critical messages only */
#define APP_UTILS_PRINT_MSG_NORMAL      (0x02U)
/** \brief Determines the log level enabled */
#define APP_UTILS_PRINT_MSG_LEVEL       (0x01U)
/** \brief Maximum size of the string that could be printed */
#define APP_UTILS_PRINT_MAX_STRING_SIZE (2000U)
/** \brief Demo / Example Status */
#define APP_UTILS_TEST_STATUS_INIT      (0U)
/** \brief Demo / Example Status */
#define APP_UTILS_TEST_STATUS_PASS      (2U)
/** \brief Demo / Example Status */
#define APP_UTILS_TEST_STATUS_FAIL      (3U)

/* These should match what is given in linker command file */
/** \brief Length of the section used to check section corruption */
#define APP_UTILS_LINKER_FILL_LENGTH    (0x100U)
/** \brief Pattern written in each sections */
#define APP_UTILS_LINKER_FILL_PATTERN   (0xFEAA55EFU)
/** \brief Sizeof Utility macro */
#define MODULEAPP_NUM_SEC_HOLES(x)     (sizeof (x) / sizeof (x[0]))
/** \brief SCICLIENT API timeout */
#define APP_SCICLIENT_TIMEOUT          (SCICLIENT_SERVICE_WAIT_FOREVER)

#define MSG_NORMAL              (APP_UTILS_PRINT_MSG_NORMAL)
/**< Message type */
#define MSG_STATUS              (APP_UTILS_PRINT_MSG_STATUS)
/**< Message type */
/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/** \brief Print utility used to display on CCS console of UART terminal.
 *
 *  type        :Either APP_UTILS_PRINT_MSG_STATUS or APP_UTILS_PRINT_MSG_NORMAL
 *  baseAddr    :Not used for now
 *  pcString    :printf, string
 */
void AppUtils_Printf(uint32_t type, const char *pcString, ...);

#ifdef __cplusplus
}
#endif

#endif /* LPM_UTILS_H_ */
