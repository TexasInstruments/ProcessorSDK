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
 *  \file csl_nsf4.c
 *
 *  \brief CSL for NSF4 module, contains implementation of API,
 *         configuring NSF4 module.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <src/csl/include/csl_nsf4.h>


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

/**
 * \brief   Function to set the LSCC configuration in NSF4v
 *
 * \param   instInfo           Pointer to NSF4 register overlay
 * \param   cfg                Pointer to LSCC config
 *
 **/
static void CSL_nsf4vSetLsccConfig(CSL_nsf4vRegs *nsf4Regs,
    const Nsf4_LsccConfig *lCfg);


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t CSL_nsf4vSetFrameConfig(CSL_nsf4vRegs *nsf4Regs,
    const CslNsf4v_FrameConfig *cfg)
{
    int32_t             status = FVID2_EBADARGS;
    volatile uint32_t   regVal;

    if ((NULL != nsf4Regs) && (NULL != cfg))
    {
        regVal = CSL_REG32_RD(&nsf4Regs->DIM);
        CSL_FINS(regVal, NSF4V_DIM_IW_CFG, (cfg->width - 1u));
        CSL_FINS(regVal, NSF4V_DIM_IH_CFG, (cfg->height - 1u));
        CSL_REG32_WR(&nsf4Regs->DIM, regVal);

        status = FVID2_SOK;
    }

    return (status);
}

/**
 *  \brief Sets the entire NSF4 configuration to the NSF4v core registers.
 *
 *  \param nsf4Regs         Base address for NSF4V config space.
 *                          This parameter should be non-NULL.
 *  \param config           Pointer to Nsf4v_Config structure
 *                          containing the register configurations.
 *                          This parameter should be non-NULL.
 *  \return                 Returns FVID2_SOK on success
 *                          else returns error value
 */
int32_t CSL_nsf4vSetConfig(CSL_nsf4vRegs *nsf4Regs, const Nsf4v_Config *cfg)
{
    int32_t             status = FVID2_EBADARGS;
    uint32_t            cnt;
    uint32_t            tmpVar;
    uint32_t            segCnt;
    volatile uint32_t   regVal;

    if ((NULL != nsf4Regs) && (NULL != cfg))
    {
        regVal = CSL_REG32_RD(&nsf4Regs->TN_SCALE);
        CSL_FINS(regVal, NSF4V_TN_SCALE_TN1_CFG, cfg->tnScale[0U]);
        CSL_FINS(regVal, NSF4V_TN_SCALE_TN2_CFG, cfg->tnScale[1U]);
        CSL_FINS(regVal, NSF4V_TN_SCALE_TN3_CFG, cfg->tnScale[2U]);
        CSL_REG32_WR(&nsf4Regs->TN_SCALE, regVal);

        CSL_REG32_FINS(&nsf4Regs->U_KNEE, NSF4V_U_KNEE_U_KNEE_CFG, cfg->tKnee);

        regVal = CSL_REG32_RD(&nsf4Regs->WHITEBAL0);
        CSL_FINS(regVal, NSF4V_WHITEBAL0_GAIN0_CFG, cfg->gains[0U]);
        CSL_FINS(regVal, NSF4V_WHITEBAL0_GAIN1_CFG, cfg->gains[1U]);
        CSL_REG32_WR(&nsf4Regs->WHITEBAL0, regVal);

        regVal = CSL_REG32_RD(&nsf4Regs->WHITEBAL1);
        CSL_FINS(regVal, NSF4V_WHITEBAL1_GAIN2_CFG, cfg->gains[2U]);
        CSL_FINS(regVal, NSF4V_WHITEBAL1_GAIN3_CFG, cfg->gains[3U]);
        CSL_REG32_WR(&nsf4Regs->WHITEBAL1, regVal);

        regVal = CSL_REG32_RD(&nsf4Regs->CTRL);
        if (CSL_NSF4V_TN_MODE_BIT == (cfg->mode & CSL_NSF4V_TN_MODE_BIT))
        {
            CSL_FINS(regVal, NSF4V_CTRL_TN_MODE_CFG, 1U);
        }
        else
        {
            CSL_FINS(regVal, NSF4V_CTRL_TN_MODE_CFG, 0U);
        }
        tmpVar = cfg->mode & (~CSL_NSF4V_TN_MODE_BIT);
        CSL_FINS(regVal, NSF4V_CTRL_U_MODE_CFG, tmpVar);
        CSL_REG32_WR(&nsf4Regs->CTRL, regVal);

        for (cnt = 0u; cnt < FVID2_BAYER_COLOR_COMP_MAX; cnt ++)
        {
            for (segCnt = 0u; segCnt < NSF4_TN_MAX_SEGMENT; segCnt ++)
            {
                regVal = CSL_REG32_RD(&nsf4Regs->COLOR[cnt].TNSEG[segCnt].TN0);
                CSL_FINS(regVal, NSF4V_COLOR_TNSEG_TN0_X_CFG,
                    (uint32_t)cfg->tnCurve[cnt][segCnt].posX);
                CSL_FINS(regVal, NSF4V_COLOR_TNSEG_TN0_Y_CFG,
                    (uint32_t)cfg->tnCurve[cnt][segCnt].posY);
                CSL_REG32_WR(&nsf4Regs->COLOR[cnt].TNSEG[segCnt].TN0,
                    regVal);

                CSL_REG32_FINS(&nsf4Regs->COLOR[cnt].TNSEG[segCnt].TN1,
                    NSF4V_COLOR_TNSEG_TN1_S_CFG,
                    cfg->tnCurve[cnt][segCnt].slope);
            }
        }

        CSL_nsf4vSetLsccConfig(nsf4Regs, &cfg->lsccCfg);
        status = FVID2_SOK;
    }

    return (status);
}

static void CSL_nsf4vSetLsccConfig(CSL_nsf4vRegs *nsf4Regs,
    const Nsf4_LsccConfig *lCfg)
{
    uint32_t            cnt;
    uint32_t            sgCnt;
    volatile uint32_t   regVal;

    if ((NULL != nsf4Regs) && (NULL != lCfg))
    {
        if ((uint32_t)UTRUE == lCfg->enable)
        {
            regVal = CSL_REG32_RD(&nsf4Regs->LSCC);
            CSL_FINS(regVal, NSF4V_LSCC_GMAX_CFG, lCfg->gMaxCfg);
            CSL_FINS(regVal, NSF4V_LSCC_T_CFG, lCfg->tCfg);
            CSL_FINS(regVal, NSF4V_LSCC_KV_CFG, lCfg->kvCfg);
            CSL_FINS(regVal, NSF4V_LSCC_KH_CFG, lCfg->khCfg);
            CSL_REG32_WR(&nsf4Regs->LSCC, regVal);

            regVal = CSL_REG32_RD(&nsf4Regs->LSCC_CENT);
            CSL_FINS(regVal, NSF4V_LSCC_CENT_X_CFG, lCfg->lensCenterX);
            CSL_FINS(regVal, NSF4V_LSCC_CENT_Y_CFG, lCfg->lensCenterY);
            CSL_REG32_WR(&nsf4Regs->LSCC_CENT, regVal);

            for (cnt = 0U; cnt < NSF4_LSCC_MAX_SET; cnt ++)
            {
                for (sgCnt = 0U; sgCnt < NSF4_LSCC_MAX_SEGMENT; sgCnt ++)
                {
                    regVal = CSL_REG32_RD(
                        &nsf4Regs->SET[cnt].LSCCSEG[sgCnt].LSCCCURVE0);
                    CSL_FINS(regVal, NSF4V_SET_LSCCSEG_LSCCCURVE0_X_CFG,
                        (uint32_t)lCfg->pwlCurve[cnt][sgCnt].posX);
                    CSL_FINS(regVal, NSF4V_SET_LSCCSEG_LSCCCURVE0_Y_CFG,
                        (uint32_t)lCfg->pwlCurve[cnt][sgCnt].posY);
                    CSL_REG32_WR(&nsf4Regs->SET[cnt].LSCCSEG[sgCnt].LSCCCURVE0,
                        regVal);

                    CSL_REG32_FINS(&nsf4Regs->SET[cnt].LSCCSEG[sgCnt].LSCCCURVE1,
                        NSF4V_SET_LSCCSEG_LSCCCURVE1_S_CFG,
                        lCfg->pwlCurve[cnt][sgCnt].slope);
                }
            }

            regVal = CSL_REG32_RD(&nsf4Regs->CTRL);
            CSL_FINS(regVal, NSF4V_CTRL_LSCC_SETSEL_CFG, lCfg->setSel);
            CSL_FINS(regVal, NSF4V_CTRL_LSCC_EN_CFG, (uint32_t)1U);
            CSL_REG32_WR(&nsf4Regs->CTRL, regVal);
        }
        else
        {
            CSL_REG32_FINS(&nsf4Regs->CTRL, NSF4V_CTRL_LSCC_EN_CFG, 0U);
        }
    }
}
