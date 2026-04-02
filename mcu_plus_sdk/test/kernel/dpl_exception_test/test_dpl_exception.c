/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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
 *  \file   test_dpl_exception.c
 *
 *  \brief  DPL driver test application main file.
 *
 */
  
/*===========================================================================*/
/*                             Includes                                      */
/*===========================================================================*/

#include <stdint.h>
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <stdarg.h>

/*===========================================================================*/
/*                             Macros & Typedefs                             */
/*===========================================================================*/

#define CSL_UART0_BASE                                            (0x2800000UL)

/* ==========================================================================*/
/*                            Global Variables                               */
/* ==========================================================================*/

static const char *const g_pcHex = "0123456789abcdef";

uint32_t gExceptionHookCount = 0U;

/* ==========================================================================*/
/*                         Structure Declarations                            */
/* ==========================================================================*/

/* None */

/* ==========================================================================*/
/*                          Internal Function Declarations                   */
/* ==========================================================================*/

/* Description  : Genearting the OSAL Exception*/
static void DPLTest_generateException(void);

void Serial_dataWrite(const char* pcBuf, uint32_t u32length);

/* ==========================================================================*/
/*                      Internal Function Definitions                        */
/* ==========================================================================*/

void Serial_putChar(uint8_t byteTx)
{
    volatile uint32_t delayVal = 1000000U;
    /* Write the byte to the Transmit Holding Register(or TX FIFO). */
    HW_WR_REG32(CSL_UART0_BASE, (uint32_t) byteTx);
    while (--delayVal > 0U)
    {}
}

void Serial_convertVal(uint32_t ulValue, uint32_t ulPos, uint32_t ulBase, uint32_t ulNeg, uint32_t ulCount, char cFill, char *pcBuf)
{
    uint32_t ulIdx;
    uint32_t count = ulCount;
    uint32_t neg = ulNeg;
    uint32_t pos = ulPos;

    for (ulIdx = 1u;
         (((ulIdx * ulBase) <= ulValue) &&
          (((ulIdx * ulBase) / ulBase) == ulIdx));
         (ulIdx = (ulIdx * ulBase)))
    {
        count--;
    }

    /* If the value is negative, reduce the count of padding
     * characters needed. */
    if (neg != 0U)
    {
        count--;
    }

    /* If the value is negative and the value is padded with
     * zeros, then place the minus sign before the padding. */
    if ((neg != 0U) && ((int8_t)cFill == (int8_t) '0'))
    {
        /* Place the minus sign in the output buffer. */
        pcBuf[pos] = (char) '-';
        pos++;

        /* The minus sign has been placed, so turn off the
         * negative flag. */
        neg = 0;
    }

    /* Provide additional padding at the beginning of the
     * string conversion if needed. */
    if ((count > 1u) && (count < 16u))
    {
        for (count--; count != 0U; count--)
        {
            pcBuf[pos] = cFill;
            pos++;
        }
    }

    /* If the value is negative, then place the minus sign
     * before the number. */
    if (neg != 0U)
    {
        /* Place the minus sign in the output buffer. */
        pcBuf[pos] = (char) '-';
        pos++;
    }

    /* Convert the value into a string. */
    for (; ulIdx != 0U; ulIdx /= ulBase)
    {
        pcBuf[pos] = g_pcHex[(ulValue / ulIdx) % ulBase];
        pos++;
    }

    /* Write the string. */
    Serial_dataWrite(pcBuf, pos);
}

void Serial_dataWrite(const char* pcBuf, uint32_t u32length)
{
    uint32_t uIdx;
    /* Send the characters */
    for (uIdx = 0; uIdx < u32length; uIdx++)
    {
        /* If the character to the Serial_ is \n, then add a \r before it so that
         * \n is translated to \n\r in the output. */
        if (pcBuf[uIdx] == (char) '\n')
        {
            Serial_putChar('\r');
            Serial_putChar('\n');
        }

        /* Send the character to the Serial output. */
        Serial_putChar(pcBuf[uIdx]);
    }
}

void Serial_printf(const char *pcString, ...)
{
    uint32_t ulIdx, ulValue, ulPos, ulCount, ulBase, ulNeg;
    char    pcBuf[16], cFill;
    va_list  vaArgP;
    const char *pStr = pcString;

    /* Start the varargs processing. */
    (void)va_start(vaArgP, pcString);

    /* Loop while there are more characters in the string. */
    while (*pStr != (char)0U)
    {
        /* Find the first non-% character, or the end of the string. */
        for (ulIdx = 0;
             (pStr[ulIdx] != (char) '%') &&
             (pStr[ulIdx] != (char) '\0');
             ulIdx++)
        {}

        /* Write this portion of the string. */
        (void)Serial_dataWrite(pStr, ulIdx);

        /* Skip the portion of the string that was written. */
        pStr = pStr + ulIdx;

        /* See if the next character is a %. */
        if (*pStr == (char) '%')
        {
            /* Skip the %. */
            pStr++;

            /* Set the digit count to zero, and the fill character to space
             * (i.e. to the defaults). */
            ulCount = 0;
            cFill   = (char) ' ';

            switch (*pStr)
            {
                case (char) 'x':
                {
                    /* Get the value from the varargs. */
                    ulValue = (uint32_t)va_arg(vaArgP, uint32_t);

                    /* Reset the buffer position. */
                    ulPos = 0;

                    /* Set the base to 16. */
                    ulBase = 16u;

                    /* Indicate that the value is positive so that a minus sign
                     * isn't inserted. */
                    ulNeg = 0;

                    /* Determine the number of digits in the string version of
                     * the value. */
                    (void)Serial_convertVal(ulValue, ulPos, ulBase, ulNeg, ulCount, cFill, pcBuf);

                    break;
                }

                /* Handle all other commands. */
                default:
                {
                    /* Indicate an error. */
                    (void)Serial_dataWrite("ERROR", 5u);

                    /* This command has been handled. */
                    break;
                }
            }
            pStr++;
        }
    }
}

void DPLTest_defaultExceptionHook(void)
{
    gExceptionHookCount++;
}

void DPLTest_returnHook(void)
{
    gExceptionHookCount++;
    DebugP_exceptionLog(" \n Exception Hook count is %x  \n", gExceptionHookCount, 0U);
    while (1U) {
    }
}

static void DPLTest_generateException()
{
    volatile uint32_t *myBadAddr;
#if defined (BUILD_MCU)
    myBadAddr = (uint32_t *)(0xFFFF0001);
#elif defined (BUILD_C7X)
    Exception_Hooks exceptionHooks;
    exceptionHooks.exceptionHook = &DPLTest_defaultExceptionHook;
    exceptionHooks.internalHook = &DPLTest_defaultExceptionHook;
    exceptionHooks.returnHook = &DPLTest_returnHook;
    Exception_registerHooks(&exceptionHooks);
#endif
    *myBadAddr = (0xDEADFADE);

}

/*
 *  ======== main ========
 */
int DPLTest_Exception(void)
{
    volatile uint32_t flag = 1U;

    while(1U == flag){}

    Drivers_open();
    DebugP_registerExceptionLogFxn(Serial_printf);
    DPLTest_generateException();

    return (0);
}
