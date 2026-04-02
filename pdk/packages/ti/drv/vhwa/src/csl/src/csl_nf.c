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
 *  \file csl_nf.c
 *
 *  \brief  NF CSL file, contains implementation of API, configuring NF module.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <src/csl/include/csl_nf.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */



/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   Function make Bi-lateral filter LUT register.
 *
 * \param   l0,l1,l2,l3        4 8 bit LUT values.
 *
 * \return  32bit LUT register.
 *
 **/
static uint32_t NfMakeLut1Reg(uint32_t l0, uint32_t l1, uint32_t l2,
                               uint32_t l3);

/**
 * \brief   Function make Generic filter coeffs register.
 *
 * \param   l0,l1        2 9 bit signed coeff values.
 *
 * \return  32bit LUT register.
 *
 **/
static uint32_t NfMakeLut2Reg(uint32_t l0, uint32_t l1);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  \brief Sets the entire NF configuration to the NR core registers.
 *         Except the filter tables
 *
 *  \param nfRegs     Pointer to structure containing DOF Base Address
 *  \param config     Pointer to #Nf_Config structure containing the register
 *                    configurations.
 *  \return           None
 */
void CSL_nfSetConfig(CSL_vpac_nfRegs *nfRegs, const Nf_Config *cfg)
{
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL != nfRegs) && (NULL != cfg))
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&nfRegs->CONTROL);
        CSL_FINS(regVal, VPAC_NF_CONTROL_ENABLE_GENERIC_FILTERING,
                                                        cfg->filterMode);
        CSL_FINS(regVal, VPAC_NF_CONTROL_ADAPTIVE_MODE, cfg->tableMode);

        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
        No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. However, due to the stated rationale, this is not tested.
        <justification end> */
        if (1U == cfg->skipMode)
        {
            CSL_FINS(regVal, VPAC_NF_CONTROL_SKIP_MODE, 1U);
            CSL_FINS(regVal, VPAC_NF_CONTROL_SKIP_ODD_MODE, 0U);
        }
        else if (2U == cfg->skipMode)
        {
            CSL_FINS(regVal, VPAC_NF_CONTROL_SKIP_MODE, 1U);
            CSL_FINS(regVal, VPAC_NF_CONTROL_SKIP_ODD_MODE, 1U);
        }
        else
        /* LDRA_JUSTIFY_END */
        {
            CSL_FINS(regVal, VPAC_NF_CONTROL_SKIP_MODE, 0U);
            CSL_FINS(regVal, VPAC_NF_CONTROL_SKIP_ODD_MODE, 0U);
        }

        CSL_FINS(regVal, VPAC_NF_CONTROL_INTERLEAVE_MODE, cfg->interleaveMode);
        CSL_FINS(regVal, VPAC_NF_CONTROL_OUTPUT_SHIFT,
            (uint32_t)cfg->outputShift);
        CSL_FINS(regVal, VPAC_NF_CONTROL_OUTPUT_OFFSET, cfg->outputOffset);

        CSL_FINS(regVal, VPAC_NF_CONTROL_NUM_SUB_TABLES, cfg->numSubTables);
        CSL_FINS(regVal, VPAC_NF_CONTROL_SUB_TABLE_SELECT, cfg->subTableIdx);

        CSL_REG32_WR(&nfRegs->CONTROL, regVal);

        regVal = CSL_REG32_RD(&nfRegs->CENTER_WEIGHT);
        CSL_FINS(regVal, VPAC_NF_CENTER_WEIGHT_CENTRAL_PIXEL_WEIGHT_W00,
            (uint32_t)cfg->centralPixelWeight);
        CSL_REG32_WR(&nfRegs->CENTER_WEIGHT, regVal);
    }

}

/**
 *  \brief Sets the tables for NF (Both Bilateral and generic filter)
 *
 *  \param nfRegs     Pointer to structure containing DOF Base Address
 *  \param config     Pointer to #Nf_Config structure containing the filter
 *                    configuration.
 *  \return           Returns 0 on success else returns error value
 */
int32_t CSL_nfSetWgtTableConfig(CSL_vpac_nfRegs *nfRegs,
                                        const Nf_WgtTableConfig *cfg)
{
    int32_t  status = CSL_PASS;
    uint32_t regVal;
    uint32_t lutCnt, wCnt;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL != nfRegs) && (NULL != cfg))
    /* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (NF_FILTER_MODE_BILATERAL == cfg->filterMode)
        /* LDRA_JUSTIFY_END */
        {
            wCnt = 0U;
            /* Configure the Weight table registers for Bi-lateral filter*/
            for(lutCnt = 0U; lutCnt < NF_BL_LUT_SIZE; lutCnt += 4U)
            {
                regVal = NfMakeLut1Reg(cfg->blFilterLut[lutCnt],
                  cfg->blFilterLut[lutCnt + 1U],
                  cfg->blFilterLut[lutCnt + 2U],
                  cfg->blFilterLut[lutCnt + 3U]);

                CSL_REG32_WR(&nfRegs->WEIGHT_LUT[wCnt], regVal);
                wCnt ++;
            }
        }
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        else if(NF_FILTER_MODE_GENERIC_2D_FILTER == cfg->filterMode)
        /* LDRA_JUSTIFY_END */
        {
            wCnt = 0U;
            /* Configure the Weight table registers for Bi-Generic 2D filter*/
            for(lutCnt = 0U; lutCnt < NF_GEN_LUT_SIZE; lutCnt += 2U)
            {
                regVal = NfMakeLut2Reg((uint32_t)cfg->genFilterCoeffs[lutCnt],
                  (uint32_t)cfg->genFilterCoeffs[lutCnt + 1U]);

                CSL_REG32_WR(&nfRegs->WEIGHT_LUT[wCnt], regVal);
                wCnt ++;
            }
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        else
        {
            status = CSL_EINVALID_PARAMS;
        }
        /* LDRA_JUSTIFY_END */
    }

    return (status);
}

void CSL_nfUpdateConfig(CSL_vpac_nfRegs *nfRegs, const Nf_Config *cfg)
{
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If control reaches here, the code base is expected to prevent undefined behaviour by avoiding dereferencing a NULL pointer.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL != nfRegs) && (NULL != cfg))
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&nfRegs->CONTROL);

        CSL_FINS(regVal, VPAC_NF_CONTROL_INTERLEAVE_MODE, cfg->interleaveMode);

        CSL_REG32_WR(&nfRegs->CONTROL, regVal);
    }

}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */

static uint32_t NfMakeLut1Reg(uint32_t l0, uint32_t l1, uint32_t l2,
                               uint32_t l3)
{
    uint32_t regVal = 0U;

    regVal |= (l0 << CSL_VPAC_NF_WEIGHT_LUT_W_0_SHIFT) &
              CSL_VPAC_NF_WEIGHT_LUT_W_0_MASK;
    regVal |= (l1 << CSL_VPAC_NF_WEIGHT_LUT_W_1_SHIFT) &
              CSL_VPAC_NF_WEIGHT_LUT_W_1_MASK;
    regVal |= (l2 << CSL_VPAC_NF_WEIGHT_LUT_W_2_SHIFT) &
              CSL_VPAC_NF_WEIGHT_LUT_W_2_MASK;
    regVal |= (l3 << CSL_VPAC_NF_WEIGHT_LUT_W_3_SHIFT) &
              CSL_VPAC_NF_WEIGHT_LUT_W_3_MASK;

    return (regVal);
}

static uint32_t NfMakeLut2Reg(uint32_t l0, uint32_t l1)
{
    uint32_t regVal = 0U;

    regVal |= l0 &
            (CSL_VPAC_NF_WEIGHT_LUT_W_0_MASK | CSL_VPAC_NF_WEIGHT_LUT_W_1_MASK);
    regVal |= (l1 << CSL_VPAC_NF_WEIGHT_LUT_W_2_SHIFT) &
            (CSL_VPAC_NF_WEIGHT_LUT_W_2_MASK | CSL_VPAC_NF_WEIGHT_LUT_W_3_MASK);

    return (regVal);
}
