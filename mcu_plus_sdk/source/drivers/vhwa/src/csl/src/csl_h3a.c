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
 */

/**
 *  \file csl_h3a.c
 *
 *  \brief CSL for h3a module, contains implementation of API,
 *         configuring h3a module.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <src/csl/include/csl_h3a.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief Minimun width for Window/Paxel
 */
#define CSL_H3A_WIN_MIN_WIDTH           (8U)

/**
 *  \brief Minimun height for Window/Paxel
 */
#define CSL_H3A_WIN_MIN_HEIGHT          (2U)

/**
 *  \brief Maximum height for Window/Paxel
 */
#define CSL_H3A_WIN_MAX_HEIGHT          (512U)

/**
 *  \brief Minimun horizontal count for Window/Paxel
 */
#define CSL_H3A_WIN_MIN_HCNT            (2U)

/**
 *  \brief Maximum horizontal count for Window/Paxel
 */
#define CSL_H3A_WIN_MAX_HCNT            (35U)

/**
 *  \brief Maximum vertical count for Window/Paxel
 */
#define CSL_H3A_WIN_MAX_VCNT            (127U)

/**
 *  \brief Minimun horizontal increment for Window/Paxel
 */
#define CSL_H3A_WIN_MIN_HINCR           (2U)
/**
 *  \brief Maximum horizontal increment for Window/Paxel
 */
#define CSL_H3A_WIN_MAX_HINCR           (32U)
/**
 *  \brief Minimun vertical increment for Window/Paxel
 */
#define CSL_H3A_WIN_MIN_VINCR           (2U)
/**
 *  \brief Maximum vertical increment for Window/Paxel
 */
#define CSL_H3A_WIN_MAX_VINCR           (32U)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Local function to check if given paxel/window
 *         configuratin is valid.
 *
 *  \param cfg              Pointer to H3a_PaxelConfig to be checked
 *                          This parameter should be non-NULL.
 *  \return                 FVID2_SOK on success, or FVID error code.
 */
static int32_t h3aCheckPaxelWinConfig(const H3a_PaxelConfig *cfg);

/**
 *  \brief Local function to set AF configuration in AF registers.
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param cfg              Pointer to H3a_AfConfig structure
 *                          This parameter should be non-NULL.
 *  \return                 Returns FVID2_SOK on success or FVID2 error code
 */
static int32_t h3aSetAfConfig(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AfConfig *cfg);

/**
 *  \brief Local function to set AEWB configuration in AEWB registers.
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param cfg              Pointer to H3a_AewbConfig structure
 *                          This parameter should be non-NULL.
 *  \return                 Returns FVID2_SOK on success or FVID2 error code
 */
static int32_t h3aSetAewbConfig(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AewbConfig *cfg);

/**
 *  \brief Local function to set IIR filter coefficients, required
 *         for AF in AF registers. It sets coefficinets for both the IIR
 *         filters.
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param iirCfg1          Coefficients for IIR filter1
 *                          This parameter should be non-NULL.
 *  \param iirCfg2          Coefficients for IIR filter2
 *                          This parameter should be non-NULL.
 */
static void h3aSetIirCoeff(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AfIirFiltConfig *iirCfg1,
    const H3a_AfIirFiltConfig *iirCfg2);

/**
 *  \brief Local function to set FIR filter coefficients, required
 *         for AF in AF registers. It sets coefficinets for both the FIR
 *         filters.
 *
 *  \param h3aRegs          H3A Register overlay
 *  \param firCfg1          Coefficients for FIR filter1
 *                          This parameter should be non-NULL.
 *  \param firCfg2          Coefficients for FIR filter2
 *                          This parameter should be non-NULL.
 */
static void h3aSetFirCoeff(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AfFirFilrConfig *firCfg1,
    const H3a_AfFirFilrConfig *firCfg2);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t CSL_h3aSetConfig(CSL_rawfe_h3a_cfgRegs *h3aRegs, const H3a_Config *cfg)
{
    int32_t             status = FVID2_SOK;
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
    if ((NULL == cfg) || (NULL == h3aRegs))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        if (H3A_MODULE_AF == cfg->module)
        {
            status = h3aSetAfConfig(h3aRegs, &cfg->afCfg);
        }
        else if (H3A_MODULE_AEWB == cfg->module)
        {
            status = h3aSetAewbConfig(h3aRegs, &cfg->aewbCfg);
        }
        else
        {
            status = FVID2_EINVALID_PARAMS;
        }

        if (FVID2_SOK == status)
        {
            regVal = CSL_REG32_RD(&h3aRegs->LINE_START);
            CSL_FINS(regVal, RAWFE_H3A_CFG_LINE_START_LINE_START,
                cfg->pos.startX);
            CSL_FINS(regVal, RAWFE_H3A_CFG_LINE_START_SLV,
                cfg->pos.startY);
            CSL_REG32_WR(&h3aRegs->LINE_START, regVal);
        }
    }

    return (status);
}

/**
 *  CSL_h3aStart
 *  \brief H3A Api for enabling/starting the module.
 *
 *  \param h3aRegs          Base Address of the H3A module
 *  \param module           Id of the Module to be started
 */
void CSL_h3aStart(CSL_rawfe_h3a_cfgRegs *h3aRegs, uint32_t module)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if (NULL != h3aRegs)
    /* LDRA_JUSTIFY_END */
    {
        if (H3A_MODULE_AF == module)
        {
            /* Enable AF engine, all the parameters must be set before
             * enabling this */
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if(UFALSE == CSL_h3aIsBusy(h3aRegs, H3A_MODULE_AF))
            /* LDRA_JUSTIFY_END */
            {
                CSL_REG32_FINS(&h3aRegs->PCR, RAWFE_H3A_CFG_PCR_AF_EN, 1U);
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
                /* If AF is already busy, do not enable it again */
                /* This is to avoid enabling the module again, if it is
                 * already enabled */
                GT_0trace(VhwaVissTrace, GT_ERR,
                        "AF module is already busy, cannot enable it again\n");
            }
            /* LDRA_JUSTIFY_END */
        }
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        else if (H3A_MODULE_AEWB == module)
        /* LDRA_JUSTIFY_END */
        {
            /* Enable AEWB engine, all the parameters must be set before
             * enabling this */
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if(UFALSE == CSL_h3aIsBusy(h3aRegs, H3A_MODULE_AEWB))
            /* LDRA_JUSTIFY_END */
            {
                CSL_REG32_FINS(&h3aRegs->PCR, RAWFE_H3A_CFG_PCR_AEW_EN, 1U);
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
                /* If AEWB is already busy, do not enable it again */
                /* This is to avoid enabling the module again, if it is
                 * already enabled */
                GT_0trace(VhwaVissTrace, GT_ERR,
                        "AEWB module is already busy, cannot enable it again\n");
            }
            /* LDRA_JUSTIFY_END */
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        else
        {
            /* Do nothing */
        }
        /* LDRA_JUSTIFY_END */
    }
}

/**
 *  CSL_h3aStop
 *  \brief H3A Api for stopping the module, when it is used in free
 *         running mode.
 *
 *  \param h3aRegs          Base Address of the H3A module
 *  \param module           Id of the Module to be stopped
 */
void CSL_h3aStop(CSL_rawfe_h3a_cfgRegs *h3aRegs, uint32_t module)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if (NULL != h3aRegs)
    /* LDRA_JUSTIFY_END */
    {
        if (H3A_MODULE_AF == module)
        {
            /* Enable AF engine, all the parameters must be set before
             * enabling this */
            CSL_REG32_FINS(&h3aRegs->PCR, RAWFE_H3A_CFG_PCR_AF_EN, 0U);
        }
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        else if (H3A_MODULE_AEWB == module)
        /* LDRA_JUSTIFY_END */
        {
            /* Enable AF engine, all the parameters must be set before
             * enabling this */
            CSL_REG32_FINS(&h3aRegs->PCR, RAWFE_H3A_CFG_PCR_AEW_EN, 0U);
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        else
        {
            /* Do nothing */
        }
        /* LDRA_JUSTIFY_END */
    }
}

/**
 *  CSL_h3aIsBusy
 *  \brief API to check if AF/AEWB module is busy or not.
 *
 *  \param h3aRegs          Base Address of the H3A module
 *  \param module           Id of the Module to be checked
 *
 *  \return                 Returns UTRUE if it is busy, or UFALSE
 */
uint32_t CSL_h3aIsBusy(const CSL_rawfe_h3a_cfgRegs *h3aRegs, uint32_t module)
{
    uint32_t          status = UFALSE;
    volatile uint32_t regVal;
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (NULL != h3aRegs)
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&h3aRegs->PCR);

        if (H3A_MODULE_AF == module)
        {
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if((regVal & CSL_RAWFE_H3A_CFG_PCR_BUSYAF_MASK) ==
                CSL_RAWFE_H3A_CFG_PCR_BUSYAF_MASK)
            {
                status = (uint32_t)UTRUE;
            }
            else
            /* LDRA_JUSTIFY_END */  
            {
                status = (uint32_t)UFALSE;
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
        else if (H3A_MODULE_AEWB == module)
        /* LDRA_JUSTIFY_END */ 
        {
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if((regVal & CSL_RAWFE_H3A_CFG_PCR_BUSYAEAWB_MASK) ==
                CSL_RAWFE_H3A_CFG_PCR_BUSYAEAWB_MASK)
            {
                status = (uint32_t)UTRUE;
            }
            else
            /* LDRA_JUSTIFY_END */  
            {
                status = (uint32_t)UFALSE;
            }
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement  <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        else
        {
            /* Do nothing */
        }
        /* LDRA_JUSTIFY_END */ 
    }

    return (status);
}

uint32_t CSL_h3aCalcAewbSize(const H3a_Config *cfg)
{
    uint32_t cnt;
    uint32_t hCnt;
    uint32_t vCnt;
    uint32_t numWins;
    uint32_t aewbSizeInBytes = 0u;
    uint32_t mode;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if (NULL != cfg)
    /* LDRA_JUSTIFY_END */
    {
        hCnt = cfg->aewbCfg.winCfg.horzCount;
        vCnt = cfg->aewbCfg.winCfg.vertCount;
        mode = cfg->aewbCfg.outMode;

        numWins = (vCnt + 1u) * hCnt;
        aewbSizeInBytes = 0u;

        for (cnt = 0u; cnt < numWins; cnt ++)
        {
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications CAN reach this portion.
                        No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
            Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                    However, due to the stated rationale, this is not tested.
            <justification end> */
            if ((mode == H3A_AEWB_OUTPUT_MODE_SUM_SQR) ||
                (mode == H3A_AEWB_OUTPUT_MODE_MIN_MAX))
            {
                aewbSizeInBytes += 8U;
            }
            else
            /* LDRA_JUSTIFY_END */
            {
                aewbSizeInBytes += 4U;
            }
            if ((cnt%8U) == 7U)
            {
                aewbSizeInBytes += 4U;
            }
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start> 
            Rationale: The component level negative test framework and test applications CAN reach this portion.
                        No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
            Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                    However, due to the stated rationale, this is not tested.
            <justification end> */            
            if (((cnt % hCnt) == (hCnt - 1u)) &&
                ((aewbSizeInBytes%8U) == 4U) &&
                (cnt != (numWins-1U)))
            {
                aewbSizeInBytes += 4U;
            }
            /* LDRA_JUSTIFY_END */
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */  
        if ((numWins%8u) != 0u)
        {
            aewbSizeInBytes += 4u;
        }
        if ((aewbSizeInBytes %8u) != 0u)
        {
            aewbSizeInBytes+= 4u;
        }
        /* LDRA_JUSTIFY_END */
    }

    return (aewbSizeInBytes * 4u);
}

uint32_t CSL_h3aCalcAfSize(const H3a_Config *cfg)
{
    uint32_t hCnt;
    uint32_t vCnt;
    uint32_t size;
    uint32_t afPad;

    hCnt = cfg->afCfg.paxelCfg.horzCount;
    vCnt = cfg->afCfg.paxelCfg.vertCount;

    if (H3A_AF_VF_VERT_HORZ == cfg->afCfg.vfMode)
    {
        size = 16u*4u*hCnt*vCnt;
    }
    else
    {
        afPad = 0u;
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */ 
        if (1u == (hCnt%2u))
        {
            afPad = 4u*(vCnt);
        }
        /* LDRA_JUSTIFY_END */
        size = ((12u*4u*hCnt*vCnt) + afPad);
    }

    return (size);
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

static int32_t h3aSetAfConfig(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AfConfig *cfg)
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
    if ((NULL == cfg) || (NULL == h3aRegs))
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
    if(FVID2_SOK == status)
    {
        /* Check for errors */
        status = h3aCheckPaxelWinConfig(&cfg->paxelCfg);
    }
    /* LDRA_JUSTIFY_END */

    if((FVID2_SOK == status) &&
       (cfg->paxelCfg.pos.startX < (cfg->iirFiltStartPos + 2U)))
    {
        status = FVID2_EINVALID_PARAMS;
    }

    if(FVID2_SOK == status)
    {
        if(H3A_AF_VF_VERT_HORZ == cfg->vfMode)
        {
            if(8U > cfg->paxelCfg.pos.startY)
            {
                status = FVID2_EINVALID_PARAMS;
            }

            if((cfg->paxelCfg.pos.startX & 0x1U) !=
               (cfg->iirFiltStartPos & 0x1U))
            {
                status = FVID2_EINVALID_PARAMS;
            }

            if((cfg->iirFiltStartPos%2U) != 0U)
            {
                status = FVID2_EINVALID_PARAMS;
            }
        }
        else
        {
            if(0U != (cfg->paxelCfg.pos.startX%2U))
            {
                status = FVID2_EINVALID_PARAMS;
            }
        }
    }

    if(FVID2_SOK == status)
    {
        /* Set the Window Configuration */
        regVal = CSL_REG32_RD(&h3aRegs->AFPAX1);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAX1_PAXW,
            ((cfg->paxelCfg.width / 2U) - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAX1_PAXH,
            ((cfg->paxelCfg.height / 2U) - 1U));
        CSL_REG32_WR(&h3aRegs->AFPAX1, regVal);

        regVal = CSL_REG32_RD(&h3aRegs->AFPAX2);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAX2_AFINCH,
            ((cfg->paxelCfg.horzIncr / 2U) - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAX2_AFINCV,
            ((cfg->paxelCfg.vertIncr / 2U) - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAX2_PAXHC,
            (cfg->paxelCfg.horzCount - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAX2_PAXVC,
            (cfg->paxelCfg.vertCount - 1U));
        CSL_REG32_WR(&h3aRegs->AFPAX2, regVal);

        regVal = CSL_REG32_RD(&h3aRegs->AFPAXSTART);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAXSTART_PAXSH,
            cfg->paxelCfg.pos.startX);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AFPAXSTART_PAXSV,
            cfg->paxelCfg.pos.startY);
        CSL_REG32_WR(&h3aRegs->AFPAXSTART, regVal);

        CSL_REG32_FINS(&h3aRegs->AFIIRSH, RAWFE_H3A_CFG_AFIIRSH_IIRSH,
            cfg->iirFiltStartPos);

        /* Set the IIR and FIR coefficients */
        h3aSetIirCoeff(h3aRegs, &cfg->iirCfg1, &cfg->iirCfg2);
        h3aSetFirCoeff(h3aRegs, &cfg->firCfg1, &cfg->firCfg2);

        regVal = CSL_REG32_RD(&h3aRegs->PCR);

        regVal &= ~CSL_RAWFE_H3A_CFG_PCR_AF_VF_EN_MASK;
        if(H3A_AF_VF_VERT_HORZ == cfg->vfMode)
        {
            regVal |= CSL_RAWFE_H3A_CFG_PCR_AF_VF_EN_MASK;
        }

        regVal &= ~CSL_RAWFE_H3A_CFG_PCR_FVMODE_MASK;
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */       
        if(H3A_AF_FV_MODE_PEAK == cfg->fvMode)
        {
            regVal |= CSL_RAWFE_H3A_CFG_PCR_FVMODE_MASK;
        }
        /* LDRA_JUSTIFY_END */ 
        regVal &= ~CSL_RAWFE_H3A_CFG_PCR_AF_ALAW_EN_MASK;
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */  
        if(UTRUE == cfg->enableALowCompr)
        {
            regVal |= CSL_RAWFE_H3A_CFG_PCR_AF_ALAW_EN_MASK;
        }
       /* LDRA_JUSTIFY_END */ 
        regVal &= ~CSL_RAWFE_H3A_CFG_PCR_AF_MED_EN_MASK;
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */ 
        if(UTRUE == cfg->enableMedFilt)
        /* LDRA_JUSTIFY_END */ 
        {
            regVal |= CSL_RAWFE_H3A_CFG_PCR_AF_MED_EN_MASK;

            CSL_FINS(regVal, RAWFE_H3A_CFG_PCR_MED_TH, cfg->midFiltThreshold);
        }
        CSL_FINS(regVal, RAWFE_H3A_CFG_PCR_RGBPOS, (uint32_t) cfg->rgbPos);

        CSL_REG32_WR(&h3aRegs->PCR, regVal);
    }

    return (status);
}

/* Set the AEWB Configuration */
static int32_t h3aSetAewbConfig(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AewbConfig *cfg)
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
    if ((NULL == cfg) || (NULL == h3aRegs))
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
        /* Check for errors */
        status = h3aCheckPaxelWinConfig(&cfg->winCfg);
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
        /* Set the output format and shift value for the saturation */
        regVal = CSL_REG32_RD(&h3aRegs->AEWCFG);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWCFG_AEFMT, cfg->outMode);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWCFG_SUMSHFT, cfg->sumShift);
        CSL_REG32_WR(&h3aRegs->AEWCFG, regVal);

        /* Set the Saturation limit and enable aLaw compression and
         * median filter */
        regVal = CSL_REG32_RD(&h3aRegs->PCR);

        CSL_FINS(regVal, RAWFE_H3A_CFG_PCR_AVE2LMT, cfg->satLimit);

        regVal &= ~CSL_RAWFE_H3A_CFG_PCR_AEW_MED_EN_MASK;
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */
        if(UTRUE == cfg->enableMedFilt)
        {
            regVal |= CSL_RAWFE_H3A_CFG_PCR_AEW_MED_EN_MASK;

            CSL_FINS(regVal, RAWFE_H3A_CFG_PCR_MED_TH, cfg->midFiltThreshold);
        }
        /* LDRA_JUSTIFY_END */
        regVal &=
            ~CSL_RAWFE_H3A_CFG_PCR_AEW_ALAW_EN_MASK;
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */
        if(UTRUE == cfg->enableALowComp)
        {
            regVal |=
                CSL_RAWFE_H3A_CFG_PCR_AEW_ALAW_EN_MASK;
        }
        /* LDRA_JUSTIFY_END */
        CSL_REG32_WR(&h3aRegs->PCR, regVal);

        /* Set the Window parameters */
        regVal = CSL_REG32_RD(&h3aRegs->AEWWIN1);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWWIN1_WINW,
            ((cfg->winCfg.width / 2U) - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWWIN1_WINH,
            ((cfg->winCfg.height / 2U) - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWWIN1_WINHC,
            (cfg->winCfg.horzCount - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWWIN1_WINVC,
            (cfg->winCfg.vertCount - 1U));
        CSL_REG32_WR(&h3aRegs->AEWWIN1, regVal);

        /* Set the Window Start Position */
        regVal = CSL_REG32_RD(&h3aRegs->AEWINSTART);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWINSTART_WINSH,
            cfg->winCfg.pos.startX);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWINSTART_WINSV,
            cfg->winCfg.pos.startY);
        CSL_REG32_WR(&h3aRegs->AEWINSTART, regVal);

        /* Set the black window start position */
        regVal = CSL_REG32_RD(&h3aRegs->AEWINBLK);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWINBLK_WINSV,
            cfg->blackLineVertStart);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWINBLK_WINH,
            ((cfg->blackLineHeight / 2U) - 1U));
        CSL_REG32_WR(&h3aRegs->AEWINBLK, regVal);

        /* Set the subwindow sampling point increment */
        regVal = CSL_REG32_RD(&h3aRegs->AEWSUBWIN);
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWSUBWIN_AEWINCH,
            ((cfg->winCfg.horzIncr / 2U) - 1U));
        CSL_FINS(regVal, RAWFE_H3A_CFG_AEWSUBWIN_AEWINCV,
            ((cfg->winCfg.vertIncr / 2U) - 1U));
        CSL_REG32_WR(&h3aRegs->AEWSUBWIN, regVal);
    }

    return (status);
}

static void h3aSetFirCoeff(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AfFirFilrConfig *firCfg1, const H3a_AfFirFilrConfig *firCfg2)
{
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if ((NULL != h3aRegs) && (NULL != firCfg1) && (NULL != firCfg2))
    /* LDRA_JUSTIFY_END */
    {
        /* Set the FIR coefficients */
        regVal = CSL_REG32_RD(&h3aRegs->VFV_CFG1);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG1_VCOEF1_0,
            (uint32_t)firCfg1->coeff[0U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG1_VCOEF1_1,
            (uint32_t)firCfg1->coeff[1U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG1_VCOEF1_2,
            (uint32_t)firCfg1->coeff[2U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG1_VCOEF1_3,
            (uint32_t)firCfg1->coeff[3U]);
        CSL_REG32_WR(&h3aRegs->VFV_CFG1, regVal);

        regVal = CSL_REG32_RD(&h3aRegs->VFV_CFG2);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG2_VCOEF1_4,
            (uint32_t)firCfg1->coeff[4U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG2_VTHR1, firCfg1->threshold);
        CSL_REG32_WR(&h3aRegs->VFV_CFG2, regVal);

        /* Set the FIR coefficients */
        regVal = CSL_REG32_RD(&h3aRegs->VFV_CFG3);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG3_VCOEF2_0,
            (uint32_t)firCfg2->coeff[0U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG3_VCOEF2_1,
            (uint32_t)firCfg2->coeff[1U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG3_VCOEF2_2,
            (uint32_t)firCfg2->coeff[2U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG3_VCOEF2_3,
            (uint32_t)firCfg2->coeff[3U]);
        CSL_REG32_WR(&h3aRegs->VFV_CFG3, regVal);

        regVal = CSL_REG32_RD(&h3aRegs->VFV_CFG4);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG4_VCOEF2_4,
            (uint32_t)firCfg2->coeff[4U]);
        CSL_FINS(regVal, RAWFE_H3A_CFG_VFV_CFG4_VTHR2, firCfg2->threshold);
        CSL_REG32_WR(&h3aRegs->VFV_CFG4, regVal);
    }
}

static void h3aSetIirCoeff(CSL_rawfe_h3a_cfgRegs *h3aRegs,
    const H3a_AfIirFiltConfig *iirCfg1, const H3a_AfIirFiltConfig *iirCfg2)
{
    uint32_t                   cnt;
    uint32_t                   iirCnt;
    volatile uint32_t         *coeffRegAddr = NULL;
    volatile uint32_t          regVal;
    const H3a_AfIirFiltConfig *tempIirCfg = NULL;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
	Rationale: The component level negative test framework and test applications cannot reach this portion.
	The parameters are pre-validated by the caller before the control reaches here.
	Effect on this unit: If the control reaches here, our code base is NOT expected to accumulate the error status.
	An else block can be added to catch and print the error status in a future release. 
	<justification end> */
    if ((NULL != h3aRegs) && (NULL != iirCfg1) && (NULL != iirCfg2))
    /* LDRA_JUSTIFY_END */
    {

        for(cnt = 0U; cnt < 2U; cnt++)
        {
            if(0U == cnt)
            {
                coeffRegAddr = &(h3aRegs->AFCOEF010);
                tempIirCfg   = iirCfg1;
            }
            else
            {
                coeffRegAddr = &(h3aRegs->AFCOEF110);
                tempIirCfg   = iirCfg2;
            }

            for(iirCnt = 0U; iirCnt < H3A_AF_IIR_COEFF_MAX; iirCnt += 2U)
            {
                regVal = CSL_REG32_RD(coeffRegAddr);
                CSL_FINS(regVal, RAWFE_H3A_CFG_AFCOEF010_COEFF0,
                    (uint32_t)tempIirCfg->coeff[iirCnt]);

                if(iirCnt < (H3A_AF_IIR_COEFF_MAX - 2U))
                {
                    CSL_FINS(regVal, RAWFE_H3A_CFG_AFCOEF010_COEFF1,
                        (uint32_t)tempIirCfg->coeff[iirCnt + 1u]);
                }

                CSL_REG32_WR(coeffRegAddr, regVal);

                coeffRegAddr ++;
            }
        }

        /* Set the IIR coefficients Threshold */
        regVal = CSL_REG32_RD(&h3aRegs->HVF_THR);
        CSL_FINS(regVal, RAWFE_H3A_CFG_HVF_THR_HTHR1, iirCfg1->threshold);
        CSL_FINS(regVal, RAWFE_H3A_CFG_HVF_THR_HTHR2, iirCfg2->threshold);
        CSL_REG32_WR(&h3aRegs->HVF_THR, regVal);
    }
}

static int32_t h3aCheckPaxelWinConfig(const H3a_PaxelConfig *cfg)
{
    int32_t status = FVID2_SOK;

    if((cfg->width < CSL_H3A_WIN_MIN_WIDTH) ||
       (cfg->height < CSL_H3A_WIN_MIN_HEIGHT) ||
       (cfg->height > CSL_H3A_WIN_MAX_HEIGHT))
    {
        status = FVID2_EINVALID_PARAMS;
    }

    if((cfg->horzCount < CSL_H3A_WIN_MIN_HCNT) ||
       (cfg->horzCount > CSL_H3A_WIN_MAX_HCNT) ||
       (cfg->vertCount > CSL_H3A_WIN_MAX_VCNT))
    {
        status = FVID2_EINVALID_PARAMS;
    }

    if((cfg->horzIncr < CSL_H3A_WIN_MIN_HINCR) ||
       (cfg->horzIncr > CSL_H3A_WIN_MAX_HINCR) ||
       (cfg->vertIncr < CSL_H3A_WIN_MIN_VINCR) ||
       (cfg->vertIncr > CSL_H3A_WIN_MAX_VINCR))
    {
        status = FVID2_EINVALID_PARAMS;
    }

    if(((cfg->width % 2U) == 1u) ||
       ((cfg->height % 2U) == 1u) ||
       ((cfg->horzIncr % 2U) == 1u) ||
       ((cfg->vertIncr % 2U) == 1u))
    {
        status = FVID2_EINVALID_PARAMS;
    }

    return (status);
}

