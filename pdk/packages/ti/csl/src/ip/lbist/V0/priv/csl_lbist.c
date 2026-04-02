/**
 * @file  csl_lbist.c
 *
 * @brief
 *  CSL-FL implementation file for the LBIST module.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2019, Texas Instruments, Inc.
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

#include <stddef.h>
#include <ti/csl/csl_lbist.h>
#include <ti/csl/csl_types.h>


/* MACRO DEFINES */
#define CSL_LBIST_STAT_MISR_MUX_CTL_COMPACT_MISR            (0x0U)

#define CSL_LBIST_STAT_OUT_MUX_CTL_CTRLMMR_PID              (0x0U)
#define CSL_LBIST_STAT_OUT_MUX_CTL_CTRL_ID                  (0x1U)
#define CSL_LBIST_STAT_OUT_MUX_CTL_MISR_VALUE_1             (0x2U)
#define CSL_LBIST_STAT_OUT_MUX_CTL_MISR_VALUE_2             (0x3U)

/* Local Function prototypes */
static void CSL_LBIST_setLoadDiv(CSL_lbistRegs *pLBISTRegs);

static void CSL_LBIST_clearLoadDiv(CSL_lbistRegs *pLBISTRegs);

static void CSL_LBIST_setDivideRatio(CSL_lbistRegs *pLBISTRegs, uint32_t divideRatio);

static void CSL_LBIST_setNumStuckAtPatterns(CSL_lbistRegs *pLBISTRegs, uint32_t stuckAtPatterns);

static void CSL_LBIST_setNumSetPatterns(CSL_lbistRegs *pLBISTRegs, uint32_t setPatterns);

static void CSL_LBIST_setNumResetPatterns(CSL_lbistRegs *pLBISTRegs, uint32_t resetPatterns);

static void CSL_LBIST_setNumChainTestPatterns(CSL_lbistRegs *pLBISTRegs, uint32_t chainTestPatterns);

static void CSL_LBIST_setSeed(CSL_lbistRegs *pLBISTRegs, uint64_t seed);

static void CSL_LBIST_setClockDelay(CSL_lbistRegs *pLBISTRegs, uint32_t clockDelay);

/**
 * Requirement: REQ_TAG(PDK-5940)
 * Design: did_csl_lbist_interfaces did_csl_lbist_check_result
 */
int32_t CSL_LBIST_getMISR(CSL_lbistRegs *pLBISTRegs, uint32_t *pMISRValue)
{
    uint32_t regVal;
    uint32_t muxVal;
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        /* Setting to 0 also selects compacted 32-bit version of full MISR */
        regVal  = ((uint32_t)CSL_LBIST_STAT_MISR_MUX_CTL_COMPACT_MISR);
        /* The MISR value is available at two locations :
           Choosing location CSL_LBIST_STAT_OUT_MUX_CTL_MISR_VALUE_1 */
        muxVal  = ((uint32_t)CSL_LBIST_STAT_OUT_MUX_CTL_MISR_VALUE_1);
        regVal |= (muxVal << CSL_LBIST_STAT_OUT_MUX_CTL_SHIFT);
        pLBISTRegs->LBIST_STAT = regVal;
        *pMISRValue  = pLBISTRegs->LBIST_MISR;
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5940)
 * Design: did_csl_lbist_interfaces did_csl_lbist_check_result
 */
int32_t CSL_LBIST_getExpectedMISR(const uint32_t *pLBISTSig, uint32_t *pEpectedMISRValue)
{
    int32_t status = CSL_PASS;

    if (pLBISTSig == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        *pEpectedMISRValue = *pLBISTSig;
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5944)
 * Design: did_csl_lbist_interfaces did_csl_lbist_configuration
 */
int32_t CSL_LBIST_programConfig(CSL_lbistRegs *pLBISTRegs, const CSL_LBIST_config_t * const pConfig )
{
    int32_t status = CSL_PASS;

    if ((pLBISTRegs == NULL) || (pConfig == NULL))
    {
        status = CSL_EBADARGS;
    }
    else
    {
        CSL_LBIST_setClockDelay( pLBISTRegs, pConfig->dc_def );

        CSL_LBIST_setDivideRatio( pLBISTRegs, pConfig->divide_ratio );

        CSL_LBIST_clearLoadDiv( pLBISTRegs );
        CSL_LBIST_setLoadDiv( pLBISTRegs );

        CSL_LBIST_setNumStuckAtPatterns  ( pLBISTRegs, pConfig->static_pc_def );
        CSL_LBIST_setNumSetPatterns      ( pLBISTRegs, pConfig->set_pc_def    );
        CSL_LBIST_setNumResetPatterns    ( pLBISTRegs, pConfig->reset_pc_def  );
        CSL_LBIST_setNumChainTestPatterns( pLBISTRegs, pConfig->scan_pc_def   );

        CSL_LBIST_setSeed( pLBISTRegs, pConfig->prpg_def );
    }

    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5936)
 * Design: did_csl_lbist_interfaces did_csl_lbist_preparation
 */
int32_t CSL_LBIST_enableIsolation(CSL_lbistRegs *pLBISTRegs)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        pLBISTRegs->LBIST_SPARE0 |= ((uint32_t)CSL_LBIST_SPARE0_LBIST_SELFTEST_EN_MASK);
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5941)
 * Design: did_csl_lbist_interfaces did_csl_lbist_restore
 */
int32_t CSL_LBIST_disableIsolation(CSL_lbistRegs *pLBISTRegs)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        pLBISTRegs->LBIST_SPARE0 &= (~((uint32_t)CSL_LBIST_SPARE0_LBIST_SELFTEST_EN_MASK));
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5936)
 * Design: did_csl_lbist_interfaces did_csl_lbist_preparation
 */
int32_t CSL_LBIST_reset(CSL_lbistRegs *pLBISTRegs)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        pLBISTRegs->LBIST_CTRL &= (~((uint32_t)CSL_LBIST_CTRL_BIST_RESET_MAX << CSL_LBIST_CTRL_BIST_RESET_SHIFT));
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5936)
 * Design: did_csl_lbist_interfaces did_csl_lbist_preparation
 */
int32_t CSL_LBIST_enableRunBISTMode(CSL_lbistRegs *pLBISTRegs)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        pLBISTRegs->LBIST_CTRL |= ((uint32_t)CSL_LBIST_CTRL_RUNBIST_MODE_MAX << CSL_LBIST_CTRL_RUNBIST_MODE_SHIFT);
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5941)
 * Design: did_csl_lbist_interfaces did_csl_lbist_restore
 */
int32_t CSL_LBIST_clearRunBISTMode(CSL_lbistRegs *pLBISTRegs)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        pLBISTRegs->LBIST_CTRL &= (~((uint32_t)CSL_LBIST_CTRL_RUNBIST_MODE_MAX << CSL_LBIST_CTRL_RUNBIST_MODE_SHIFT));
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5939)
 * Design: did_csl_lbist_interfaces did_csl_lbist_control
 */
int32_t CSL_LBIST_start(CSL_lbistRegs *pLBISTRegs)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        pLBISTRegs->LBIST_CTRL |= ((uint32_t)CSL_LBIST_CTRL_BIST_RESET_MAX << CSL_LBIST_CTRL_BIST_RESET_SHIFT);  /* Deassert reset */
        pLBISTRegs->LBIST_CTRL |= ((uint32_t)CSL_LBIST_CTRL_BIST_RUN_MAX << CSL_LBIST_CTRL_BIST_RUN_SHIFT);
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5939)
 * Design: did_csl_lbist_interfaces did_csl_lbist_control
 */
int32_t CSL_LBIST_stop(CSL_lbistRegs *pLBISTRegs)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        pLBISTRegs->LBIST_CTRL &= (~((uint32_t)CSL_LBIST_CTRL_BIST_RUN_MAX << CSL_LBIST_CTRL_BIST_RUN_SHIFT));
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5940)
 * Design: did_csl_lbist_interfaces did_csl_lbist_check_result
 */
int32_t CSL_LBIST_isRunning (const CSL_lbistRegs *pLBISTRegs, Bool *pIsRunning)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        *pIsRunning = ((pLBISTRegs->LBIST_STAT & CSL_LBIST_STAT_BIST_RUNNING_MASK) != ((uint32_t)0u))
                          ? CSL_TRUE : CSL_FALSE;
    }
    return status;
}

/**
 * Requirement: REQ_TAG(PDK-5940)
 * Design: did_csl_lbist_interfaces did_csl_lbist_check_result
 */
int32_t CSL_LBIST_isDone (const CSL_lbistRegs *pLBISTRegs, Bool *pIsDone)
{
    int32_t status = CSL_PASS;

    if (pLBISTRegs == NULL)
    {
        status = CSL_EBADARGS;
    }
    else
    {
        *pIsDone = ((pLBISTRegs->LBIST_STAT & CSL_LBIST_STAT_BIST_DONE_MASK) != ((uint32_t)0u))
                       ? CSL_TRUE : CSL_FALSE;
    }
    return status;
}

static void CSL_LBIST_setLoadDiv(CSL_lbistRegs *pLBISTRegs)
{
    pLBISTRegs->LBIST_CTRL |= ((uint32_t)CSL_LBIST_CTRL_LOAD_DIV_MASK);
}

static void CSL_LBIST_clearLoadDiv(CSL_lbistRegs *pLBISTRegs)
{
    pLBISTRegs->LBIST_CTRL &= (~((uint32_t)CSL_LBIST_CTRL_LOAD_DIV_MASK));
}

static void CSL_LBIST_setDivideRatio(CSL_lbistRegs *pLBISTRegs,
                                     uint32_t divideRatio)
{
    pLBISTRegs->LBIST_CTRL &= (~(uint32_t)CSL_LBIST_CTRL_DIVIDE_RATIO_MASK);
    pLBISTRegs->LBIST_CTRL |= (divideRatio & CSL_LBIST_CTRL_DIVIDE_RATIO_MASK);
}

static void CSL_LBIST_setNumStuckAtPatterns(CSL_lbistRegs *pLBISTRegs,
                                            uint32_t stuckAtPatterns)
{
    pLBISTRegs->LBIST_PATCOUNT &= (~((uint32_t)CSL_LBIST_PATCOUNT_STATIC_PC_DEF_MASK));
    pLBISTRegs->LBIST_PATCOUNT |= ((stuckAtPatterns & CSL_LBIST_PATCOUNT_STATIC_PC_DEF_MAX)
                                << CSL_LBIST_PATCOUNT_STATIC_PC_DEF_SHIFT);
}

static void CSL_LBIST_setNumSetPatterns(CSL_lbistRegs *pLBISTRegs,
                                        uint32_t setPatterns)
{
    pLBISTRegs->LBIST_PATCOUNT &= (~((uint32_t)CSL_LBIST_PATCOUNT_SET_PC_DEF_MASK));
    pLBISTRegs->LBIST_PATCOUNT |= ((setPatterns & CSL_LBIST_PATCOUNT_RESET_PC_DEF_MAX)
                                << CSL_LBIST_PATCOUNT_SET_PC_DEF_SHIFT);
}

static void CSL_LBIST_setNumResetPatterns(CSL_lbistRegs *pLBISTRegs,
                                          uint32_t resetPatterns)
{
    pLBISTRegs->LBIST_PATCOUNT &= (~((uint32_t)CSL_LBIST_PATCOUNT_RESET_PC_DEF_MASK));
    pLBISTRegs->LBIST_PATCOUNT |= ((resetPatterns & CSL_LBIST_PATCOUNT_RESET_PC_DEF_MAX)
                                << CSL_LBIST_PATCOUNT_RESET_PC_DEF_SHIFT);
}

static void CSL_LBIST_setNumChainTestPatterns(CSL_lbistRegs *pLBISTRegs,
                                              uint32_t chainTestPatterns)
{
    pLBISTRegs->LBIST_PATCOUNT &= (~((uint32_t)CSL_LBIST_PATCOUNT_SCAN_PC_DEF_MASK));
    pLBISTRegs->LBIST_PATCOUNT |= ((chainTestPatterns & CSL_LBIST_PATCOUNT_SCAN_PC_DEF_MAX)
                                << CSL_LBIST_PATCOUNT_SCAN_PC_DEF_SHIFT);
}

static void CSL_LBIST_setSeed(CSL_lbistRegs *pLBISTRegs, uint64_t seed)
{
    pLBISTRegs->LBIST_SEED0 = (uint32_t)(seed & CSL_LBIST_SEED0_PRPG_DEF_MASK);
    pLBISTRegs->LBIST_SEED1 = (uint32_t)((seed >> 32) & CSL_LBIST_SEED1_PRPG_DEF_MASK);
}

static void CSL_LBIST_setClockDelay(CSL_lbistRegs *pLBISTRegs,
                                    uint32_t clockDelay)
{
    pLBISTRegs->LBIST_CTRL &= ((uint32_t)CSL_LBIST_CTRL_DC_DEF_MASK);
    pLBISTRegs->LBIST_CTRL |= ((clockDelay & CSL_LBIST_CTRL_DC_DEF_MAX)
                            << CSL_LBIST_CTRL_DC_DEF_SHIFT);
}

/* Nothing past this point */
