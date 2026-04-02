/**
 *   Copyright (c) Texas Instruments Incorporated 2018
 *   All rights reserved.
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
*  \file vhwa_examples_common.h
*
*  \brief Decaration of structs and function prototypes common to all examples
*/

#ifndef VPAC_EX_HWA_COMMON_H_
#define VPAC_EX_HWA_COMMON_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>
#if defined (VHWA_PDK)
#include     "ti/csl/soc.h"
#include     "ti/csl/hw_types.h"
#include     "ti/drv/uart/UART.h"
#include     "ti/drv/uart/UART_stdio.h"
#include     "ti/board/board.h"
#include     "examples/include/vhwa_common_crc.h"
#elif defined VHWA_MCU_PLUS_SDK
#include     "kernel/dpl/DebugP.h"
#include     "ti_drivers_config.h"
#include     "ti_drivers_open_close.h"
#include     "ti_board_open_close.h"
#include     "drivers/gtc.h"
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if 1
#define VHWA_EXAMPLE_MEM_START_ADDR     	(0x80000000U)
#define VHWA_EXAMPLE_BUFF_START_ADDR    	(0x90000000U)
#if defined (SAFERTOS)
#define VHWA_EXAMPLE_CONFIG_BUFF_START_ADDR	(0x7012C84CU)
#else
#define VHWA_EXAMPLE_CONFIG_BUFF_START_ADDR (0x70040000U)
#endif
#else
#define VHWA_EXAMPLE_MEM_START_ADDR     (0x10000000U)
#define VHWA_EXAMPLE_BUFF_START_ADDR    (0x10000000U)
#endif

#if defined(VHWA_MCU_PLUS_SDK)
    #define VHWA_UDMA_INST_ID_0    UDMA_INST_ID_0
#elif defined (VHWA_PDK)
    #define VHWA_UDMA_INST_ID_0    UDMA_INST_ID_MAIN_0
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
#if defined VHWA_PDK
typedef struct Udma_DrvObj           VhwaAl_Udma_DrvObj_Struct;
#elif defined VHWA_MCU_PLUS_SDK
typedef Udma_DrvObjectInt            VhwaAl_Udma_DrvObj_Struct;
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if !defined (VHWA_MCU_PLUS_SDK)
extern void App_print(const char *format, ...);
extern void App_startTimer(void);
extern uint64_t App_getTimerTicks(void);
extern void App_udmaPrint(const char *str);
#endif
extern void Vhwa_print(const char *format, ...);
extern uint64_t Vhwa_getTime(void);
extern uint64_t Vhwa_getTimeUsec(void);

#endif /*VPAC_EX_HWA_COMMON_H_*/
