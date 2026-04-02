/**
 *   Copyright (c) Texas Instruments Incorporated 2021-2022
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
 */

/**
 *  \file csl_fcp.c
 *
 *  \brief VISS Flex Cp CSL file, setting up different modules of Flex CP
 *
 */


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <src/csl/include/csl_fcp.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief macro to check max frame size for FCP, extracted from the register */
#define FCP_MAX_INPUT_WIDTH                                                    \
    (CSL_FLEXCFA_CFG_0_WIDTH_MASK >> CSL_FLEXCFA_CFG_0_WIDTH_SHIFT)
#define FCP_MAX_INPUT_HEIGHT                                                   \
    (CSL_FLEXCFA_CFG_0_HEIGHT_MASK >> CSL_FLEXCFA_CFG_0_HEIGHT_SHIFT)

/** \brief Mask and shift value for selecting Blend Mode for the input core
 */
#define FCP_FCFA_GRAD_CFG_BLDMODECORE_MASK(core)                               \
    ((uint32_t)CSL_FLEXCFA_GRAD_CFG_BLENDMODECORE0_MASK << ((core) * 8U))
#define FCP_FCFA_GRAD_CFG_BLDMODECORE_SHIFT(core)                              \
    (CSL_FLEXCFA_GRAD_CFG_BLENDMODECORE0_SHIFT + ((core) * 8U))

/** \brief Mask and shift value for selecting core for the input color
 */
#define FCP_FCFA_GRAD_CFG_SELCORE_MASK(core)                                   \
    ((uint32_t)CSL_FLEXCFA_GRAD_CFG_BITMASKSELCORE0_MASK << ((core) * 8U))
#define FCP_FCFA_GRAD_CFG_SELCORE_SHIFT(core)                                  \
    (CSL_FLEXCFA_GRAD_CFG_BITMASKSELCORE0_SHIFT + ((core) * 8U))

/** \brief Register offset for Horizontal gradient for given set
 */
#define FCP_FCFA_SET_GRAD_HZ(set)                                              \
    (CSL_FLEXCFA_SET0_GRAD_HZ +                                                \
    (CSL_FLEXCFA_SET1_GRAD_HZ - CSL_FLEXCFA_SET0_GRAD_HZ) * (set))

/** \brief Register offset for Vertical gradient for given set
 */
#define FCP_FCFA_SET_GRAD_VT(set)                                              \
    (CSL_FLEXCFA_SET0_GRAD_VT +                                                \
    (CSL_FLEXCFA_SET1_GRAD_VT - CSL_FLEXCFA_SET0_GRAD_VT) * (set))

/** \brief Register offset for intensity register for given set
 */
#define FCP_FCFA_SET_INTENSITY(set, cnt)                                       \
    (CSL_FLEXCFA_SET0_INTENSITY0 +                                             \
    (CSL_FLEXCFA_SET1_INTENSITY0 - CSL_FLEXCFA_SET0_INTENSITY0) * (set) +      \
        (0x4U * (cnt)))

/** \brief Register offset for intensity register for given set
 */
#define FCP_FCFA_SET_THR(set, cnt)                                             \
    (CSL_FLEXCFA_SET0_THR0_1 +                                                 \
    (CSL_FLEXCFA_SET1_THR0_1 - CSL_FLEXCFA_SET0_THR0_1) * (set) +              \
        (0x4U * (cnt)))

/** \brief Mask and shift value for configuring contrast Lut
 */
#define FCP_CNTRST_LUT_LO_MASK                         (0xFFFU)
#define FCP_CNTRST_LUT_LO_SHIFT                        (0U)

/** \brief Mask and shift value for configuring contrast Lut
 */
#define FCP_CNTRST_LUT_HI_MASK                         (0xFFF0000U)
#define FCP_CNTRST_LUT_HI_SHIFT                        (16U)

/** \brief Maximum number of intensity registers for each set, used as
  *        loop counter
  */
#define FCP_CFA_MAX_INTENSITY_REG                      (2U)

/** \brief Maximum number of threshold registers for each set, used as
  *        loop counter
  */
#define FCP_CFA_MAX_THR_REG                            (4U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Local function to set CFA Intensity for the given set.
 *
 *  \param cfaRegs          Pointer to CFA register overlay
 *                          This parameter should be non-NULL.
 *  \param set              Id of the Intensity set, must be either 0 or 1
 *  \param cfg              pointer to CFA config, from which intensity
 *                          config is extracted and set.
 */
static void FcpSetCfaIntensity(CSL_flexcfaRegs *cfaRegs, uint32_t set,
    const Fcp_CfaConfig *cfg);

/**
 *  \brief Local function to set CFA threshold for the given set.
 *
 *  \param cfaRegs          Pointer to CFA register overlay
 *                          This parameter should be non-NULL.
 *  \param set              Id of the Threshold set, must be either 0 or 1
 *  \param cfg              pointer to CFA config, from which threshold
 *                          config is extracted and set.
 */
static void FcpSetCfaThreshold(CSL_flexcfaRegs *cfaRegs, uint32_t set,
    const Fcp_CfaConfig *cfg);

/**
 *  \brief Local function to set horizontal and vertical
 *         gradient for the given set.
 *
 *  \param cfaRegs          Pointer to CFA register overlay
 *                          This parameter should be non-NULL.
 *  \param set              Id of the Threshold set, must be either 0 or 1
 *  \param cfg              pointer to CFA config, from which gradient
 *                          config is extracted and set.
 */
static void FcpSetCfaGradHorzVert(CSL_flexcfaRegs *cfaRegs, uint32_t set,
    const Fcp_CfaConfig *cfg);

/**
 *  \brief Local function to set FIR
 *
 *  \param cfaRegs          Pointer to CFA register overlay
 *                          This parameter should be non-NULL.
 *  \param cfg              pointer to CFA config, from which FIR
 *                          config is extracted and set.
 */
static void FcpSetCfaFir(CSL_flexcfaRegs *cfaRegs, const Fcp_CfaConfig *cfg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Function to set frame size */
int32_t CSL_fcpSetFrameConfig(CSL_flexcfaRegs *cfaRegs,
    CSL_flexcc_cfgRegs *fcpRegs, const CslFcp_FrameConfig *cfg)
{
    int32_t           status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    cfaRegs, cfg and fcpRegs are pre-validated in Vhwa_memVissSetVissSizeAndPath()
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Null pointer check */
    if ((NULL == cfg) || (NULL == cfaRegs) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        FCP_MAX_INPUT_WIDTH and FCP_MAX_INPUT_HEIGHT are pre-validated in LseCheckCfg()
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if ((cfg->width > FCP_MAX_INPUT_WIDTH) ||
            (cfg->height > FCP_MAX_INPUT_HEIGHT))
        {
            status = FVID2_EINVALID_PARAMS;
        }
        /* LDRA_JUSTIFY_END */
    }
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The statements setting error status cannot be reached.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&cfaRegs->CFG_0);
        CSL_FINS(regVal, FLEXCFA_CFG_0_WIDTH, cfg->width);
        CSL_FINS(regVal, FLEXCFA_CFG_0_HEIGHT, cfg->height);
        CSL_REG32_WR(&cfaRegs->CFG_0, regVal);

        regVal = CSL_REG32_RD(&fcpRegs->CFG_0);
        CSL_FINS(regVal, FLEXCC_CFG_CFG_0_WIDTH, cfg->width);
        CSL_FINS(regVal, FLEXCC_CFG_CFG_0_HEIGHT, cfg->height);
        CSL_REG32_WR(&fcpRegs->CFG_0, regVal);
    }

    return (status);
}

/* Function to set CFA Configuration,
   Does a minimal error checking, upper layer or application should make
   sure that all parameters are valid */
int32_t CSL_fcpSetCfaConfig(CSL_flexcfaRegs *cfaRegs,
    const Fcp_CfaConfig *cfg)
{
    int32_t            status = FVID2_SOK;
    uint32_t           cnt;
    volatile uint32_t  regVal;
    volatile uint32_t *regAddr = NULL;

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Null pointer check */
    if ((NULL == cfg) || (NULL == cfaRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        /* Set CFA Coefficients */
        regAddr = &cfaRegs->CORE[0U].DIR[0U].PHASE[0U].ROW[0U].COL[0U].COEF;
        for (cnt = 0U; cnt < FCP_MAX_CFA_COEFF; cnt += 2U)
        {
            regVal = CSL_REG32_RD(regAddr);
            CSL_FINS(regVal,
                FLEXCFA_CORE_DIR_PHASE_ROW_COL_COEF_COEF_0,
				(uint32_t)cfg->coeff[cnt]);
            CSL_FINS(regVal, FLEXCFA_CORE_DIR_PHASE_ROW_COL_COEF_COEF_1,
                (uint32_t)cfg->coeff[cnt + 1U]);
            CSL_REG32_WR(regAddr, regVal);

            regAddr ++;
        }

        /* Select the core and core mode for each input color */
        regVal = CSL_REG32_RD(&cfaRegs->GRAD_CFG);
        for (cnt = 0U; cnt < FCP_MAX_COLOR_COMP; cnt ++)
        {
            regVal &= ~FCP_FCFA_GRAD_CFG_SELCORE_MASK(cnt);
            regVal &= ~FCP_FCFA_GRAD_CFG_BLDMODECORE_MASK(cnt);

            regVal |= ((uint32_t)cfg->coreSel[cnt] <<
                FCP_FCFA_GRAD_CFG_SELCORE_SHIFT(cnt)) &
                FCP_FCFA_GRAD_CFG_SELCORE_MASK(cnt);
            regVal |= ((uint32_t)cfg->coreBlendMode[cnt] <<
                FCP_FCFA_GRAD_CFG_BLDMODECORE_SHIFT(cnt)) &
                FCP_FCFA_GRAD_CFG_BLDMODECORE_MASK(cnt);
        }
        CSL_REG32_WR(&cfaRegs->GRAD_CFG, regVal);

        for (cnt = 0U; cnt < FCP_CFA_MAX_SET; cnt ++)
        {
            /* Set the Horizontal & vertical Gradient */
            FcpSetCfaGradHorzVert(cfaRegs, cnt, cfg);
            /* Set the Intensity */
            FcpSetCfaIntensity(cfaRegs, cnt, cfg);
            /* Set the CFA threshold */
            FcpSetCfaThreshold(cfaRegs, cnt, cfg);
        }

        regVal = CSL_REG32_RD(&cfaRegs->CFG_1);
        regVal &= ~(CSL_FLEXCFA_CFG_1_BYPASS_CORE0_MASK|
                    CSL_FLEXCFA_CFG_1_BYPASS_CORE1_MASK|
                    CSL_FLEXCFA_CFG_1_BYPASS_CORE2_MASK|
                    CSL_FLEXCFA_CFG_1_BYPASS_CORE3_MASK);
        for (cnt = 0U; cnt < FCP_MAX_COLOR_COMP; cnt ++)
        {
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications CAN reach this portion.
                        No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
            Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                    However, due to the stated rationale, this is not tested.
            <justification end> */
            if (UTRUE == cfg->bypass[cnt])
            {
                regVal |= ((uint32_t)1U << (CSL_FLEXCFA_CFG_1_BYPASS_CORE0_SHIFT
                                    + cnt));
            }
            /* LDRA_JUSTIFY_END */
        }
        CSL_FINS(regVal, FLEXCFA_CFG_1_EN16BITMODE,
                (uint32_t)cfg->enable16BitMode);

        CSL_REG32_WR(&cfaRegs->CFG_1, regVal);

        regVal = CSL_REG32_RD(&cfaRegs->DANDC_COM_CTRL);

        CSL_FINS(regVal, FLEXCFA_DANDC_COM_CTRL_LINEARBITWIDTH,
                                    (uint32_t)cfg->linearBitWidth);
        if(UTRUE == cfg->ccmEnable)
        {
            CSL_FINS(regVal, FLEXCFA_DANDC_COM_CTRL_CCMEN,
                                    (uint32_t)1u);
        }
        else
        {
            CSL_FINS(regVal, FLEXCFA_DANDC_COM_CTRL_CCMEN,
                                    (uint32_t)0u);
        }

        CSL_REG32_WR(&cfaRegs->DANDC_COM_CTRL, regVal);

        if(UTRUE == cfg->ccmEnable)
        {
            for (cnt = 0U; cnt < FCP_MAX_COLOR_COMP; cnt ++)
            {
                regVal = 0u;
                CSL_FINS(regVal, FLEXCFA_CCM_OCH0_ICH1_0_CCM_OCH0_ICH0,
                            (uint32_t)cfg->ccmConfig[cnt].inputCh0);
                CSL_FINS(regVal, FLEXCFA_CCM_OCH0_ICH1_0_CCM_OCH0_ICH1,
                            (uint32_t)cfg->ccmConfig[cnt].inputCh1);
                CSL_REG32_WR(&cfaRegs->CCM_OCH[cnt].ICH1_0, regVal);

                regVal = 0u;
                CSL_FINS(regVal, FLEXCFA_CCM_OCH0_ICH3_2_CCM_OCH0_ICH2,
                            (uint32_t)cfg->ccmConfig[cnt].inputCh2);
                CSL_FINS(regVal, FLEXCFA_CCM_OCH0_ICH3_2_CCM_OCH0_ICH3,
                            (uint32_t)cfg->ccmConfig[cnt].inputCh3);
                CSL_REG32_WR(&cfaRegs->CCM_OCH[cnt].ICH3_2, regVal);

                CSL_REG32_FINS(&cfaRegs->CCM_OCH[cnt].OFFSET,
                                FLEXCFA_CCM_OCH0_OFFSET_CCM_OCH0_OFFSET,
                                cfg->ccmConfig[cnt].offset);
            }
        }

        FcpSetCfaFir(cfaRegs, cfg);
    }

    return (status);
}

/* Function to set the companding module configuration
   Companding module is used for converting 16bit GLBCE output to 12bit */
int32_t CSL_fcpSetCompConfig(CSL_flexcfaRegs *cfaRegs, const Vhwa_LutConfig *cfg)
{
    int32_t            status = FVID2_SOK;
    uint32_t           cnt;
    volatile uint32_t  regVal;
    volatile uint32_t *inLutAddr = NULL;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Null pointer check */
    if ((NULL == cfg) || (NULL == cfaRegs))
    {
        status = FVID2_EBADARGS;
    }
    /* LDRA_JUSTIFY_END */

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)UTRUE == cfg->enable)
        {
            inLutAddr = cfg->tableAddr;
            for (cnt = 0U; cnt < FCP_COMPANDING_LUT_SIZE; cnt += 2U)
            {
                regVal = 0U;
                CSL_FINS(regVal, FLEXCFA_LUT_LUT_ENTRY_LO, inLutAddr[cnt]);

                /* Two lut entries are stored in a single 32bit word,
                 * except the last one */
                if ((cnt + 1U) < FCP_COMPANDING_LUT_SIZE)
                {
                    CSL_FINS(regVal, FLEXCFA_LUT_LUT_ENTRY_HI,
                        inLutAddr[cnt + 1U]);
                }
                CSL_REG32_WR(&cfaRegs->LUT[cnt / 2u], regVal);
            }

            /* Set the input bit width and enble the companding module */
            regVal = CSL_REG32_RD(&cfaRegs->CFG_1);
            CSL_FINS(regVal, FLEXCFA_CFG_1_BITWIDTH, cfg->inputBits);
            CSL_FINS(regVal, FLEXCFA_CFG_1_LUT_ENABLE, 1U);
            CSL_REG32_WR(&cfaRegs->CFG_1, regVal);
        }
        else
        {
            /* Set the input bit width and enble the companding module */
            regVal = CSL_REG32_RD(&cfaRegs->CFG_1);
            CSL_FINS(regVal, FLEXCFA_CFG_1_BITWIDTH, cfg->inputBits);
            CSL_FINS(regVal, FLEXCFA_CFG_1_LUT_ENABLE, 0U);
            CSL_REG32_WR(&cfaRegs->CFG_1, regVal);
        }
    }

    return (status);
}

/* Function to set CCM (color conversion module) configuration */
int32_t CSL_fcpSetCcmConfig(CSL_flexcc_cfgRegs *fcpRegs,
    const Fcp_CcmConfig *cfg)
{
    int32_t            status = FVID2_SOK;
    uint32_t           cnt;
    volatile uint32_t  regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* check for null pointer */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        for (cnt = 0U; cnt < FCP_MAX_CCM_COEFF; cnt ++)
        {
            regVal = CSL_REG32_RD(&fcpRegs->CCM[cnt].W_0_1);
            CSL_FINS(regVal, FLEXCC_CFG_CCM_W0_0_1_W_0,
				(uint32_t)cfg->weights[cnt][0U]);
            CSL_FINS(regVal, FLEXCC_CFG_CCM_W0_0_1_W_1,
				(uint32_t)cfg->weights[cnt][1U]);
            CSL_REG32_WR(&fcpRegs->CCM[cnt].W_0_1, regVal);

            regVal = CSL_REG32_RD(&fcpRegs->CCM[cnt].W_2_3);
            CSL_FINS(regVal, FLEXCC_CFG_CCM_W0_2_3_W_2,
				(uint32_t)cfg->weights[cnt][2U]);
            CSL_FINS(regVal, FLEXCC_CFG_CCM_W0_2_3_W_3,
				(uint32_t)cfg->weights[cnt][3U]);
            CSL_REG32_WR(&fcpRegs->CCM[cnt].W_2_3, regVal);

            CSL_REG32_FINS(&fcpRegs->CCM[cnt].OFFSET,
                FLEXCC_CFG_CCM_OFFSET_0_OFFSET_0, cfg->offsets[cnt]);
        }
    }

    return (status);
}

/* Function to set Contrast strentch/Gamma correction modules configuration */
int32_t CLS_fcpSetGammaConfig(CSL_flexcc_cfgRegs *fcpRegs, uint32_t lut1Addr[],
    uint32_t lut2Addr[], uint32_t lut3Addr[], const Fcp_GammaConfig *cfg)
{
    int32_t           status = FVID2_SOK;
    uint32_t          cnt;
    uint32_t         *lut0;
    uint32_t         *lut1;
    uint32_t         *lut2;
    volatile uint32_t regVal = 0U;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Check for null pointer */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        if ((uint32_t)UTRUE == cfg->enable)
        /* LDRA_JUSTIFY_END */
        {
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The parameters are pre-validated by the caller before the control reaches here.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if ((NULL == cfg->tableC1) || (NULL == cfg->tableC2) ||
                (NULL == cfg->tableC3))
            {
                status = FVID2_EBADARGS;
            }
            /* LDRA_JUSTIFY_END */

            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The parameters are pre-validated by the caller before the control reaches here.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if ((NULL == lut1Addr) || (NULL == lut2Addr) ||
                (NULL == lut3Addr))
            {
                status = FVID2_EBADARGS;
            }
            /* LDRA_JUSTIFY_END */
        }
    }

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The negative test framework and test apps cannot reach this portion.
        The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
        Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach here.
        <justification end> */
        if ((uint32_t)UTRUE == cfg->enable)
        /* LDRA_JUSTIFY_END */
        {
            lut0 = cfg->tableC1;
            lut1 = cfg->tableC2;
            lut2 = cfg->tableC3;
            for (cnt = 0U; cnt < FCP_GAMMA_LUT_SIZE; cnt += 2U)
            {
                regVal = 0u;

                CSL_FINS(regVal, FLEXCC_CONTRASTC1_LUT_CONTRASTC1_LUT_0,
                    lut0[cnt]);

                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_GAMMA_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_CONTRASTC1_LUT_CONTRASTC1_LUT_1,
                        lut0[cnt + 1u]);
                }
                CSL_REG32_WR(&lut1Addr[cnt / 2U], regVal);

                regVal = 0u;
                CSL_FINS(regVal, FLEXCC_CONTRASTC1_LUT_CONTRASTC1_LUT_0,
                    lut1[cnt]);
                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_GAMMA_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_CONTRASTC1_LUT_CONTRASTC1_LUT_1,
                        lut1[cnt + 1U]);
                }
                CSL_REG32_WR(&lut2Addr[cnt / 2U], regVal);

                regVal = 0u;
                CSL_FINS(regVal, FLEXCC_CONTRASTC1_LUT_CONTRASTC1_LUT_0,
                    lut2[cnt]);
                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_GAMMA_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_CONTRASTC1_LUT_CONTRASTC1_LUT_1,
                        lut2[cnt + 1U]);
                }
                CSL_REG32_WR(&lut3Addr[cnt / 2U], regVal);
            }

            regVal = CSL_REG32_RD(&fcpRegs->CFG_2);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_2_CONTRASTBITCLIP, cfg->outClip);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_2_CONTRASTEN, (uint32_t)1U);
            CSL_REG32_WR(&fcpRegs->CFG_2, regVal);
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The negative test framework and test apps cannot reach this portion.
        The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
        Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach here.
        <justification end> */
        else
        {
            /* Disable Gamm correction/contrast stretch */
            regVal = CSL_REG32_RD(&fcpRegs->CFG_2);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_2_CONTRASTBITCLIP, cfg->outClip);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_2_CONTRASTEN, (uint32_t)0U);
            CSL_REG32_WR(&fcpRegs->CFG_2, regVal);
        }
        /* LDRA_JUSTIFY_END */
    }

    return (status);
}

/* Function to set RGB2HSV modules configuration
   This module converts input 12b RGB to Saturation and Value */
int32_t CSL_fcpSetRgb2HsvConfig(CSL_flexcc_cfgRegs *fcpRegs,
    const Fcp_Rgb2HsvConfig *cfg)
{
    int32_t           status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Check for Null pointer */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&fcpRegs->CFG_1);
        /* Select the input to the HSV module, could be gamma corrected
         * data or non-corrected data */
        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXRGBHSV,
            (uint32_t)cfg->inputSelect);
        /* Select the input for H1 input,
         * H1 input is used for grey scale calculation */
        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXRGBHSV_H1, (uint32_t)cfg->h1Input);
        /* Select the input for H2 input,
         * H2 input is used for grey scale calculation */
        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXRGBHSV_H2, (uint32_t)cfg->h2Input);
        /* configures to make use of WB corrected for grey scale calculation */
        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXRGBHSV_MUX_V,
            cfg->useWbDataForGreyCalc);
        CSL_REG32_WR(&fcpRegs->CFG_1, regVal);

        /* Set the weights and offset used for calculating grey scale */
        regVal = CSL_REG32_RD(&fcpRegs->RGBHSV_W0);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_W0_W11, cfg->weights[0U]);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_W0_W12, cfg->weights[1U]);
        CSL_REG32_WR(&fcpRegs->RGBHSV_W0, regVal);

        regVal = CSL_REG32_RD(&fcpRegs->RGBHSV_W1);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_W1_W13, cfg->weights[2U]);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_W1_OFFSET_1, cfg->offset);
        CSL_REG32_WR(&fcpRegs->RGBHSV_W1, regVal);

        /* Set the threshold values used in white balance */
        regVal = CSL_REG32_RD(&fcpRegs->RGBHSV_WB_LINLOGTHR_1);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_WB_LINLOGTHR_1_THR_0,
            cfg->threshold[0U]);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_WB_LINLOGTHR_1_THR_1,
            cfg->threshold[1U]);
        CSL_REG32_WR(&fcpRegs->RGBHSV_WB_LINLOGTHR_1, regVal);

        regVal = CSL_REG32_RD(&fcpRegs->RGBHSV_WB_LINLOGTHR_2);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_WB_LINLOGTHR_2_THR_2,
            cfg->threshold[2U]);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_WB_LINLOGTHR_2_SATMINTHR,
            cfg->satMinThr);
        CSL_REG32_WR(&fcpRegs->RGBHSV_WB_LINLOGTHR_2, regVal);

        /* Set the white balance offset */
        regVal = CSL_REG32_RD(&fcpRegs->RGBHSV_OFF1);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_OFF1_OFFSET_1, cfg->wbOffset[0U]);
        CSL_FINS(regVal, FLEXCC_CFG_RGBHSV_OFF1_OFFSET_2, cfg->wbOffset[1U]);
        CSL_REG32_WR(&fcpRegs->RGBHSV_OFF1, regVal);

        CSL_REG32_FINS(&fcpRegs->RGBHSV_OFF2, FLEXCC_CFG_RGBHSV_OFF2_OFFSET_3,
            cfg->wbOffset[2U]);

        /* Set saturation numerator and denominator */
        regVal = CSL_REG32_RD(&fcpRegs->CFG_2);
        CSL_FINS(regVal, FLEXCC_CFG_CFG_2_HSVSATMODE, (uint32_t)cfg->satMode);
        CSL_FINS(regVal, FLEXCC_CFG_CFG_2_HSVSATDIVMODE, (uint32_t)cfg->satDiv);
        CSL_REG32_WR(&fcpRegs->CFG_2, regVal);
    }

    return (status);
}

/* Function to set RGB2YUV coefficients */
int32_t CLS_fcpSetRgb2YuvConfig(CSL_flexcc_cfgRegs *fcpRegs,
    const Fcp_Rgb2YuvConfig *cfg)
{
    int32_t           status = FVID2_SOK;
    uint32_t          cnt;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        for (cnt = 0U; cnt < FCP_MAX_RGB2YUV_COEFF; cnt ++)
        {
            regVal = CSL_REG32_RD(&fcpRegs->RGB2YUV[cnt].W0);
            CSL_FINS(regVal, FLEXCC_CFG_RGBYUV_W01_W_01,
				(uint32_t)cfg->weights[cnt][0U]);
            CSL_FINS(regVal, FLEXCC_CFG_RGBYUV_W01_W_02,
				(uint32_t)cfg->weights[cnt][1U]);
            CSL_REG32_WR(&fcpRegs->RGB2YUV[cnt].W0, regVal);

            regVal = CSL_REG32_RD(&fcpRegs->RGB2YUV[cnt].W1);
            CSL_FINS(regVal, FLEXCC_CFG_RGBYUV_W02_W_03,
				(uint32_t)cfg->weights[cnt][2U]);
            CSL_FINS(regVal, FLEXCC_CFG_RGBYUV_W02_OFFSET_0,
				(uint32_t)cfg->offsets[cnt]);
            CSL_REG32_WR(&fcpRegs->RGB2YUV[cnt].W1, regVal);
        }
    }

    return (status);
}

int32_t CSL_fcpHistChangeBank(CSL_flexcc_cfgRegs *fcpRegs)
{
    int32_t           status = FVID2_SOK;
    uint32_t          bank;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion. fcpRegs is pre-validated for not being NULL
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    <justification end> */
    if (NULL == fcpRegs)
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        bank = CSL_REG32_RD(&fcpRegs->CFG_HIST_1);
        bank = ((bank & CSL_FLEXCC_CFG_CFG_HIST_1_BANK_MASK) >>
                            CSL_FLEXCC_CFG_CFG_HIST_1_BANK_SHIFT);
        bank = (bank ^ 0x1U) & 0x1U;

        CSL_REG32_FINS(&fcpRegs->CFG_HIST_1,
                            FLEXCC_CFG_CFG_HIST_1_BANK, bank);
    }

    return status;
}

/* Function to set histogram configuration */
int32_t CSL_fcpSetHistogramConfig(CSL_flexcc_cfgRegs *fcpRegs,
    const Fcp_HistConfig *cfg)
{
    int32_t           status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)UTRUE == cfg->enable)
        {
            regVal = CSL_REG32_RD(&fcpRegs->CFG_HIST_2);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_HIST_2_HISTMODE, cfg->input);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_HIST_2_HISTSIZEX,
                cfg->roi.cropWidth);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_HIST_2_HISTSIZEY,
                cfg->roi.cropHeight);
            CSL_REG32_WR(&fcpRegs->CFG_HIST_2, regVal);

            regVal = CSL_REG32_RD(&fcpRegs->CFG_HIST_1);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_HIST_1_HISTSTARTX,
                cfg->roi.cropStartX);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_HIST_1_HISTSTARTY,
                cfg->roi.cropStartY);
            CSL_FINS(regVal, FLEXCC_CFG_CFG_HIST_1_HISTEN, 1U);
            CSL_REG32_WR(&fcpRegs->CFG_HIST_1, regVal);
        }
        else
        {
            CSL_REG32_FINS(&fcpRegs->CFG_HIST_1,
                FLEXCC_CFG_CFG_HIST_1_HISTEN, 0U);
        }
    }

    return (status);
}

/* Function reads histogram data from the memory and stores in hist array */
int32_t CSL_fcpReadHistogram(const CSL_flexcc_histRegs *histRegs,
    uint32_t histData[])
{
    int32_t           status = FVID2_SOK;
    uint32_t          cnt;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion. histRegs and histData are pre-validated for not being NULL.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.   
    <justification end> */
    if ((NULL == histRegs) || (NULL == histData))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        for (cnt = 0u; cnt < FCP_HISTOGRAM_SIZE; cnt ++)
        {
            histData[cnt] = CSL_REG32_RD(&histRegs->HIST[cnt]);
        }
    }

    return (status);
}

/* Function to set the RGB Lut used for converting 12b rgb to 8b
   there is a single bit enable all three luts,
   when disabled, shift is used for bit depth conversion */
int32_t CSL_fcpSetRgbLutConfig(CSL_flexcc_cfgRegs *fcpRegs, uint32_t lut1Addr[],
    uint32_t lut2Addr[], uint32_t lut3Addr[], const Fcp_RgbLutConfig *cfg)
{
    int32_t           status = FVID2_SOK;
    volatile uint32_t regVal = 0U;
    uint32_t         *lut0;
    uint32_t         *lut1;
    uint32_t         *lut2;
    uint32_t          cnt;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Check for Null pointer */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        if ((uint32_t)UTRUE == cfg->enable)
        /* LDRA_JUSTIFY_END */
        {
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The parameters are pre-validated by the caller before the control reaches here.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if ((NULL == lut1Addr) || (NULL == lut2Addr) ||
                (NULL == lut3Addr) ||
                (NULL == cfg->redLutAddr) || (NULL == cfg->greenLutAddr) ||
                (NULL == cfg->blueLutAddr))
            /* LDRA_JUSTIFY_END */
            {
                status = FVID2_EBADARGS;
            }
        }
    }

    if (FVID2_SOK == status)
    {
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The negative test framework and test apps cannot reach this portion.
        The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
        Effect on this unit: Unused feature, cannot be enabled, control cannot reach here.
        <justification end> */
        if ((uint32_t)UTRUE == cfg->enable)
        /* LDRA_JUSTIFY_END */
        {
            /* Set the Lut in the memory */

            /* Lut1Address is for Red Lut,
             * Lut2Address is for green lut
             * Lut3Address is for Blue lut */
            lut0 = cfg->redLutAddr;
            lut1 = cfg->greenLutAddr;
            lut2 = cfg->blueLutAddr;
            for (cnt = 0U; cnt < FCP_RGB_YUVSAT_LUT_SIZE; cnt += 2U)
            {
                CSL_FINS(regVal, FLEXCC_Y8R8_LUT_Y8R8_LUT_0, lut0[cnt]);

                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_RGB_YUVSAT_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_Y8R8_LUT_Y8R8_LUT_1,
                        lut0[cnt + 1U]);
                }
                CSL_REG32_WR(&lut1Addr[cnt / 2U], regVal);

                CSL_FINS(regVal, FLEXCC_C8G8_LUT_C8G8_LUT_0, lut1[cnt]);
                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_RGB_YUVSAT_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_C8G8_LUT_C8G8_LUT_1,
                        lut1[cnt + 1U]);
                }
                CSL_REG32_WR(&lut2Addr[cnt / 2U], regVal);

                CSL_FINS(regVal, FLEXCC_S8B8_LUT_S8B8_LUT_0, lut2[cnt]);
                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_RGB_YUVSAT_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_S8B8_LUT_S8B8_LUT_1,
                        lut2[cnt + 1U]);
                }
                CSL_REG32_WR(&lut3Addr[cnt / 2U], regVal);
            }

            /* Set the Luts in the */
            /* Enable RGB Lut, there is a single enable bit for enabling
             * all three Luts for RGB */
            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_RGB8LUTEN, 1U);
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The negative test framework and test apps cannot reach this portion.
        The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
        Effect on this unit: Unused feature, cannot be enabled, control cannot reach here.
        <justification end> */
        else
        {
            /* Enable Lut for RGB */
            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_RGB8LUTEN, 0U);
        }
        /* LDRA_JUSTIFY_END */
    }

    return (status);
}

/* Function to Set companding lut for YUV and saturation */
int32_t CSL_fcpSetYuvSatLutConfig(CSL_flexcc_cfgRegs *fcpRegs,
    uint32_t lut1Addr[], uint32_t lut2Addr[], uint32_t lut3Addr[],
    const Fcp_YuvSatLutConfig *cfg)
{
    int32_t             status = FVID2_SOK;
    uint32_t            cnt;
    volatile uint32_t   regVal = 0;
    uint32_t           *lut = NULL;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Check for Null pointer */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)UTRUE == cfg->enableLumaLut)
        {
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The parameters are pre-validated by the caller before the control reaches here.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if ((0U == lut1Addr) || (NULL == cfg->lumaLutAddr))
            /* LDRA_JUSTIFY_END */
            {
                status = FVID2_EBADARGS;
            }
        }
        if ((uint32_t)UTRUE == cfg->enableChromaLut)
        {
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The parameters are pre-validated by the caller before the control reaches here.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if ((0U == lut1Addr) || (NULL == cfg->chromaLutAddr))
            /* LDRA_JUSTIFY_END */
            {
                status = FVID2_EBADARGS;
            }
        }
        if ((uint32_t)UTRUE == cfg->enableSaturLut)
        {
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The parameters are pre-validated by the caller before the control reaches here.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if ((0U == lut3Addr) || (NULL == cfg->saturLutAddr))
            /* LDRA_JUSTIFY_END */
            {
                status = FVID2_EBADARGS;
            }
        }
    }

    if (FVID2_SOK == status)
    {
        /* Luma input size need to be set even if lut is disabled,
         * it is also used for calculating shift size for luma */
        CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_Y8INBITWIDTH,
            cfg->lumaInputBits);

        if ((uint32_t)UTRUE == cfg->enableLumaLut)
        {
            lut = cfg->lumaLutAddr;
            for (cnt = 0U; cnt < FCP_RGB_YUVSAT_LUT_SIZE; cnt += 2U)
            {
                CSL_FINS(regVal, FLEXCC_Y8R8_LUT_Y8R8_LUT_0, lut[cnt]);
                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_RGB_YUVSAT_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_Y8R8_LUT_Y8R8_LUT_1,
                        lut[cnt + 1u]);
                }
                CSL_REG32_WR(&lut1Addr[cnt / 2U], regVal);
            }

            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_Y8LUTEN, 1U);
        }
        else
        {
            /* Disable Luma companding,
             * shift will be used in this case */
            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_Y8LUTEN, 0U);
        }

        /* Set the chroma companding Lut */
        if ((uint32_t)UTRUE == cfg->enableChromaLut)
        {
            lut = cfg->chromaLutAddr;
            for (cnt = 0U; cnt < FCP_RGB_YUVSAT_LUT_SIZE; cnt += 2U)
            {
                CSL_FINS(regVal, FLEXCC_C8G8_LUT_C8G8_LUT_0, lut[cnt]);

                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_RGB_YUVSAT_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_C8G8_LUT_C8G8_LUT_1,
                        lut[cnt + 1u]);
                }
                CSL_REG32_WR(&lut2Addr[cnt / 2U], regVal);
            }

            /* Enable Chroma Lut */
            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_C8LUTEN, 1U);
        }
        else
        {
            /* Disable chroma companding,
             * shift will be used in this case */
            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_C8LUTEN, 0U);
        }

        /* Set the Saturation companding Lut */
        if ((uint32_t)UTRUE == cfg->enableSaturLut)
        {
            lut = cfg->saturLutAddr;
            for (cnt = 0U; cnt < FCP_RGB_YUVSAT_LUT_SIZE; cnt += 2U)
            {
                CSL_FINS(regVal, FLEXCC_S8B8_LUT_S8B8_LUT_0, lut[cnt]);

                /* LUT has 513 entries,
                 * Two lut entries are stored in one 32bit memory
                 * so Last Lut entry will be stored only on Low position */
                if (cnt < (FCP_RGB_YUVSAT_LUT_SIZE - 1U))
                {
                    CSL_FINS(regVal, FLEXCC_S8B8_LUT_S8B8_LUT_1,
                        lut[cnt + 1u]);
                }
                CSL_REG32_WR(&lut3Addr[cnt / 2U], regVal);
            }

            /* Enable Chroma Lut */
            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_SATLUTEN, 1U);
        }
        else
        {
            /* Disable saturation companding,
             * shift will be used in this case */
            CSL_REG32_FINS(&fcpRegs->CFG_2, FLEXCC_CFG_CFG_2_SATLUTEN, 0U);
        }
    }

    return (status);
}

/* Function to set the output multiplexer */
int32_t CSL_fcpSetOutputMux(CSL_flexcc_cfgRegs *fcpRegs,
    const CslFcp_OutputSelect *cfg)
{
    int32_t           status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == cfg) || (NULL == fcpRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&fcpRegs->CFG_1);
        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_S8B8OUTEN, cfg->sat8OutSel);
        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_C8G8OUTEN, cfg->chroma8OutSel);
        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_C12OUTEN, cfg->chroma12OutSel);

        /* This configures Y8R8OutEn MUX in CFG1 register */
        if (FCP_LUMA8_OUT_DISABLE == cfg->luma8OutSel)
        {
            CSL_FINS(regVal, FLEXCC_CFG_CFG_1_Y8R8OUTEN, (uint32_t)0U);
        }
        else if (FCP_LUMA8_OUT_RED8 == cfg->luma8OutSel)
        {
            CSL_FINS(regVal, FLEXCC_CFG_CFG_1_Y8R8OUTEN, (uint32_t)2U);
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The negative test framework and test apps cannot reach this portion.
        The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
        Effect on this unit: Unused feature, cannot be enabled, control cannot reach here.
        <justification end> */
        else if (FCP_LUMA8_OUT_CFA_C2 == cfg->luma8OutSel)
        {
            CSL_FINS(regVal, FLEXCC_CFG_CFG_1_Y8R8OUTEN, (uint32_t)3U);
        }
        else
        /* LDRA_JUSTIFY_END */
        {
            CSL_FINS(regVal, FLEXCC_CFG_CFG_1_Y8R8OUTEN, (uint32_t)1U);

            /* This configures MuxY8_Out MUX in CFG1 register */
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The negative test framework and test apps cannot reach this portion.
            The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
            Effect on this unit: Unused feature, cannot be enabled, control cannot reach here.
            <justification end> */
            if (FCP_LUMA8_OUT_RGB2YUV_Y8 == cfg->luma8OutSel)
            {
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXY8_OUT, 1U);
            }
            /* LDRA_JUSTIFY_END */
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The test framework and test apps cannot reach this portion.
            The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
            Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
            <justification end> */
            else if (FCP_LUMA8_OUT_RGB2HSV_Y8 == cfg->luma8OutSel)
            {
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXY8_OUT, 2U);
            }
            else
            {
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXY8_OUT, 0U);
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXC1_4,
                    cfg->luma8OutSel - 1U);
            }
/* LDRA_JUSTIFY_END */
        }

        /* This configures MuxY12_Out MUX in CFG1 register */
        if (FCP_LUMA12_OUT_DISABLE == cfg->luma12OutSel)
        {
            CSL_FINS(regVal, FLEXCC_CFG_CFG_1_Y12OUTEN, (uint32_t)0U);
        }
        else
        {
            CSL_FINS(regVal, FLEXCC_CFG_CFG_1_Y12OUTEN, (uint32_t)1U);
            if (FCP_LUMA12_OUT_RGB2YUV == cfg->luma12OutSel)
            {
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXY12_OUT, 1U);
            }
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The test framework and test apps cannot reach this portion.
            The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
            Effect on this unit: There is no impact, as the feature is unused, cannot be enabled, and control cannot reach this condition.
            <justification end> */
            else if (FCP_LUMA8_OUT_RGB2HSV_Y8 == cfg->luma12OutSel)
            {
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXY12_OUT, 2U);
            }
            else
            {
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXY12_OUT, 0U);
                /* TODO: Mux MUXC1_4 is common between Y12 and Y8
                 * outputs. */
                CSL_FINS(regVal, FLEXCC_CFG_CFG_1_MUXC1_4,
                    cfg->luma12OutSel - 1U);
            }
            /* LDRA_JUSTIFY_END */
        }

        CSL_FINS(regVal, FLEXCC_CFG_CFG_1_CHROMA_MODE, cfg->chromaMode);

        CSL_REG32_WR(&fcpRegs->CFG_1, regVal);

    }

    return (status);
}

int32_t CSL_fcpSetCfaCompLut(CSL_flexcfaRegs *cfaRegs,
                            const Fcp_comDecomLutConfig *cfg)
{
    int32_t            status = FVID2_SOK;
    uint32_t           cnt, idx;
    volatile uint32_t  regVal;
    volatile uint32_t *lutAddr = NULL;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Null pointer check */
    if ((NULL == cfg) || (NULL == cfaRegs))
    {
        status = FVID2_EBADARGS;
    }
    /* LDRA_JUSTIFY_END */
 
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)UTRUE == cfg->enable)
        {
            CSL_REG32_FINS(&cfaRegs->DANDC_COM_CTRL,
                            FLEXCFA_DANDC_COM_CTRL_CMPDLUTEN, 1U);

            for (idx = 0U; idx < FCP_MAX_COLOR_COMP; idx++)
            {
                lutAddr = cfg->tableAddr[idx];
                for (cnt = 0U; cnt < FCP_COMPANDING_LUT_SIZE; cnt += 2U)
                {
                    regVal = 0U;
                    CSL_FINS(regVal, FLEXCFA_CLUT0_LUT_ENTRY_LO, lutAddr[cnt]);

                    /* Two lut entries are stored in a single 32bit word,
                     * except the last one */
                    if ((cnt + 1U) < FCP_COMPANDING_LUT_SIZE)
                    {
                        CSL_FINS(regVal, FLEXCFA_CLUT0_LUT_ENTRY_HI,
                            lutAddr[cnt + 1U]);
                    }
                    if(0u == idx)
                    {
                        CSL_REG32_WR(&cfaRegs->CLUT0[cnt / 2u], regVal);
                    }
                    else if (1u == idx)
                    {
                        CSL_REG32_WR(&cfaRegs->CLUT1[cnt / 2u], regVal);
                    }
                    else if (2u == idx)
                    {
                        CSL_REG32_WR(&cfaRegs->CLUT2[cnt / 2u], regVal);
                    }
                    else
                    {
                        CSL_REG32_WR(&cfaRegs->CLUT3[cnt / 2u], regVal);
                    }
                }
            }
        }
        else
        {
            CSL_REG32_FINS(&cfaRegs->DANDC_COM_CTRL,
                            FLEXCFA_DANDC_COM_CTRL_CMPDLUTEN, 0U);
        }
    }

    return (status);
}

int32_t CSL_fcpSetCfaDeCompLut(CSL_flexcfaRegs *cfaRegs,
                               CSL_flexcfa_dlutsRegs *cfaDLutRegs,
                               const Fcp_comDecomLutConfig *cfg)
{
    int32_t            status = FVID2_SOK;
    uint32_t           cnt, idx;
    volatile uint32_t  regVal;
    volatile uint32_t *lutAddr = NULL;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    /* Null pointer check */
    if ((NULL == cfg) || (NULL == cfaRegs) || (NULL == cfaDLutRegs))
    {
        status = FVID2_EBADARGS;
    }
/* LDRA_JUSTIFY_END */

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)UTRUE == cfg->enable)
        {
            CSL_REG32_FINS(&cfaRegs->DANDC_COM_CTRL,
                            FLEXCFA_DANDC_COM_CTRL_DCMPDLUTEN, 1U);

            for (idx = 0U; idx < FCP_MAX_COLOR_COMP; idx++)
            {
                lutAddr = cfg->tableAddr[idx];
                for (cnt = 0U; cnt < FCP_COMPANDING_LUT_SIZE; cnt ++)
                {
                    regVal = 0U;
                    CSL_FINS(regVal, FLEXCFA_DLUTS_DLUT0_LUT_ENTRY, lutAddr[cnt]);

                    if(0u == idx)
                    {
                        CSL_REG32_WR(&cfaDLutRegs->DLUT0[cnt], regVal);
                    }
                    else if (1u == idx)
                    {
                        CSL_REG32_WR(&cfaDLutRegs->DLUT1[cnt], regVal);
                    }
                    else if (2u == idx)
                    {
                        CSL_REG32_WR(&cfaDLutRegs->DLUT2[cnt], regVal);
                    }
                    else
                    {
                        CSL_REG32_WR(&cfaDLutRegs->DLUT3[cnt], regVal);
                    }
                }
            }
        }
        else
        {
            CSL_REG32_FINS(&cfaRegs->DANDC_COM_CTRL,
                            FLEXCFA_DANDC_COM_CTRL_DCMPDLUTEN, 0U);
        }
    }

    return (status);
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

/* Function to set horizontal and vertical gradient */
static void FcpSetCfaGradHorzVert(CSL_flexcfaRegs *cfaRegs, uint32_t set,
    const Fcp_CfaConfig *cfg)
{
    volatile uint32_t  regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can added to print the error status in a future release.
    <justification end> */
    if ((NULL != cfg) && (NULL != cfaRegs) && (set < FCP_CFA_MAX_SET))
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&cfaRegs->GRAD_INT_SET[set].GRAD_HZ);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_HZ_PHASE0, cfg->gradHzPh[set][0U]);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_HZ_PHASE1, cfg->gradHzPh[set][1U]);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_HZ_PHASE2, cfg->gradHzPh[set][2U]);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_HZ_PHASE3, cfg->gradHzPh[set][3U]);
        CSL_REG32_WR(&cfaRegs->GRAD_INT_SET[set].GRAD_HZ, regVal);

        regVal = CSL_REG32_RD(&cfaRegs->GRAD_INT_SET[set].GRAD_VT);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_VT_PHASE0, cfg->gradVtPh[set][0U]);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_VT_PHASE1, cfg->gradVtPh[set][1U]);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_VT_PHASE2, cfg->gradVtPh[set][2U]);
        CSL_FINS(regVal, FLEXCFA_SET0_GRAD_VT_PHASE3, cfg->gradVtPh[set][3U]);
        CSL_REG32_WR(&cfaRegs->GRAD_INT_SET[set].GRAD_VT, regVal);
    }
}

/* Function to set CFA Intensity for the given set */
static void FcpSetCfaIntensity(CSL_flexcfaRegs *cfaRegs, uint32_t set,
    const Fcp_CfaConfig *cfg)
{
    volatile uint32_t  regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can added to print the error status in a future release.
    <justification end> */
    if ((NULL != cfg) && (NULL != cfaRegs) && (set < FCP_CFA_MAX_SET))
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&cfaRegs->GRAD_INT_SET[set].INTENSITY0);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY0_BITFIELD_PH0,
            cfg->intsBitField[set][0U]);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY0_SHIFT_PH0,
            cfg->intsShiftPh[set][0U]);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY0_BITFIELD_PH1,
            cfg->intsBitField[set][1U]);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY0_SHIFT_PH1,
            cfg->intsShiftPh[set][1U]);
        CSL_REG32_WR(&cfaRegs->GRAD_INT_SET[set].INTENSITY0, regVal);

        regVal = CSL_REG32_RD(&cfaRegs->GRAD_INT_SET[set].INTENSITY1);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY1_BITFIELD_PH2,
            cfg->intsBitField[set][2U]);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY1_SHIFT_PH2,
            cfg->intsShiftPh[set][2U]);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY1_BITFIELD_PH3,
            cfg->intsBitField[set][3U]);
        CSL_FINS(regVal, FLEXCFA_SET0_INTENSITY1_SHIFT_PH3,
            cfg->intsShiftPh[set][3U]);
        CSL_REG32_WR(&cfaRegs->GRAD_INT_SET[set].INTENSITY1, regVal);
    }
}

/* Function to set CFA threshold for the given set */
static void FcpSetCfaThreshold(CSL_flexcfaRegs *cfaRegs, uint32_t set,
    const Fcp_CfaConfig *cfg)
{
    volatile uint32_t   regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can added to print the error status in a future release.
    <justification end> */
    if ((NULL != cfg) && (NULL != cfaRegs) && (set < FCP_CFA_MAX_SET))
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&cfaRegs->THR_SET[set].THR0_1);
        CSL_FINS(regVal, FLEXCFA_SET0_THR0_1_THR_0, cfg->thr[set][0U]);
        CSL_FINS(regVal, FLEXCFA_SET0_THR0_1_THR_1, cfg->thr[set][1U]);
        CSL_REG32_WR(&cfaRegs->THR_SET[set].THR0_1, regVal);

        regVal = CSL_REG32_RD(&cfaRegs->THR_SET[set].THR2_3);
        CSL_FINS(regVal, FLEXCFA_SET0_THR2_3_THR_2, cfg->thr[set][2U]);
        CSL_FINS(regVal, FLEXCFA_SET0_THR2_3_THR_3, cfg->thr[set][3U]);
        CSL_REG32_WR(&cfaRegs->THR_SET[set].THR2_3, regVal);

        regVal = CSL_REG32_RD(&cfaRegs->THR_SET[set].THR4_5);
        CSL_FINS(regVal, FLEXCFA_SET0_THR4_5_THR_4, cfg->thr[set][4U]);
        CSL_FINS(regVal, FLEXCFA_SET0_THR4_5_THR_5, cfg->thr[set][5U]);
        CSL_REG32_WR(&cfaRegs->THR_SET[set].THR4_5, regVal);

        regVal = CSL_REG32_RD(&cfaRegs->THR_SET[set].THR6);
        CSL_FINS(regVal, FLEXCFA_SET0_THR6_THR_6, cfg->thr[set][6U]);
        CSL_REG32_WR(&cfaRegs->THR_SET[set].THR6, regVal);
    }
}

static void FcpSetCfaFir(CSL_flexcfaRegs *cfaRegs, const Fcp_CfaConfig *cfg)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can added to print the error status in a future release.
    <justification end> */
    if ((NULL != cfg) && (NULL != cfaRegs))
    /* LDRA_JUSTIFY_END */
    {
        if(UTRUE == cfg->firConfig[0].enable)
        {
            CSL_REG32_FINS(&cfaRegs->FIR_SCALES_1_0,
                    FLEXCFA_FIR_SCALES_1_0_FIR_SCALER0,
                    cfg->firConfig[0].scaler);

            CSL_REG32_FINS(&cfaRegs->FIR_OFFSETS_1_0,
                    FLEXCFA_FIR_OFFSETS_1_0_FIR_OFFSET0,
                    cfg->firConfig[0].offset);
        }
        if(UTRUE == cfg->firConfig[1].enable)
        {
            CSL_REG32_FINS(&cfaRegs->FIR_SCALES_1_0,
                    FLEXCFA_FIR_SCALES_1_0_FIR_SCALER1,
                    cfg->firConfig[1].scaler);

            CSL_REG32_FINS(&cfaRegs->FIR_OFFSETS_1_0,
                    FLEXCFA_FIR_OFFSETS_1_0_FIR_OFFSET1,
                    cfg->firConfig[1].offset);
        }
        if(UTRUE == cfg->firConfig[2].enable)
        {
            CSL_REG32_FINS(&cfaRegs->FIR_SCALES_3_2,
                    FLEXCFA_FIR_SCALES_3_2_FIR_SCALER2,
                    cfg->firConfig[2].scaler);

            CSL_REG32_FINS(&cfaRegs->FIR_OFFSETS_3_2,
                    FLEXCFA_FIR_OFFSETS_3_2_FIR_OFFSET2,
                    cfg->firConfig[2].offset);
        }

        if(UTRUE == cfg->firConfig[3].enable)
        {
            CSL_REG32_FINS(&cfaRegs->FIR_SCALES_3_2,
                    FLEXCFA_FIR_SCALES_3_2_FIR_SCALER3,
                    cfg->firConfig[3].scaler);

            CSL_REG32_FINS(&cfaRegs->FIR_OFFSETS_3_2,
                    FLEXCFA_FIR_OFFSETS_3_2_FIR_OFFSET3,
                    cfg->firConfig[3].offset);
        }
    }
}

