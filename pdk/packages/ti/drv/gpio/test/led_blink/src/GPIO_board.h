/**
 *  \file   GPIO_board.h
 *
 *  \brief  GPIO board header file.
 *
 */

/*
 * Copyright (C) 2014 - 2023 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef _GPIO_BOARD_H
#define _GPIO_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif


/* Example/Board Header files */
#include <ti/board/board.h>

/**********************************************************************
 ************************** Macros ************************************
 **********************************************************************/


#define WKUP_GPIO                       (0U)
#define MAIN_GPIO                       (1U)
#define MAX_GPIOS_USED_IN_APP           (2U)

#define WKUP_GPIO_PIN_NUM (6U)
#if defined(SOC_J721E)
#define MAIN_GPIO_PIN_NUM (6U)
#elif defined(SOC_J7200)
#define MAIN_GPIO_PIN_NUM (0U)
#elif defined(SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#define MAIN_GPIO_PIN_NUM (11U)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _GPIO_LOG_H */
