/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2015-2017
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
 *  \file     app_utils_k3.c
 *
 *  \brief    This file contains common utility functions used by the apps
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

#include "app_utils.h"
#include <ti/osal/osal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void AppUtils_defaultInit(void)
{
    Board_initCfg   boardCfg;
    boardCfg = //BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    return;
}

void AppUtils_printf(const char *pStr, ...)
{
    static char tempBuf[400];
    va_list     vaArgP;

    /* Start the varargs processing. */
    va_start(vaArgP, pStr);

    vsnprintf(&tempBuf[0U], sizeof (tempBuf), pStr, vaArgP);
    UART_printf("%s", &tempBuf[0U]);
    App_printf("%s", &tempBuf[0U]);

    /* End the varargs processing. */
    va_end(vaArgP);

    return;
}

int32_t AppUtils_getCharTimeout(char *ch, uint32_t msec)
{
    int32_t  retVal = 0;

    /* Timeout not supported by stdio scanf */
    scanf("%c", ch);

    return (retVal);
}

int32_t AppUtils_getNum(void)
{
    int32_t num;

    scanf("%d", &num);

    return (num);
}

void AppUtils_delay(uint32_t msec)
{
    Osal_delay(msec);
    return;
}

uint32_t AppUtils_getCurTimeInMsec(void)
{
    uint64_t curTimeUsec = TimerP_getTimeInUsecs();
    uint32_t curTimeMsec = curTimeUsec / 1000U;

    return (curTimeMsec);
}

/**
 * \brief   This function gets the silicon package type of the SoC.
 *
 * \param   None
 *
 * \return  packageType   Refer enum #platformSiliconPackageType_t for details.
 */
uint32_t AppUtils_GetSiliconPackageType(void)
{
    uint32_t packageType;
    packageType = PLATFORM_SILICON_PACKAGE_TYPE_UNKNOWN;
    return packageType;
}
