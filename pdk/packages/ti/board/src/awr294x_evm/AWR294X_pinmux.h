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

/**
 *
 * \file   AWR294X_pinmux.h
 *
 * \brief  This file contains pad configure register offsets and bit-field 
 *         value macros for different configurations,
 *
 *           BIT[9]		    PUPDSEL		    Pullup/PullDown Selection 0 -- Pull Down
 *           BIT[8]		    PI              Pull Inhibit/Pull Disable 0 -- Enable
 *           BIT[3:0]		FUNC_SEL		Function Select
 *
 */

#ifndef _AWR294X_PIN_MUX_H_
#define _AWR294X_PIN_MUX_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/board/src/awr294x_evm/include/pinmux.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc/awr294x/src/cslr_mss_iomux.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define PIN_MODE(mode)	                (mode)
#define PINMUX_END                      (-1)

/** \brief Active mode configurations */
/** \brief Resistor disable */
#define PIN_PULL_DISABLE                (0x1U << 8U)  
/** \brief Pull direction */
#define	PIN_PULL_DIRECTION              (0x1U << 9U)


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief Pinmux configuration data for the board. Auto-generated from 
           Pinmux tool. */
extern pinmuxBoardCfg_t gAWR294XPinmuxData[];

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _AWR294X_PIN_MUX_H_ */
