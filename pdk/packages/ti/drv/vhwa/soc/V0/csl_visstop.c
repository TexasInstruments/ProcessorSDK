/**
 *   Copyright (c) Texas Instruments Incorporated 2019
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
 *
 */

/**
 *  \file csl_vissTop.c
 *
 *  \brief File implementing common APIs, functionality of VISS_Top
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/csl/csl_types.h>
#include <src/csl/include/csl_visstop.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  Function returns maximum line width supported in VISS
 */
uint32_t CSL_vissTopGetMaxWidth(const CSL_viss_topRegs *vissTopRegs)
{
    volatile uint32_t regVal;
    uint32_t maxWidth = 0U;

    if (NULL != vissTopRegs)
    {
        regVal = CSL_REG32_RD(&vissTopRegs->VISS_LINEMEM_SIZE);
        maxWidth = CSL_FEXT(regVal, VISS_TOP_VISS_LINEMEM_SIZE_LINEMEM_SZ);
    }

    return (maxWidth);
}

/**
 *  Function sets GLBCE delay configuration
 */
void CSL_vissTopGlbceSetDelayConfig(CSL_viss_topRegs *vissTopRegs,
    const CslGlbce_DelayConfig *dlyCfg)
{
    volatile uint32_t regVal;

    if ((NULL != vissTopRegs) && (NULL != dlyCfg))
    {
        regVal = CSL_REG32_RD(&vissTopRegs->GLBCE_VPSYNCDLY);
        CSL_FINS(regVal, VISS_TOP_GLBCE_VPSYNCDLY_H_DLY, dlyCfg->horzDelay);
        CSL_FINS(regVal, VISS_TOP_GLBCE_VPSYNCDLY_V_DLY, dlyCfg->vertDelay);
        CSL_REG32_WR(&vissTopRegs->GLBCE_VPSYNCDLY, regVal);
    }
}

/**
 *  Function to enable/disable GLBCE module
 */
void CSL_vissTopGlbceEnable(CSL_viss_topRegs *vissTopRegs, uint32_t enable)
{
    if (NULL != vissTopRegs)
    {
        if ((uint32_t)UTRUE == enable)
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_GLBCE_EN, 1U);
        }
        else
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_GLBCE_EN, 0U);
        }
    }
}

/**
 *  Function to enable/disable NSF4 module
 */
void CSL_vissTopNsf4Enable(CSL_viss_topRegs *vissTopRegs, uint32_t enable)
{
    if (NULL != vissTopRegs)
    {
        if ((uint32_t)UTRUE == enable)
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_NSF4V_EN, 1U);
        }
        else
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_NSF4V_EN, 0U);
        }
    }
}

/**
 *  Function to enable/disable GLBCE clock to free running
 */
void CSL_vissTopGlbceFreeRunning(CSL_viss_topRegs *vissTopRegs,
    uint32_t enable)
{
    if (NULL != vissTopRegs)
    {
        if ((uint32_t)UTRUE == enable)
        {
            CSL_REG32_FINS(&vissTopRegs->GLBCECONFIG,
                VISS_TOP_GLBCECONFIG_GLBCE_PCLKFREE, 1U);
        }
        else
        {
            CSL_REG32_FINS(&vissTopRegs->GLBCECONFIG,
                VISS_TOP_GLBCECONFIG_GLBCE_PCLKFREE, 0U);
        }
    }
}

uint32_t CSL_vissTopIsGlbceFiltDone(const CSL_viss_topRegs *vissTopRegs)
{
    uint32_t status = UFALSE;
    volatile uint32_t regVal;

    if (NULL != vissTopRegs)
    {
        regVal = CSL_REG32_RD(&vissTopRegs->GLBCE_INT_STAT);
        if ((regVal & CSL_VISS_TOP_GLBCE_INT_STAT_FILT_DONE_MASK) ==
                CSL_VISS_TOP_GLBCE_INT_STAT_FILT_DONE_MASK)
        {
            status = (uint32_t)UTRUE;
        }
        else
        {
            status = (uint32_t)UFALSE;
        }
    }

    return (status);
}


