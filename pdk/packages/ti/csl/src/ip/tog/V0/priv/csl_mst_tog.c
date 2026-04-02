/**
 * @file  csl_mst_tog.c
 *
 * @brief
 *  Implementation file for the VBUSM Master Timeout Gasket module CSL.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2019-2020, Texas Instruments, Inc.
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
 *    Neither the name of Texas Instruments Incorpomst_toged nor the names of
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

#include <ti/csl/csl_tog.h>
#include <ti/csl/csl_types.h>

/* Magic value used to force a timeout */
#define CSL_MST_TOG_FORCE_KEY       ((uint32_t) 0x95U)

int32_t CSL_mstTogSetTimeoutVal( CSL_MstTog_Regs *pRegs, CSL_MstTogVal timeoutVal )
{
    int32_t retVal = CSL_EFAIL;
    uint32_t regVal;

    if (pRegs == NULL_PTR)
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        regVal = pRegs->CONTROL;
        /* Timeout value can only be changed when enable==0 */
        if( CSL_FEXT( regVal, MST_TOG_EN ) == 0U )
        {
            CSL_FINS( regVal, MST_TOG_VAL, timeoutVal );
            pRegs->CONTROL = regVal;
            retVal = CSL_PASS;
        }
    }
    return retVal;
}

int32_t CSL_mstTogStart( CSL_MstTog_Regs *pRegs )
{
    int32_t  retVal = CSL_EBADARGS;

    if (pRegs != NULL_PTR)
    {
        CSL_REG32_FINS( &pRegs->CONTROL, MST_TOG_EN, (uint32_t)1U );
        retVal = CSL_PASS;
    }
    return retVal;
}

int32_t CSL_mstTogStop( CSL_MstTog_Regs *pRegs )
{
    int32_t  retVal = CSL_EBADARGS;

    if (pRegs != NULL_PTR)
    {
        CSL_REG32_FINS( &pRegs->CONTROL, MST_TOG_EN, (uint32_t)0U );
        retVal = CSL_PASS;
    }
    return retVal;

}

int32_t CSL_mstTogForceTimeout( CSL_MstTog_Regs *pRegs )
{
    int32_t retVal = CSL_EFAIL;
    uint32_t regVal;

    if (pRegs == NULL_PTR)
    {
        retVal = CSL_EBADARGS;
    }
    else
    {    
        regVal = pRegs->CONTROL;
        /* Forcing a timeout is only applicable when enable==1 */
        if( CSL_FEXT( regVal, MST_TOG_EN ) == 1U )
        {
            CSL_FINS( regVal, MST_TOG_FORCE, CSL_MST_TOG_FORCE_KEY );
            pRegs->CONTROL = regVal;
            retVal = CSL_PASS;
        }
    }
    return retVal;
}

int32_t CSL_mstTogReset( CSL_MstTog_Regs *pRegs )
{
    uint32_t regVal;
    int32_t  retVal = CSL_EBADARGS;

    if (pRegs != NULL_PTR)
    {
        regVal = pRegs->CONTROL;
        CSL_FINS( regVal, MST_TOG_FORCE, (uint32_t)0U );
        CSL_FINS( regVal, MST_TOG_EN,    (uint32_t)0U );
        pRegs->CONTROL = regVal;
        retVal = CSL_PASS;

    }
    return retVal;
}
