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
 *  \file csl_sde.c
 *
 *  \brief  SDE CSL file, contains implementation of API, configuring NF module.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <src/csl/include/csl_sde.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  \brief Checks for the valdity of Sde_Config parameters set
 *        by the user
 *
 *  \param cfg          Pointer to Sde_Config structure
 *                      This parameter should be non-NULL.
 *  \return             Returns 0 on success else returns error value
 */
static int32_t CSL_sdeCheckCfg(const Sde_Config *cfg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  \brief Sets the entire SDE configuration to the SDE registers.
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param config           Pointer to Sde_Config structure
 *                          containing the register configurations.
 *                          This parameter should be non-NULL.
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_sdeSetConfig(CSL_dmpac_sdeRegs            *sdeRegs,
                         const Sde_Config             *cfg)
{
    int32_t  status = FVID2_SOK;
    volatile uint32_t regVal;
    uint32_t i;
    volatile uint32_t *pConfScore;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The negative test framework and test apps cannot reach this portion.
    No test configuration will result in NULL arguments from the caller.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == sdeRegs) || (NULL == cfg))
    {
        status = FVID2_EBADARGS;
    }
    else
    /* LDRA_JUSTIFY_END */
    {
        /* Check for Error */
        status = CSL_sdeCheckCfg(cfg);
    }

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        regVal = CSL_REG32_RD(&sdeRegs->CTRL);
        CSL_FINS(
          regVal,
          DMPAC_SDE_CTRL_SDEEN,
          cfg->enableSDE);
        CSL_FINS(
          regVal,
          DMPAC_SDE_CTRL_MEDFEN,
          cfg->medianFilter);
        CSL_FINS(
          regVal,
          DMPAC_SDE_CTRL_RRSRCHEN,
          1U);
        CSL_REG32_WR(&sdeRegs->CTRL, regVal);

        regVal = CSL_REG32_RD(&sdeRegs->IMGRES);
        CSL_FINS(
          regVal,
          DMPAC_SDE_IMGRES_IHINC,
          ((cfg->height - 64u)/16u));
        CSL_FINS(
          regVal,
          DMPAC_SDE_IMGRES_IWINC,
          ((cfg->width-128u)/16u));
        CSL_REG32_WR(&sdeRegs->IMGRES, regVal);

        regVal = CSL_REG32_RD(&sdeRegs->SRCHRNG);
        CSL_FINS(
          regVal,
          DMPAC_SDE_SRCHRNG_CFGDMIN,
          cfg->minDisparity);
        CSL_FINS(
          regVal,
          DMPAC_SDE_SRCHRNG_CFGDRANGE,
          cfg->searchRange);
        CSL_REG32_WR(&sdeRegs->SRCHRNG, regVal);

        CSL_REG32_FINS(&sdeRegs->LRCHCK,
          DMPAC_SDE_LRCHCK_DIFFTHLD,
          (uint32_t)cfg->lrThreshold);

        regVal = CSL_REG32_RD(&sdeRegs->TXTFLT);
        CSL_FINS(
          regVal,
          DMPAC_SDE_TXTFLT_TXTFLTEN,
          cfg->enableTextureFilter);
        CSL_FINS(
          regVal,
          DMPAC_SDE_TXTFLT_TXTTHLD,
          cfg->textureFilterThreshold);
        CSL_REG32_WR(&sdeRegs->TXTFLT, regVal);

        regVal = CSL_REG32_RD(&sdeRegs->PNLTY);
        CSL_FINS(
          regVal,
          DMPAC_SDE_PNLTY_P1,
          cfg->penaltyP1);
        CSL_FINS(
          regVal,
          DMPAC_SDE_PNLTY_P2,
          cfg->penaltyP2);
        CSL_REG32_WR(&sdeRegs->PNLTY, regVal);

        pConfScore = &sdeRegs->CONFMAPG0;

        for(i = 0u; i < DMPAC_SDE_NUM_SCORE_MAP; i+=2u)
        {
          regVal = CSL_REG32_RD(pConfScore);
          CSL_FINS(
            regVal,
            DMPAC_SDE_CONFMAPG0_CONFMAP_0,
            cfg->confScoreMap[i]);
          /* Range of confidence score map supported is 0 -> 6 */
          if(i < (DMPAC_SDE_NUM_SCORE_MAP - 2U))
          {
            CSL_FINS(
              regVal,
              DMPAC_SDE_CONFMAPG0_CONFMAP_1,
              cfg->confScoreMap[i+1U]);
          }
          else
          {
            /* Writing 0 value for confidence score map idx 1 of CONFMAPG3,
            *  since it is a rsvd field */
            CSL_FINS(
              regVal,
              DMPAC_SDE_CONFMAPG0_CONFMAP_1,
              0U);
          }
          CSL_REG32_WR(pConfScore, regVal);
          pConfScore += 1U;
        }

    }

    return (status);

}


/**
 *  \brief API for setting SDE SL2 Buffer parameters
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param bufParam         Pointer to the structure containing
 *                          SL2 buffer and Image Parameters
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_sdeSetBufParam(CSL_dmpac_sdeRegs            *sdeRegs,
                           const CSL_SdeBufParams       *bufParam)
{
    int32_t  status = FVID2_SOK;
    volatile uint32_t regVal;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The negative test framework and test apps cannot reach this portion.
    No test configuration will result in NULL arguments from the caller.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == sdeRegs) || (NULL == bufParam))
    {
        status = FVID2_EBADARGS;
    }

    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {

        CSL_REG32_FINS(&sdeRegs->BASEIMGADDR,
          DMPAC_SDE_BASEIMGADDR_BASEAD,
          (uint32_t)bufParam->baseImageAddress);
        CSL_REG32_FINS(&sdeRegs->BASEIMGWD,
          DMPAC_SDE_BASEIMGWD_BWIDTH,
          (uint32_t)bufParam->baseImageWidth);
        CSL_REG32_FINS(&sdeRegs->REFIMGADDR,
          DMPAC_SDE_REFIMGADDR_REFAD,
          (uint32_t)bufParam->refImageAddress);
        CSL_REG32_FINS(&sdeRegs->REFIMGWD,
          DMPAC_SDE_REFIMGWD_RWIDTH,
          (uint32_t)bufParam->refImageWidth);

        regVal = CSL_REG32_RD(&sdeRegs->DISPBUFCFG);
        CSL_FINS(
          regVal,
          DMPAC_SDE_DISPBUFCFG_DISPBUFAD,
          bufParam->disparityAddress);
        CSL_FINS(
          regVal,
          DMPAC_SDE_DISPBUFCFG_NUMDISPBUF_M1,
          bufParam->numOutputBufs-1u);
        CSL_REG32_WR(&sdeRegs->DISPBUFCFG, regVal);

        CSL_REG32_FINS(&sdeRegs->IRCBUF,
          DMPAC_SDE_IRCBUF_IRCBUFAD,
          (uint32_t)bufParam->rowCostBufferAddress);
    }

    return (status);
}

/**
 *  \brief API to get the confidence score histogram for SDE.
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param csHistogram      Pointer to the memory where Confidence Score
 *                          Histogram will be stored
 */
int32_t CSL_sdeGetCsHistogram(const CSL_dmpac_sdeRegs  *sdeRegs,
                              uint32_t           *csHistogram)
{
    int32_t  status = FVID2_SOK;
    uint32_t idx;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The negative test framework and test apps cannot reach this portion.
    No test configuration will result in NULL arguments from the caller.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == sdeRegs) || (NULL == csHistogram))
    {
        status = FVID2_EBADARGS;
    }

    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        for(idx = 0u; idx < DMPAC_SDE_MAX_HISTOGRAM_SIZE; idx++)
        {
            csHistogram[idx] = CSL_REG32_RD(&sdeRegs->HIST[idx]);
            csHistogram[idx] = csHistogram[idx] &
                               CSL_DMPAC_SDE_HIST_BIN_STS_MASK;
        }
    }
    return (status);
}

/**
 *  \brief API for getting the CRC of the disparity output
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param crc              Pointer to the memory where crc for the disparity
 *                          output is stored
 *
 *  \return                 none
 */
int32_t CSL_sdeGetPsa(const CSL_dmpac_sdeRegs           *sdeRegs,
                      uint32_t                    *crc)
{
    int32_t  status = FVID2_SOK;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The negative test framework and test apps cannot reach this portion.
    No test configuration will result in NULL arguments from the caller.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if ((NULL == sdeRegs) || (NULL == crc))
    {
        status = FVID2_EBADARGS;
    }

    if (FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        *crc = CSL_REG32_RD(&sdeRegs->PSA_SIGNATURE);
    }

    return (status);
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

static int32_t CSL_sdeCheckCfg(const Sde_Config *cfg)
{
    int32_t status = FVID2_SOK;

    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The negative test framework and test apps cannot reach this portion.
    No test configuration will result in NULL arguments from the caller.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(NULL == cfg)
    {
        status = FVID2_EBADARGS;
    }
    else
  /* LDRA_JUSTIFY_END */
    {
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The negative test framework and test apps cannot reach this portion.
    Search Range error is already filtered out before call to this function.
    Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
    However, due to the stated rationale, this is not tested.
    <justification end> */
        if((SDE_SR_64 != cfg->searchRange)  &&
           (SDE_SR_128 != cfg->searchRange)  &&
           (SDE_SR_192 != cfg->searchRange))
        {
            /* iirFilterAlpha is out of supported range */
            status = FVID2_EINVALID_PARAMS;
        }
    /* LDRA_JUSTIFY_END */
    }

    return (status);
}

/**
 *  \brief API for getting the PSA signature of the output flow vector
 *
 *  \param sdeRegs          Pointer to structure containing SDE Base Address
 *  \param psaSig           Pointer to the memory where crc for the flow
 *                          vector will be stored
 *
 *  \return                 Returns 0 on success else returns error value
 */
void CSL_sdeEnablePsa(CSL_dmpac_sdeRegs           *sdeRegs,
                      uint32_t                    enable)
{
    CSL_REG32_FINS(&sdeRegs->PSA_CTRL,
          DMPAC_SDE_PSA_CTRL_PSA_EN_CFG,
          enable);
}
