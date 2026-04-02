/*
 * Copyright (c) 2016-2020, Texas Instruments Incorporated
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
 *
 */

/**
 *
 * \file  oled_test.h
 *
 * \brief   This file contains structure, typedefs, functions and
 *          prototypes used for oled test
 *
 *****************************************************************************/

#ifndef _OLED_TEST_H_
#define _OLED_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(SOC_AM64X)
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#endif

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#if defined(SOC_K2G) || defined(SOC_AM64X)
#include "diag_common_cfg.h"
#endif

#include "board.h"
#include "board_cfg.h"
#include "oled_display.h"

#if defined(SOC_AM64X)
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include "board_internal.h"
#include "board_i2c_io_exp.h"

/* Platform test return codes */
#define TEST_PASS     (0)
#define TEST_FAIL     (-1)

#endif

#if !defined(SOC_AM64X)
/* GPIO pin value definitions */
#define GPIO_PIN_VAL_LOW     (0U)
#define GPIO_PIN_VAL_HIGH    (1U)

#define GPIO_INDEX           (0U)

/* Port and pin number mask for LCD GPIO.
   Bits 7-0: Pin number  and Bits 15-8: Port number */
#define LCD_BST_CONV_CTL_GPIO    0x002D
#define LCD_RESET                0x0134
#endif


#if defined(SOC_AM64X)
/* Platform test return type */
typedef	int16_t TEST_STATUS;
#endif

/**
 * \brief This function performs oled test
 *
 * \param void
 *
 * \return
 * \n      TEST_PASS  - Test Passed
 * \n      TEST_FAIL  - Test Failed
 *
 */
TEST_STATUS oledTest(void);

#endif // _OLED_TEST_H_

/* Nothing past this point */
