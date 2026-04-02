/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2021-2022
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
 *  \file csl_cac.c
 *
 *  \brief File containing the VPAC CAC config functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <src/csl/include/csl_cac.h>

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

int32_t CSL_cacSetFrameConfig(CSL_cacRegs *cacRegs,
    const CslCac_FrameConfig *cfg)
{
    int32_t             status = CSL_PASS;
    volatile uint32_t   regVal;

    /* Check for NULL pointer */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == cacRegs) || (NULL == cfg))
    {
        status = CSL_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&cacRegs->FRAMESZ);
        CSL_FINS(regVal, CAC_FRAMESZ_WIDTH, (cfg->width - 1u));
        CSL_FINS(regVal, CAC_FRAMESZ_HEIGHT, (cfg->height - 1u));
        CSL_REG32_WR(&cacRegs->FRAMESZ, regVal);
    }

    return (status);
}

int32_t CSL_cacSetConfig(CSL_cacRegs *cacRegs, CSL_cac_lutRegs *cacLutRegs,
                            const Cac_Config *cfg)
{
    int32_t             status = CSL_PASS;
    uint32_t            cnt;
    volatile uint32_t   regVal;
    volatile uint32_t  *regAddr = NULL;
    volatile uint32_t   *lutAddr = NULL;

    /* Check for NULL pointer */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == cacRegs) || (NULL == cacLutRegs) || (NULL == cfg))
    {
        status = CSL_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&cacRegs->CTRL);
        CSL_FINS(regVal, CAC_CTRL_COLOR_EN, (uint32_t)cfg->colorEnable);
        CSL_REG32_WR(&cacRegs->CTRL, regVal);

        regVal = CSL_REG32_RD(&cacRegs->BLOCKSZ);
        CSL_FINS(regVal, CAC_BLOCKSZ_SIZE, (uint32_t)cfg->blkSize);
        CSL_REG32_WR(&cacRegs->BLOCKSZ, regVal);

        regVal = CSL_REG32_RD(&cacRegs->BLOCKCNT);
        CSL_FINS(regVal, CAC_BLOCKCNT_HCNT, (uint32_t)cfg->blkGridSize.hCnt);
        CSL_FINS(regVal, CAC_BLOCKCNT_VCNT, (uint32_t)cfg->blkGridSize.vCnt);
        CSL_REG32_WR(&cacRegs->BLOCKCNT, regVal);

        regAddr = &cacLutRegs->LUT[0];
        lutAddr = cfg->displacementLut;
        /* Update the LUT table */
        for (cnt = 0u; cnt < (CAC_LUT_SIZE/4U); cnt += 1U)
        {
            regVal = lutAddr[cnt];
            CSL_REG32_WR(regAddr, regVal);
            regAddr++;
        }
    }

    return (status);
}


