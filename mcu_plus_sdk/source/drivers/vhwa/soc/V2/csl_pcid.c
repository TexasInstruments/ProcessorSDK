/**
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file csl_pcid.c
 *
 *  \brief Low level functions to configure PCID block
 *
 */


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <src/csl/include/csl_pcid.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Local Function Declaration                          */
/* ========================================================================== */

/* Check PCID configurations */
static int32_t pcidCheckConfig(const Pcid_Cfg *pCfg);

/* Configure PCID output muxes */
static int32_t pcidSetOpConfig(CSL_pcidRegs *pcidRegs,
                                const Pcid_OpChCfg *pCfg);
/* Configure IR interpolation config */
static int32_t pcidSetIrIntConfig(CSL_pcidRegs *pcidRegs,
                                const Pcid_Cfg *pCfg);
/* Configure IR substration configuration */
static int32_t pcidSetIrSubConfig(CSL_pcidRegs *pcidRegs,
                                CSL_irremap_lutRegs *pRemapLut,
                                const Pcid_IrSubCfg *pCfg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Local Functions                                 */
/* ========================================================================== */

static int32_t pcidCheckConfig(const Pcid_Cfg *pCfg)
{
    /* Checks
        #1 Check if pIRRemapLut is not NULL if irRemapLutEn is enabled and
            PCID block is enabled
        #2 If PCID is enabled, at least IR out or Bayer path should be enabled
        */
    int32_t status = FVID2_EBADARGS;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != pCfg)
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;
        if ((TRUE == pCfg->opChCfg.irSubtractEn) &&
            (TRUE == pCfg->irSubCfg.irRemapLutEn))
        {
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario. 
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
            <justification end> */
            if (NULL == pCfg->irSubCfg.pIRRemapLut)
            {
                status = FVID2_EINVALID_PARAMS;
            }
            /* LDRA_JUSTIFY_END */
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario. 
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
        <justification end> */
        if ((TRUE != pCfg->opChCfg.irOutEn) &&
            (TRUE != pCfg->opChCfg.bayerOutEn))
        {
            status = FVID2_EINVALID_PARAMS;
        }
        /* LDRA_JUSTIFY_END */
    }

    return status;
}

static int32_t pcidSetOpConfig(CSL_pcidRegs *pcidRegs,
                                const Pcid_OpChCfg *pCfg)
{
    int32_t status = FVID2_EBADARGS;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if ((NULL != pcidRegs) && (NULL != pCfg))
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;

        regVal = CSL_REG32_RD(&pcidRegs->CTRL);

        CSL_FINS(regVal, PCID_CTRL_IROUTEN, pCfg->irOutEn);
        CSL_FINS(regVal, PCID_CTRL_BAYEROUTEN, pCfg->bayerOutEn);
        CSL_FINS(regVal, PCID_CTRL_BAYEROUTSEL, pCfg->bayerOutSel);
        CSL_FINS(regVal, PCID_CTRL_RBINTPATIR, pCfg->rbIntpAtIR);
        CSL_FINS(regVal, PCID_CTRL_RBIRINTPMETHOD, pCfg->rbIRIntpMethod);
        CSL_FINS(regVal, PCID_CTRL_IRSUBTRACTEN, pCfg->irSubtractEn);

        CSL_REG32_WR(&pcidRegs->CTRL, regVal);

    }

    return (status);
}

static int32_t pcidSetIrIntConfig(CSL_pcidRegs *pcidRegs,
                                    const Pcid_Cfg *pCfg)
{
    int32_t status = FVID2_EBADARGS;
    const Pcid_RBIrIntThCfg *thCfg;
    const Pcid_RBIrIntClrDiffCfg *clrDifCfg;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if ((NULL != pcidRegs) && (NULL != pCfg))
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;
        thCfg = &pCfg->thRBIrCfg;
        clrDifCfg = &pCfg->clrDiffRBIrCfg;

        regVal = CSL_REG32_RD(&pcidRegs->RBIRINTPCFG1);
        CSL_FINS(regVal, PCID_RBIRINTPCFG1_T1, thCfg->t1);
        CSL_FINS(regVal, PCID_RBIRINTPCFG1_T2, thCfg->t2);
        CSL_REG32_WR(&pcidRegs->RBIRINTPCFG1, regVal);

        regVal = CSL_REG32_RD(&pcidRegs->RBIRINTPCFG2);
        CSL_FINS(regVal, PCID_RBIRINTPCFG2_T3, thCfg->t3);
        CSL_REG32_WR(&pcidRegs->RBIRINTPCFG2, regVal);

        regVal = CSL_REG32_RD(&pcidRegs->RBIRCOLORDIFFCFG);
        CSL_FINS(regVal, PCID_RBIRCOLORDIFFCFG_GHFXFERFACTORIR, clrDifCfg->gHFXferFactorIr);
        CSL_FINS(regVal, PCID_RBIRCOLORDIFFCFG_GHFXFERFACTOR, clrDifCfg->gHFXferFactor);
        CSL_REG32_WR(&pcidRegs->RBIRCOLORDIFFCFG, regVal);

    }

    return (status);
}

static int32_t pcidSetIrRemapLut(CSL_irremap_lutRegs *pRemapLut,
                                    const Pcid_IrSubCfg *pCfg)
{
    int32_t status = FVID2_EBADARGS;
    uint32_t i = 0U;
    volatile uint32_t regVal;
    uint32_t lut_iter = 0;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (((NULL != pRemapLut) && (NULL != pCfg)) && (NULL != pCfg->pIRRemapLut))
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;
        do
        {
            regVal = CSL_REG32_RD(&pRemapLut->LUT[i]);
            CSL_FINS(regVal,IRREMAP_LUT_LUT_LUT_ENTRY_LO,pCfg->pIRRemapLut->lut[lut_iter++]);
            CSL_FINS(regVal,IRREMAP_LUT_LUT_LUT_ENTRY_HI,pCfg->pIRRemapLut->lut[lut_iter++]);
            CSL_REG32_WR(&pRemapLut->LUT[i], regVal);
            i++;

        }while (i < PCID_IR_REMAP_LUT_SIZE);
    }

    return (status);
}


static int32_t pcidSetIrSubConfig(CSL_pcidRegs *pcidRegs,
                                    CSL_irremap_lutRegs *pRemapLut,
                                    const Pcid_IrSubCfg *pCfg)
{
    int32_t status = FVID2_EBADARGS;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if ((NULL != pcidRegs) && (NULL != pCfg))
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;

        regVal = CSL_REG32_RD(&pcidRegs->CTRL);
        CSL_FINS(regVal, PCID_CTRL_IRSUBTRACTFILTEN, pCfg->irSubtractFiltEn);
        CSL_FINS(regVal, PCID_CTRL_IRREMAPLUTEN, pCfg->irRemapLutEn);
        CSL_REG32_WR(&pcidRegs->CTRL, regVal);

        if (FALSE != pCfg->irRemapLutEn)
        {
            status = pcidSetIrRemapLut(pRemapLut, pCfg);
        }

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion. This branch statement checks for errors accumulated in previous steps. Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached. 
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
        <justification end> */
        if (FVID2_SOK == status)
        /* LDRA_JUSTIFY_END */
        {
            regVal = CSL_REG32_RD(&pcidRegs->IRSUBCFG1);
            CSL_FINS(regVal, PCID_IRSUBCFG1_CUTOFFTH, pCfg->cutOffTh);
            CSL_REG32_WR(&pcidRegs->IRSUBCFG1, regVal);

            regVal = CSL_REG32_RD(&pcidRegs->IRSUBCFG2);
            CSL_FINS(regVal, PCID_IRSUBCFG2_TRANSITIONRANGE, pCfg->transitionRange);
            CSL_FINS(regVal, PCID_IRSUBCFG2_TRANSITIONRANGEINV, pCfg->transitionRangeInv);
            CSL_REG32_WR(&pcidRegs->IRSUBCFG2, regVal);

            regVal = CSL_REG32_RD(&pcidRegs->IRSUBDISTSCALELUT0);
            CSL_FINS(regVal, PCID_IRSUBDISTSCALELUT0_D0SCALE, pCfg->irSubDistScaleLut[0U]);
            CSL_FINS(regVal, PCID_IRSUBDISTSCALELUT0_D1SCALE, pCfg->irSubDistScaleLut[1U]);
            CSL_REG32_WR(&pcidRegs->IRSUBDISTSCALELUT0, regVal);

            regVal = CSL_REG32_RD(&pcidRegs->IRSUBDISTSCALELUT1);
            CSL_FINS(regVal, PCID_IRSUBDISTSCALELUT1_D2SCALE, pCfg->irSubDistScaleLut[2U]);
            CSL_FINS(regVal, PCID_IRSUBDISTSCALELUT1_D3SCALE, pCfg->irSubDistScaleLut[3U]);
            CSL_REG32_WR(&pcidRegs->IRSUBDISTSCALELUT1, regVal);

            regVal = CSL_REG32_RD(&pcidRegs->IRSUBDISTSCALELUT2);
            CSL_FINS(regVal, PCID_IRSUBDISTSCALELUT2_D4SCALE, pCfg->irSubDistScaleLut[4U]);
            CSL_REG32_WR(&pcidRegs->IRSUBDISTSCALELUT2, regVal);

            regVal = CSL_REG32_RD(&pcidRegs->IRSUBSCALE0);
            CSL_FINS(regVal, PCID_IRSUBSCALE0_SUBFACTSCALE00, pCfg->irSubFactScale[0U]);
            CSL_FINS(regVal, PCID_IRSUBSCALE0_SUBFACTSCALE01, pCfg->irSubFactScale[1U]);
            CSL_REG32_WR(&pcidRegs->IRSUBSCALE0, regVal);

            regVal = CSL_REG32_RD(&pcidRegs->IRSUBSCALE1);
            CSL_FINS(regVal, PCID_IRSUBSCALE1_SUBFACTSCALE10, pCfg->irSubFactScale[2U]);
            CSL_FINS(regVal, PCID_IRSUBSCALE1_SUBFACTSCALE11, pCfg->irSubFactScale[3U]);
            CSL_REG32_WR(&pcidRegs->IRSUBSCALE1, regVal);
        }
    }

    return (status);
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  Sets the common configuration to the RAW FE registers.
 */
int32_t CSL_pcidSetFrameConfig(CSL_pcidRegs *pcidRegs,
    const CslPcid_FrameConfig *cfg)
{
    int32_t  status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == cfg) || (NULL == pcidRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications CAN reach this portion. No existing test case can reach this portion. A test case will be added to cover this gap in a future release. 
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
        <justification end> */
        if ((CSL_PCID_FRAMESZ_HEIGHT_MAX < cfg->height) ||
            (CSL_PCID_FRAMESZ_WIDTH_MAX < cfg->width))
        {
            status = FVID2_EINVALID_PARAMS;
        }
        /* LDRA_JUSTIFY_END */
    }

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. This branch statement checks for errors accumulated in previous steps. Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&pcidRegs->FRAMESZ);
        CSL_FINS(regVal, PCID_FRAMESZ_HEIGHT, cfg->height-1);
        CSL_FINS(regVal, PCID_FRAMESZ_WIDTH, cfg->width-1);
        CSL_REG32_WR(&pcidRegs->FRAMESZ, regVal);
    }

    return (status);
}

/**
 *  Configures the PCID block
 */
int32_t CSL_pcidSetConfig(CSL_pcidRegs *pcidRegs,
                            CSL_irremap_lutRegs *pRemapLut,
                            const Pcid_Cfg *cfg)
{
    int32_t status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The parameters are pre-validated by the caller before the control reaches here. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == cfg) || (NULL == pcidRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        status = pcidCheckConfig(cfg);
    }

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. This branch statement checks for errors accumulated in previous steps. Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        status = pcidSetOpConfig(pcidRegs, &cfg->opChCfg);
    }

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. This branch statement checks for errors accumulated in previous steps. Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        status = pcidSetIrIntConfig(pcidRegs, cfg);
    }

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. This branch statement checks for errors accumulated in previous steps. Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        status = pcidSetIrSubConfig(pcidRegs, pRemapLut, &cfg->irSubCfg);
    }

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. This branch statement checks for errors accumulated in previous steps. Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached. 
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        /* Configure cfa format & enable/disable */
        /* PCID Enable Disable - Check in function CSL_vissTopPcidEnable () */
        regVal = CSL_REG32_RD(&pcidRegs->CTRL);
            CSL_FINS(regVal, PCID_CTRL_CFAFORMAT, cfg->cfaFormat);
        CSL_REG32_WR(&pcidRegs->CTRL, regVal);

    }

    return (status);
}
