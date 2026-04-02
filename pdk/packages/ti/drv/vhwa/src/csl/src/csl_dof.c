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
 *
 */

/**
 *  \file csl_dof.c
 *
 *  \brief DOF CSL file, contains implementation of API, configuring DOF module.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <src/csl/include/csl_dof.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  \brief  Function to check configuration, checks all parameters of
 *          dof config and returns status.
 *
 *  \param cfg          Pointer to Dof_Config structure
 *                      This parameter should be non-NULL.
 *  \return             Returns 0 on success else returns error value
 */
static int32_t CSL_dofCheckCfg(const Dof_Config *cfg);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 *  \brief Sets the entire DOF configuration to the DOF registers.
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param config           Pointer to #Dof_Config structure
 *                          containing the register configurations.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofSetConfig(CSL_dmpac_dofRegs           *dofRegs,
                         const Dof_Config            *cfg)

{
    int32_t  status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> Rationale:The negative test framework and test apps cannot reach this portion.
    To make this if statement true, either socInfo→dofRegs or &hObj→dofCfg[pyrLvl] needs to be NULL.
    However, no test config will result in this.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == dofRegs) || (NULL == cfg))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        /* Check for Error */
        status = CSL_dofCheckCfg(cfg);
    }
    if (FVID2_SOK == status)
    {
        regVal = CSL_REG32_RD(&dofRegs->RES);
        CSL_FINS(
          regVal,
          DMPAC_DOF_RES_WIDTH_CFG,
          cfg->width);
        CSL_FINS(
          regVal,
          DMPAC_DOF_RES_HEIGHT_CFG,
          cfg->height);
        CSL_REG32_WR(&dofRegs->RES, regVal);

        regVal = CSL_REG32_RD(&dofRegs->SR);
        CSL_FINS(
          regVal,
          DMPAC_DOF_SR_HSR_CFG,
          cfg->horizontalSearchRange);
        CSL_FINS(
          regVal,
          DMPAC_DOF_SR_VSR_P_CFG,
          cfg->bottomSearchRange);
        CSL_FINS(
          regVal,
          DMPAC_DOF_SR_VSR_N_CFG,
          cfg->topSearchRange);
        CSL_REG32_WR(&dofRegs->SR, regVal);

        regVal = CSL_REG32_RD(&dofRegs->CR);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_DOF_EN_CFG,
          cfg->enableDOF);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_PYC_EN_CFG,
          cfg->pyramidalTopColPred);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_PYL_EN_CFG,
          cfg->pyramidalTopLeftPred);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_TP_EN_CFG,
          cfg->temporalPred);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_DL_EN_CFG,
          cfg->delayedLeftPred);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_LK_CS_EN_CFG,
          cfg->lkConfidanceScore);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_MF_EN_CFG,
          cfg->medianFilter);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_SOFSPARSEEN_CFG,
          cfg->enableSof);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_CUR_CT_TYPE_CFG,
          cfg->currentCensusTransform);
        CSL_FINS(
          regVal,
          DMPAC_DOF_CR_REF_CT_TYPE_CFG,
          cfg->referenceCensusTransform);
        CSL_REG32_WR(&dofRegs->CR, regVal);

        CSL_REG32_FINS(&dofRegs->SOF,
          DMPAC_DOF_SOF_MAX_OUTPUT_COUNT_PER_LINE_CFG,
          (uint32_t)cfg->maxMVsInSof);

        CSL_REG32_FINS(&dofRegs->MSFR,
          DMPAC_DOF_MSFR_MSF_CFG,
          (uint32_t)cfg->motionSmoothnessFactor);

        CSL_REG32_FINS(&dofRegs->CSCFGR,
          DMPAC_DOF_CSCFGR_IIR_ALPHA_CFG,
          (uint32_t)cfg->iirFilterAlpha);
    }

    return (status);

}


/**
 *  \brief API for setting DOF Confidance score params
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param csParam          Pointer to the structure containing
 *                          Confidance score params
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofSetConfScoreParam(CSL_dmpac_dofRegs               *dofRegs,
                                 const Dof_ConfScoreParam        *csParam)
{
    int32_t  status = FVID2_SOK;
    volatile uint32_t regVal;
    uint32_t i = 0;
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> Rationale: The test framework and test apps cannot reach this portion. dofreg and csParam are never NULL, dofreg has default base address (vhwa_dmpac_soc.c) and csParam is predefined in vhwa_m2mDofControl
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == dofRegs) || (NULL == csParam))
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
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {

        CSL_REG32_FINS(&dofRegs->CSCFGR,
          DMPAC_DOF_CSCFGR_CS_GAIN_CFG,
          (uint32_t)csParam->confidanceScoreGain);
        for(i = 0U ; i < DOF_NUM_DECISION_TREES; i++)
        {
            CSL_REG32_FINS(&dofRegs->TREE[i].TH0,
              DMPAC_DOF_TREE_TH0_THRESHOLD_CFG,
              (uint32_t)csParam->decisionTree[i].threshold[0]);
            CSL_REG32_FINS(&dofRegs->TREE[i].TH10,
              DMPAC_DOF_TREE_TH10_THRESHOLD_CFG,
              (uint32_t)csParam->decisionTree[i].threshold[1]);
            CSL_REG32_FINS(&dofRegs->TREE[i].TH11,
              DMPAC_DOF_TREE_TH11_THRESHOLD_CFG,
              (uint32_t)csParam->decisionTree[i].threshold[2]);

            CSL_REG32_FINS(&dofRegs->TREE[i].WT00,
              DMPAC_DOF_TREE_WT00_WEIGHT_CFG,
              (uint32_t)csParam->decisionTree[i].weight[0]);
            CSL_REG32_FINS(&dofRegs->TREE[i].WT01,
              DMPAC_DOF_TREE_WT00_WEIGHT_CFG,
              (uint32_t)csParam->decisionTree[i].weight[1]);
            CSL_REG32_FINS(&dofRegs->TREE[i].WT10,
              DMPAC_DOF_TREE_WT00_WEIGHT_CFG,
              (uint32_t)csParam->decisionTree[i].weight[2]);
            CSL_REG32_FINS(&dofRegs->TREE[i].WT11,
              DMPAC_DOF_TREE_WT00_WEIGHT_CFG,
              (uint32_t)csParam->decisionTree[i].weight[3]);

            regVal = CSL_REG32_RD(&dofRegs->TREE[i].FIDS);
            CSL_FINS(
              regVal,
              DMPAC_DOF_TREE_FIDS_INDEX0_CFG,
              csParam->decisionTree[i].index[0]);
            CSL_FINS(
              regVal,
              DMPAC_DOF_TREE_FIDS_INDEX1_CFG,
              csParam->decisionTree[i].index[1]);
            CSL_FINS(
              regVal,
              DMPAC_DOF_TREE_FIDS_INDEX2_CFG,
              csParam->decisionTree[i].index[2]);
            CSL_REG32_WR(&dofRegs->TREE[i].FIDS, regVal);
        }
    }

    return (status);
}

/**
 *  \brief API for setting DOF SL2 Buffer and image params
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param bufParam         Pointer to the structure containing
 *                          SL2 buffer and Image Parameters
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofSetBufParam(CSL_dmpac_dofRegs           *dofRegs,
                           const CSL_DofBufParams      *bufParam)
{
    int32_t  status = FVID2_SOK;
    volatile uint32_t regVal;
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The test framework and test apps cannot reach this portion. dofreg and bufParam never be NULL,dofreg has default base address(vhwa_dmpac_soc.c)
    and bufParam is predefined in vhwa_m2mDofControl
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == dofRegs) || (NULL == bufParam))
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
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        CSL_REG32_FINS(&dofRegs->CFGWBASE,
          DMPAC_DOF_CFGWBASE_ADDR_CFG,
          (uint32_t)bufParam->currImageAddress);
        CSL_REG32_FINS(&dofRegs->CFGWWIDTH,
          DMPAC_DOF_CFGWWIDTH_WIDTH_CFG,
          (uint32_t)bufParam->currBufWidth);
        CSL_REG32_FINS(&dofRegs->CFGWHEIGHT,
          DMPAC_DOF_CFGWHEIGHT_HEIGHT_CFG,
          (uint32_t)bufParam->currBufHeight);
        CSL_REG32_FINS(&dofRegs->RFGWBASE,
          DMPAC_DOF_RFGWBASE_ADDR_CFG,
          (uint32_t)bufParam->refImageAddress);
        CSL_REG32_FINS(&dofRegs->RFGWWIDTH,
          DMPAC_DOF_RFGWWIDTH_WIDTH_CFG,
          (uint32_t)bufParam->refBufWidth);
        CSL_REG32_FINS(&dofRegs->RFGWHEIGHT,
          DMPAC_DOF_RFGWHEIGHT_HEIGHT_CFG,
          (uint32_t)bufParam->refBufHeight);

        regVal = CSL_REG32_RD(&dofRegs->SPBUFBASE);
        CSL_FINS(
          regVal,
          DMPAC_DOF_SPBUFBASE_ADDR_CFG,
          bufParam->spatialPredAddress);
        CSL_FINS(
          regVal,
          DMPAC_DOF_SPBUFBASE_DEPTH_CFG,
          bufParam->spatialPredDepth);
        CSL_REG32_WR(&dofRegs->SPBUFBASE, regVal);

        regVal = CSL_REG32_RD(&dofRegs->TPBUFBASE);
        CSL_FINS(
          regVal,
          DMPAC_DOF_TPBUFBASE_ADDR_CFG,
          bufParam->temporalPredAddress);
        CSL_FINS(
          regVal,
          DMPAC_DOF_TPBUFBASE_DEPTH_CFG,
          bufParam->temporalPredDepth);
        CSL_REG32_WR(&dofRegs->TPBUFBASE, regVal);

        regVal = CSL_REG32_RD(&dofRegs->BMBUFBASE);
        CSL_FINS(
          regVal,
          DMPAC_DOF_BMBUFBASE_ADDR_CFG,
          bufParam->binaryMapAddress);
        CSL_FINS(
          regVal,
          DMPAC_DOF_BMBUFBASE_DEPTH_CFG,
          bufParam->binaryMapDepth);
        CSL_REG32_WR(&dofRegs->BMBUFBASE, regVal);

        regVal = CSL_REG32_RD(&dofRegs->FVBUFBASE);
        CSL_FINS(
          regVal,
          DMPAC_DOF_FVBUFBASE_ADDR_CFG,
          bufParam->outFlowAddress);
        CSL_FINS(
          regVal,
          DMPAC_DOF_FVBUFBASE_DEPTH_CFG,
          bufParam->outFlowpDepth);
        CSL_REG32_WR(&dofRegs->FVBUFBASE, regVal);
    }

    return (status);
}



/**
 *  \brief API for getting the CS histogram of the output flow vector.
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param csHistogram      Pointer to the memory where Confidence Score
 *                          Histogram will be stored
 *  \param arg              Returns 0 on success else returns error value
 */
int32_t CSL_dofGetCsHistogram(const CSL_dmpac_dofRegs  *dofRegs,
                              uint32_t           *csHistogram)
{
    int32_t  status = FVID2_SOK;
    uint32_t idx;
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> Rationale: The test framework and test apps cannot reach this portion. dofreg  never be NULL,dofreg has default base address(vhwa_dmpac_soc.c)
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == dofRegs))
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
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        for(idx = 0u; idx < DOF_MAX_HISTOGRAM_SIZE; idx++)
        {
            csHistogram[idx] = CSL_REG32_RD(&dofRegs->DOFCSHIST[idx]);
            csHistogram[idx] = csHistogram[idx] &
                               CSL_DMPAC_DOF_DOFCSHIST_BIN_STS_MASK;
        }
    }
    return (status);
}

/**
 *  \brief API for enabling the PSA signature generation for flow vector
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param enable           Flag to enable/disable the PSA
 *
 *  \return                 None
 */
void CSL_dofEnablePsa(CSL_dmpac_dofRegs       *dofRegs,
                      uint32_t                 enable)
{
    CSL_REG32_FINS(&dofRegs->PSA_CTRL,
          DMPAC_DOF_PSA_CTRL_PSA_EN_CFG,
          enable);
}

/**
 *  \brief API for getting the PSA signature of the output flow vector
 *
 *  \param dofRegs          Pointer to structure containing DOF Base Address
 *  \param psaSig           Pointer to the memory where crc for the flow
 *                          vector will be stored
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_dofGetPsa(const CSL_dmpac_dofRegs           *dofRegs,
                      uint32_t                    *psaSig)
{
    int32_t  status = FVID2_SOK;
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The test framework and test apps cannot reach this portion. dofreg and psaSig never be NULL,dofreg has default base address(vhwa_dmpac_soc.c) and psaSig is pre-validated in vhwa_m2mDofControl
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == dofRegs) || (NULL == psaSig))
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
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        *psaSig = CSL_REG32_RD(&dofRegs->PSA_SIGNATURE);
    }

    return (status);
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */
/**
 *  \brief  Function to check configuration, checks all parameters of
 *          dof config and returns status.
 *
 *  \param cfg          Pointer to Dof_Config structure
 *                      This parameter should be non-NULL.
 *  \return             Returns 0 on success else returns error value
 */
static int32_t CSL_dofCheckCfg(const Dof_Config *cfg)
{
    int32_t status = FVID2_SOK;
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start> Rationale: The test framework and test apps cannot reach this portion. cfg never be NULL,pre-validated from CSL_dofSetConfig
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(NULL == cfg)
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        if(0U == cfg->iirFilterAlpha)
        {
            /* iirFilterAlpha is out of supported range */
            status = FVID2_EBADARGS;
        }
    }

    return (status);
}
