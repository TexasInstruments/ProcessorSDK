/*
 *  Copyright (C) 2024 Texas Instruments Incorporated.
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
 *  \file  csl_clec.c
 *
 *  \brief Implementation file for the CLEC CSL-FL module.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/csl_clec.h>

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t CSL_clecConfigEvent(CSL_CLEC_EVTRegs *pRegs,
                            uint32_t evtNum,
                            const CSL_ClecEventConfig *evtCfg)
{
    int32_t     retVal = CSL_PASS;
    uint32_t    regVal;

    if((NULL == pRegs) ||
       (NULL == evtCfg) ||
       (evtNum >= CSL_CLEC_MAX_EVT_IN) ||
       (evtCfg->extEvtNum >= CSL_CLEC_MAX_EXT_EVT_OUT) ||
       (evtCfg->c7xEvtNum >= CSL_CLEC_MAX_C7X_EVT_OUT))
    {
        retVal = CSL_EFAIL;
    }
    else
    {
        /* Perform read/modify/write so that the default interrupt mode (bit 24)
         * is in power on reset value and should not be changed by CSL
         */
        regVal = CSL_REG32_RD(&pRegs->CFG[evtNum].MRR);
        CSL_REG32_FINS(&regVal, CLEC_EVT_CFG_MRR_S, evtCfg->secureClaimEnable);
        CSL_REG32_FINS(&regVal, CLEC_EVT_CFG_MRR_ESE, evtCfg->evtSendEnable);
        CSL_REG32_FINS(&regVal, CLEC_EVT_CFG_MRR_RTMAP, evtCfg->rtMap);
        CSL_REG32_FINS(&regVal, CLEC_EVT_CFG_MRR_EXT_EVTNUM, evtCfg->extEvtNum);
        CSL_REG32_FINS(&regVal, CLEC_EVT_CFG_MRR_C7X_EVTNUM, evtCfg->c7xEvtNum);
        CSL_REG32_WR(&pRegs->CFG[evtNum].MRR, regVal);
    }

    return (retVal);
}

int32_t CSL_clecConfigEventLevel(CSL_CLEC_EVTRegs *pRegs,
                            uint32_t evtNum,
                            uint32_t is_level)
{
    int32_t     retVal = CSL_PASS;
    uint32_t    regVal;

    if((NULL == pRegs) ||
       (evtNum >= CSL_CLEC_MAX_EVT_IN) )
    {
        retVal = CSL_EFAIL;
    }
    else
    {
        /* Update is_lvl field (bit 24)
         */
        regVal = CSL_REG32_RD(&pRegs->CFG[evtNum].MRR);
        if(is_level==0)
        {
            regVal &= ~(1<<24);
        }
        else
        {
            regVal |= (1<<24);
        }
        CSL_REG32_WR(&pRegs->CFG[evtNum].MRR, regVal);
    }

    return (retVal);
}

int32_t CSL_clecSendEvent(CSL_CLEC_EVTRegs *pRegs, uint32_t evtNum)
{
    int32_t     retVal = CSL_PASS;
    uint32_t    regVal;

    if((NULL == pRegs) ||
       (evtNum >= CSL_CLEC_MAX_EVT_IN))
    {
        retVal = CSL_EFAIL;
    }
    else
    {
        /* ESR is a write-only register. It ignores the value written
         * (stateless). So write 0. */
        regVal = 0x0U;
        CSL_REG32_WR(&pRegs->CFG[evtNum].ESR, regVal);
    }

    return (retVal);
}

int32_t CSL_clecClearEvent(CSL_CLEC_EVTRegs *pRegs, uint32_t evtNum)
{
    int32_t     retVal = CSL_PASS;
    uint32_t    regVal;

    if((NULL == pRegs) ||
       (evtNum >= CSL_CLEC_MAX_EVT_IN))
    {
        retVal = CSL_EFAIL;
    }
    else
    {
        /* ECR is a write-only register. When written value of 0x1
         * it will clear the event. So write 0x1. */
        regVal = 0x1U;
        CSL_REG32_WR(&pRegs->CFG[evtNum].ECR, regVal);
    }

    return (retVal);
}

int32_t CSL_clecGetSecureClaimStatus(CSL_CLEC_EVTRegs *pRegs, uint32_t evtNum, uint32_t *secureClaim)
{
    int32_t     retVal = CSL_PASS;
    uint32_t    regVal;

    if((NULL == pRegs) ||
       (evtNum >= CSL_CLEC_MAX_EVT_IN))
    {
        retVal = CSL_EFAIL;
    }
    else
    {
        regVal = CSL_REG32_RD(&pRegs->CFG[evtNum].MRR);
        *secureClaim = CSL_REG32_FEXT(&regVal, CLEC_EVT_CFG_MRR_S);
    }
    
    return (retVal);
}

#if defined (BUILD_C7X)
uint32_t CSL_clecGetC7xRtmapCpuId(void)
{
    uint8_t corePacNum;
    uint32_t retVal;
    uint64_t dnum;
    /* Get the bits from bit 7 to bit 15, which represents the core pac number */
#if !defined (HOST_EMULATION)
    dnum = __DNUM;
#else
    /* NA for Host Emulation Build. Hence this will return RTMAP for CPU ALL */
    dnum = 0U;
#endif
    corePacNum = CSL_REG64_FEXT(&dnum, C7X_CPU_DNUM_COREPACNUM);
    switch (corePacNum)
    {
        case CSL_C7X_CPU_COREPACK_NUM_C7X1:
            retVal = CSL_CLEC_RTMAP_CPU_4;
            break;
        case CSL_C7X_CPU_COREPACK_NUM_C7X2:
            retVal = CSL_CLEC_RTMAP_CPU_5;
            break;
        default:
            retVal = CSL_CLEC_RTMAP_CPU_ALL;
            break;
    }
    return retVal;
}
#endif
