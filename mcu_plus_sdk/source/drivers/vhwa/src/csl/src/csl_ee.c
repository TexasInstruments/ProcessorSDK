/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file csl_msc.c
 *
 *  \brief File containing the VPAC MSC CSL init, deinit and other common
 *  functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <src/csl/include/csl_ee.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t CSL_eeSetFrameConfig(CSL_flexeeRegs *eeRegs,
    const CslEe_FrameConfig *cfg)
{
    int32_t             status = CSL_PASS;
    volatile uint32_t   regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Check for NULL pointer */
    if ((NULL == eeRegs) || (NULL == cfg))
    {
        status = CSL_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&eeRegs->EE_CFG_0);
        CSL_FINS(regVal, FLEXEE_EE_CFG_0_WIDTH, cfg->width);
        CSL_FINS(regVal, FLEXEE_EE_CFG_0_HEIGHT, cfg->height);
        CSL_REG32_WR(&eeRegs->EE_CFG_0, regVal);
    }

    return (status);
}

/**
 *  \brief Sets the entire MSC configuration to the MSC registers.
 *
 *  \param mscRegs          MSC Base Address
 *  \param config           Pointer to CslMsc_Config structure
 *                          containing the register configurations.
 *                          This parameter should be non-NULL.
 *  \param arg              Not used, should be NULL
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_eeSetConfig(CSL_flexeeRegs *eeRegs, const Fcp_EeConfig *cfg)
{
    int32_t             status = CSL_PASS;
    uint32_t            cnt;
    volatile uint32_t   regVal;
    volatile uint32_t  *regAddr;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Check for NULL pointer */
    if ((NULL == eeRegs) || (NULL == cfg))
    {
        status = CSL_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&eeRegs->EE_CFG_1);
        regVal &= ~(CSL_FLEXEE_EE_CFG_1_CLSE8_MUX_SEL_MASK |
            CSL_FLEXEE_EE_CFG_1_LLSE8_MUX_SEL_MASK         |
            CSL_FLEXEE_EE_CFG_1_CLSE12_MUX_SEL_MASK        |
            CSL_FLEXEE_EE_CFG_1_LLSE12_MUX_SEL_MASK);
        if ((uint32_t)UFALSE == cfg->bypassY12)
        {
            CSL_FINS(regVal, FLEXEE_EE_CFG_1_LLSE12_MUX_SEL, (uint32_t)1u);
        }
        if ((uint32_t)UFALSE == cfg->bypassC12)
        {
            CSL_FINS(regVal, FLEXEE_EE_CFG_1_CLSE12_MUX_SEL, (uint32_t)1u);
        }
        if ((uint32_t)UFALSE == cfg->bypassY8)
        {
            CSL_FINS(regVal, FLEXEE_EE_CFG_1_LLSE8_MUX_SEL, (uint32_t)1u);
        }
        if ((uint32_t)UFALSE == cfg->bypassC8)
        {
            CSL_FINS(regVal, FLEXEE_EE_CFG_1_CLSE8_MUX_SEL, (uint32_t)1u);
        }

        CSL_FINS(regVal, FLEXEE_EE_CFG_1_SHIFTLEFT_NUM, cfg->leftShift);
        CSL_FINS(regVal, FLEXEE_EE_CFG_1_SHIFTRIGHT_NUM, cfg->rightShift);

        regVal &= ~(CSL_FLEXEE_EE_CFG_1_YUV8_CL_ALIGN_MASK |
            CSL_FLEXEE_EE_CFG_1_YUV12_CL_ALIGN_MASK);
        if ((uint32_t)UTRUE == cfg->alignY12withChroma)
        {
            CSL_FINS(regVal, FLEXEE_EE_CFG_1_YUV12_CL_ALIGN, (uint32_t)1u);
        }
        if ((uint32_t)UTRUE == cfg->alignY8withChroma)
        {
            CSL_FINS(regVal, FLEXEE_EE_CFG_1_YUV8_CL_ALIGN, (uint32_t)1u);
        }

        CSL_FINS(regVal, FLEXEE_EE_CFG_1_EE_FE_MUX_SEL, cfg->eeForY12OrY8);

        CSL_REG32_WR(&eeRegs->EE_CFG_1, regVal);

        CSL_REG32_FINS(&eeRegs->YEE_SHIFT, FLEXEE_YEE_SHIFT_YEE_SHIFT,
            cfg->yeeShift);

        CSL_REG32_FINS(&eeRegs->YEE_COEF_R0_C0, FLEXEE_YEE_COEF_R0_C0_YEE_COEF_R0_C0,
            cfg->coeff[0]);
        CSL_REG32_FINS(&eeRegs->YEE_COEF_R0_C1, FLEXEE_YEE_COEF_R0_C1_YEE_COEF_R0_C1,
            cfg->coeff[1]);
        CSL_REG32_FINS(&eeRegs->YEE_COEF_R0_C2, FLEXEE_YEE_COEF_R0_C2_YEE_COEF_R0_C2,
            cfg->coeff[2]);

        CSL_REG32_FINS(&eeRegs->YEE_COEF_R1_C0, FLEXEE_YEE_COEF_R1_C0_YEE_COEF_R1_C0,
            cfg->coeff[3]);
        CSL_REG32_FINS(&eeRegs->YEE_COEF_R1_C1, FLEXEE_YEE_COEF_R1_C1_YEE_COEF_R1_C1,
            cfg->coeff[4]);
        CSL_REG32_FINS(&eeRegs->YEE_COEF_R1_C2, FLEXEE_YEE_COEF_R1_C2_YEE_COEF_R1_C2,
            cfg->coeff[5]);

        CSL_REG32_FINS(&eeRegs->YEE_COEF_R2_C0, FLEXEE_YEE_COEF_R2_C0_YEE_COEF_R2_C0,
            cfg->coeff[6]);
        CSL_REG32_FINS(&eeRegs->YEE_COEF_R2_C1, FLEXEE_YEE_COEF_R2_C1_YEE_COEF_R2_C1,
            cfg->coeff[7]);
        CSL_REG32_FINS(&eeRegs->YEE_COEF_R2_C2, FLEXEE_YEE_COEF_R2_C2_YEE_COEF_R2_C2,
            cfg->coeff[8]);

        CSL_REG32_FINS(&eeRegs->YEE_E_THR, FLEXEE_YEE_E_THR_YEE_E_THR,
            cfg->yeeEThr);
        CSL_REG32_FINS(&eeRegs->YEE_MERGESEL,
            FLEXEE_YEE_MERGESEL_YEE_MERGESEL, cfg->yeeMergeSel);
        CSL_REG32_FINS(&eeRegs->YES_E_HAL,
            FLEXEE_YES_E_HAL_YES_E_HAL, cfg->haloReductionOn);
        CSL_REG32_FINS(&eeRegs->YES_G_GAIN,
            FLEXEE_YES_G_GAIN_YES_G_GAIN, cfg->yesGGain);
        CSL_REG32_FINS(&eeRegs->YES_E_GAIN,
            FLEXEE_YES_E_GAIN_YES_E_GAIN, cfg->yesEGain);
        CSL_REG32_FINS(&eeRegs->YES_E_THR1,
            FLEXEE_YES_E_THR1_YES_E_THR1, cfg->yesEThr1);
        CSL_REG32_FINS(&eeRegs->YES_E_THR2,
            FLEXEE_YES_E_THR2_YES_E_THR2, cfg->yesEThr2);
        CSL_REG32_FINS(&eeRegs->YES_G_OFT,
            FLEXEE_YES_G_OFT_YES_G_OFT, cfg->yesGOfset);
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        if (NULL != cfg->lut)
        /* LDRA_JUSTIFY_END */
        {
            uint32_t *inAddr = (uint32_t *)cfg->lut;
            regAddr = &eeRegs->EELUT_RAM[0];
            uint32_t regval;
            for (cnt = 0; cnt < (FCP_EE_LUT_SIZE/2u); cnt ++)
            {
                regval = (inAddr[cnt] & CSL_FLEXEE_EELUT_RAM_EELUT_ENTRY_LO_MASK);
                regval |= (inAddr[cnt] & CSL_FLEXEE_EELUT_RAM_EELUT_ENTRY_HI_MASK);
                CSL_REG32_WR(&regAddr[cnt], regval);
            }
        }

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        if (UTRUE == cfg->enable)
        /* LDRA_JUSTIFY_END */
        {
            CSL_REG32_FINS(&eeRegs->EE_ENABLE,
                FLEXEE_EE_ENABLE_YEE_ENABLE, 1u);
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        else
        {
            CSL_REG32_FINS(&eeRegs->EE_ENABLE,
                FLEXEE_EE_ENABLE_YEE_ENABLE, 0u);
        }
        /* LDRA_JUSTIFY_END */
    }

    return (status);
}

void CSL_eeStart(CSL_flexeeRegs *eeRegs, uint32_t eeMux)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if (NULL != eeRegs)
    /* LDRA_JUSTIFY_END */
    {
        CSL_REG32_WR(&eeRegs->EE_CFG_1, eeMux);
        CSL_REG32_FINS(&eeRegs->EE_ENABLE,
            FLEXEE_EE_ENABLE_YEE_ENABLE, 1u);
    }
}

void CSL_eeStop(CSL_flexeeRegs *eeRegs, uint32_t eeMux)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if (NULL != eeRegs)
    /* LDRA_JUSTIFY_END */
    {
        CSL_REG32_WR(&eeRegs->EE_CFG_1, eeMux);
        CSL_REG32_FINS(&eeRegs->EE_ENABLE,
            FLEXEE_EE_ENABLE_YEE_ENABLE, 0u);
    }
}

uint32_t CSL_eeGetMuxValue(const CSL_flexeeRegs *eeRegs)
{
    uint32_t regVal = 0u;
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if (NULL != eeRegs)
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&eeRegs->EE_CFG_1);
    }

    return (regVal);
}


