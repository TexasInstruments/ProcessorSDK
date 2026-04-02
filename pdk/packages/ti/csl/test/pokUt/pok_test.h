/* Copyright (c) 2020 Texas Instruments Incorporated
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
 */

 /**
 *  \file     pok_test.h
 *
 *  \brief    This file contains POK test code defines.
 *
 **/

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_pok.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <ti/osal/osal.h>
#include <ti/csl/csl_esm.h>

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif


#if !defined(CSL_POK_TEST_H)
#define CSL_POK_TEST_H

/*===========================================================================*/
/*                         Declarations                                      */
/*===========================================================================*/

/* Define the test interface */
typedef struct cslPOKTest_s
{
    int32_t (*testFunction)(void);      /* The code that runs the test */
    char                *name;          /* The test name */
    int32_t              testStatus;    /* Test Status */
} cslPOKTest_t;

/*===========================================================================*/
/*                         Macros                                            */
/*===========================================================================*/
#define CSL_APP_TEST_NOT_RUN        (-(int32_t) (2))
#define CSL_APP_TEST_FAILED         (-(int32_t) (1))
#define CSL_APP_TEST_PASS           ( (int32_t) (0))

#if defined(SOC_J721E)
#define CSL_TEST_POK_MMR_BASE           (CSL_WKUP_CTRL_MMR0_CFG0_BASE)
#else
/* Address needs to be defined */
#endif


/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/
static int32_t cslApp_initBoard(void);
static void cslApp_print(const char * str);
static void cslApp_printArg(uint32_t value);

/*===========================================================================*/
/*                         External function declarations                    */
/*===========================================================================*/
extern int32_t cslPOK_apiTest(void);
extern int32_t cslPOK_funcTest(void);
extern int32_t cslPOKInPor_funcTest(void);
/*===========================================================================*/
/*                   Local Function definitions                              */
/*===========================================================================*/

static void cslApp_print(const char * str)
{
    UART_printf(str);
}

static void cslApp_printArg(uint32_t value)
{
   UART_printf("0x%x \n", value);
}
#endif /* CSL_POK_TEST_H */
/* Nothing past this point */
