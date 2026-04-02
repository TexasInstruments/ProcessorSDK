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

static uint32_t vissTopGetMuxValue(uint32_t    inSelect);

static uint32_t vissTopGetSatMuxValue(uint32_t    inSelect);

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

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start>
Rationale: The test framework and test apps cannot reach this portion
The function is unused because of an un-implemented debug feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach to the false condition.
<justification end> */ 
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
/* LDRA_JUSTIFY_END */

/**
 *  Function sets GLBCE delay configuration
 */

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start>
Rationale: The test framework and test apps cannot reach this portion
The function is unused because of an un-implemented debug feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach to the false condition.
<justification end> */ 
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
/* LDRA_JUSTIFY_END */

/**
 *  Function to enable/disable GLBCE module
 */
void CSL_vissTopGlbceEnable(CSL_viss_topRegs *vissTopRegs, uint32_t enable)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
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
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
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
 *  Function to enable/disable CAC module
 */
void CSL_vissTopCacEnable(CSL_viss_topRegs *vissTopRegs, uint32_t enable)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)UTRUE == enable)
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_CAC_EN, 1U);
        }
        else
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_CAC_EN, 0U);
        }
    }
}

#if defined (VHWA_VPAC_IP_REV_VPAC3L)
/**
 *  Function to enable/disable PCID block
 */
void CSL_vissTopPcidEnable(CSL_viss_topRegs *vissTopRegs, uint32_t enable)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)TRUE == enable)
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_PCID_EN, 1U);
        }
        else
        {
            CSL_REG32_FINS(&vissTopRegs->VISS_CNTL,
                VISS_TOP_VISS_CNTL_PCID_EN, 0U);
        }
    }
}
#endif

/**
 *  Function to enable/disable GLBCE clock to free running
 */
void CSL_vissTopGlbceFreeRunning(CSL_viss_topRegs *vissTopRegs,
    uint32_t enable)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
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

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
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

void CSL_vissTopMuxConfig(CSL_viss_topRegs *vissTopRegs,
                            const CslViss_OutputSelect *mux)
{
    volatile uint32_t   regVal;
    uint32_t            muxVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if ((NULL != vissTopRegs) && (NULL != mux))
    /* LDRA_JUSTIFY_END */
    {
        regVal = 0;

        muxVal = vissTopGetMuxValue(mux->luma12OutSel);
        CSL_FINS(regVal, VISS_TOP_LSEOUT_MUX_CNTL_Y12SEL, muxVal);
        muxVal = vissTopGetMuxValue(mux->chroma12OutSel);
        CSL_FINS(regVal, VISS_TOP_LSEOUT_MUX_CNTL_UV12SEL, muxVal);
        muxVal = vissTopGetMuxValue(mux->luma8OutSel);
        CSL_FINS(regVal, VISS_TOP_LSEOUT_MUX_CNTL_Y8SEL, muxVal);
        muxVal = vissTopGetMuxValue(mux->chroma8OutSel);
        CSL_FINS(regVal, VISS_TOP_LSEOUT_MUX_CNTL_UV8SEL, muxVal);
        muxVal = vissTopGetSatMuxValue(mux->sat8OutSel);
        CSL_FINS(regVal, VISS_TOP_LSEOUT_MUX_CNTL_S8SEL, muxVal);

        CSL_REG32_WR(&vissTopRegs->LSEOUT_MUX_CNTL, regVal);
    }
}

#if !defined(VHWA_VPAC_IP_REV_VPAC3L)

void CSL_vissTopFcp2Enable(CSL_viss_topRegs *vissTopRegs,
                            uint32_t    enable)
{   
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
    {
        if ((uint32_t)UTRUE == enable)
        {
            CSL_REG32_FINS(&vissTopRegs->FCP2_CNTL,
                VISS_TOP_FCP2_CNTL_PIXCLK_EN, 1U);
        }
        else
        {
            CSL_REG32_FINS(&vissTopRegs->FCP2_CNTL,
                VISS_TOP_FCP2_CNTL_PIXCLK_EN, 0U);
        }
    }
}

void CSL_vissTopFcp2InSelect(CSL_viss_topRegs *vissTopRegs,
                            uint32_t    inSelect,
                            uint32_t    pipeDelay)
{
    volatile uint32_t  regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The parameters are pre-validated by the caller before the control reaches here.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&vissTopRegs->FCP2_CNTL);

        CSL_FINS(regVal,
                VISS_TOP_FCP2_CNTL_IN_PIPEDLY,
				pipeDelay);

        CSL_FINS(regVal, VISS_TOP_FCP2_CNTL_IN_SEL,
                inSelect);

        CSL_REG32_WR(&vissTopRegs->FCP2_CNTL, regVal);
    }
}
#endif

static uint32_t vissTopGetMuxValue(uint32_t    inSelect)
{
    uint32_t retVal = 0;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The test framework and test apps cannot reach this portion
    The function is unused because of an un-implemented debug feature, this code base is not being used/enabled with the current kernel support.
    Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach to the false condition.
    <justification end> */
    switch (inSelect)
    /* LDRA_JUSTIFY_END */
    {
        case VISS_OUT_FCP_Y12:
        case VISS_OUT_FCP_UV12:
        {
            retVal = 0;
        }
        break;
        case VISS_OUT_FCP2_Y12:
        case VISS_OUT_FCP2_UV12:
        {
            retVal = 1;
        }
        break;
        case VISS_OUT_FCP_Y8:
        case VISS_OUT_FCP_UV8:
        {
            retVal = 2;
        }
        break;
        case VISS_OUT_FCP2_Y8:
        case VISS_OUT_FCP2_UV8:
        {
            retVal = 3;
        }
        break;
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The test framework and test apps cannot reach this portion
        The function is unused because of an un-implemented debug feature, this code base is not being used/enabled with the current kernel support.
        Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach to the false condition.
        <justification end> */
        case VISS_OUT_FCP_S8:
        {
            retVal = 0;
        }
        break;
        case VISS_OUT_FCP2_S8:
        {
            retVal = 1;
        }
        break;
        /* LDRA_JUSTIFY_END */
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
        case VISS_OUT_IR_8:
        {
            retVal = 4;
        }
        break;
        case VISS_OUT_IR_12:
        {
            retVal = 4;
        }
        break;
        case VISS_OUT_IR_16:
        {
            retVal = 5;
        }
        break;
#endif
        default:
        {
          /*Do Nothing*/
        }
        break;
    }

    return retVal;
}

static uint32_t vissTopGetSatMuxValue(uint32_t    inSelect)
{
    uint32_t retVal = 0;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The test framework and test apps cannot reach this portion
    The function is unused because of an un-implemented debug feature, this code base is not being used/enabled with the current kernel support.
    Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach to the false condition.
    <justification end> */
    switch (inSelect)
    /* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The test framework and test apps cannot reach this portion
        The function is unused because of an un-implemented debug feature, this code base is not being used/enabled with the current kernel support.
        Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach to the false condition.
        <justification end> */
        case VISS_OUT_FCP_Y12:
        case VISS_OUT_FCP_UV12:
        {
            retVal = 0;
        }
        break;
        case VISS_OUT_FCP2_Y12:
        case VISS_OUT_FCP2_UV12:
        {
            retVal = 1;
        }
        break;
        case VISS_OUT_FCP_Y8:
        case VISS_OUT_FCP_UV8:
        {
            retVal = 2;
        }
        break;
        case VISS_OUT_FCP2_Y8:
        case VISS_OUT_FCP2_UV8:
        {
            retVal = 3;
        }
        break;
        /* LDRA_JUSTIFY_END */
        case VISS_OUT_FCP_S8:
        {
            retVal = 0;
        }
        break;
        case VISS_OUT_FCP2_S8:
        {
            retVal = 1;
        }
        break;
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
        case VISS_OUT_IR_8:
        {
            retVal = 2;
        }
        break;
        case VISS_OUT_IR_12:
        {
            retVal = 4;
        }
        break;
        case VISS_OUT_IR_16:
        {
            retVal = 5;
        }
        break;
#endif
        default:
        {
          /*Do Nothing*/
        }
        break;
    }

    return retVal;
}

#if defined (VHWA_VPAC_IP_REV_VPAC3L)
void CSL_pcidSetIrCntl(CSL_viss_topRegs *vissTopRegs,
                            uint32_t irDelay,
                            uint32_t dwidth)
{
    volatile uint32_t  regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application. This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != vissTopRegs)
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&vissTopRegs->IROUT_CNTL);
        CSL_FINS(regVal, VISS_TOP_IROUT_CNTL_PIPEDLY, irDelay);
        CSL_FINS(regVal, VISS_TOP_IROUT_CNTL_DWIDTH, dwidth);
        CSL_REG32_WR(&vissTopRegs->IROUT_CNTL, regVal);
    }

    return;
}
#endif
