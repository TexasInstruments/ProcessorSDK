/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *  \file     safety_checkers_pm.c
 *
 *  \brief    This file contains PM safety checker library functions
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <cslr.h>
#include <safety_checkers_pm.h>
#include <safety_checkers_common.h>
#include <safety_checkers_soc.h>

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

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 * Design: SAFETY_CHECKERS-249
 */
int32_t SafetyCheckers_pmGetPscRegCfg(uintptr_t *pscRegCfg, uint32_t size)
{
    uint32_t md = 0U;
    uint32_t pd = 0U;
    uint32_t offset = 0U;
    uint32_t totalLength = 0U;
    uint32_t totalPSC;
    int32_t  status = SAFETY_CHECKERS_SOK;

    /* Check if pscRegCfg is NULL */
    if (pscRegCfg == NULL)
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (totalPSC = 0U; totalPSC < (sizeof(gSafetyCheckers_PmPscData) / sizeof(SafetyCheckers_PmPscData)); totalPSC++)
        {
            totalLength += (uint32_t)gSafetyCheckers_PmPscData[totalPSC].pdStat + gSafetyCheckers_PmPscData[totalPSC].mdStat;
        }

        if (totalLength > size)
        {
            /* Check for the buffer size */
            status = SAFETY_CHECKERS_INSUFFICIENT_BUFF;
        }
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (totalPSC = 0U; totalPSC < (sizeof(gSafetyCheckers_PmPscData) / sizeof(SafetyCheckers_PmPscData)); totalPSC++)
        {
            for(pd = 0U; pd < (gSafetyCheckers_PmPscData[totalPSC].pdStat); pd++)
            {
                pscRegCfg[offset] = (uintptr_t)CSL_REG32_RD((gSafetyCheckers_PmPscData[totalPSC].baseAddr + SAFETY_CHECKERS_PM_PSC_PD_STAT_OFFSET) + (0x4U * pd));
                offset++;
            }

            for(md = 0U; md < (gSafetyCheckers_PmPscData[totalPSC].mdStat); md++)
            {
                pscRegCfg[offset] = (uintptr_t)CSL_REG32_RD((gSafetyCheckers_PmPscData[totalPSC].baseAddr + SAFETY_CHECKERS_PM_PSC_MD_STAT_OFFSET) + (0x4U * md));
                offset++;
            }
        }
    }

    return (status);
}

/**
 * Design: SAFETY_CHECKERS-250, SAFETY_CHECKERS-251
 */
int32_t SafetyCheckers_pmVerifyPscRegCfg(const uintptr_t *pscRegCfg, uint32_t size)
{
    uint32_t md = 0U;
    uint32_t pd = 0U;
    uint32_t offset = 0U;
    uint32_t totalLength = 0U;
    uint32_t readData = 0U;
    uint32_t mismatchCnt = 0U;
    uint32_t totalPSC;
    int32_t  status = SAFETY_CHECKERS_SOK;

    /* Check if pscRegCfg is NULL */
    if (pscRegCfg == NULL)
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (totalPSC = 0U; totalPSC < (sizeof(gSafetyCheckers_PmPscData) / sizeof(SafetyCheckers_PmPscData)); totalPSC++)
        {
            totalLength += (uint32_t)gSafetyCheckers_PmPscData[totalPSC].pdStat + gSafetyCheckers_PmPscData[totalPSC].mdStat;
        }

        if (totalLength > size)
        {
            /* Check for the buffer size */
            status = SAFETY_CHECKERS_INSUFFICIENT_BUFF;
        }
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (totalPSC = 0U; totalPSC < (sizeof(gSafetyCheckers_PmPscData) / sizeof(SafetyCheckers_PmPscData)); totalPSC++)
        {
            for(pd = 0U; pd < (gSafetyCheckers_PmPscData[totalPSC].pdStat); pd++)
            {
                readData = (uint32_t)CSL_REG32_RD((gSafetyCheckers_PmPscData[totalPSC].baseAddr + SAFETY_CHECKERS_PM_PSC_PD_STAT_OFFSET) + (0x4U * pd));

                mismatchCnt |= pscRegCfg[offset] ^ readData;

                offset++;
            }

            for(md = 0U; md < (gSafetyCheckers_PmPscData[totalPSC].mdStat); md++)
            {
                readData = (uint32_t)CSL_REG32_RD((gSafetyCheckers_PmPscData[totalPSC].baseAddr + SAFETY_CHECKERS_PM_PSC_MD_STAT_OFFSET) + (0x4U * md));

                mismatchCnt |= (uint32_t)(pscRegCfg[offset] ^ readData);

                offset++;
            }
        }
        if(mismatchCnt != 0U)
        {
            status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
        }
    }

    return (status);
}

/**
 * Design: SAFETY_CHECKERS-248
 */
int32_t SafetyCheckers_pmGetPllRegCfg(uintptr_t *pllRegCfg, uint32_t size)
{
    uint32_t offset = 0U;
    uint32_t totalLength = 0U;
    uint32_t length;
    uint32_t pllLength;
    int32_t  status = SAFETY_CHECKERS_SOK;

    /* Check if pllRegCfg is NULL */
    if (pllRegCfg == NULL)
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (length = 0U; length < (sizeof(gSafetyCheckers_PmPllData) / sizeof(SafetyCheckers_PmPllData)); length++)
        {
            pllLength = 0U;
            while((gSafetyCheckers_PmPllData[length].length) != gSafetyCheckers_PmPllData[length].regOffsetArr[pllLength])
		    {
                pllLength++;
                totalLength++;
            }
        }

        /* Check for the buffer size */
        if (totalLength > size)
        {
            status = SAFETY_CHECKERS_INSUFFICIENT_BUFF;
        }
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (length = 0U; length < (sizeof(gSafetyCheckers_PmPllData) / sizeof(SafetyCheckers_PmPllData)); length++)
        {
		    pllLength = 0U;
            while((gSafetyCheckers_PmPllData[length].length) != gSafetyCheckers_PmPllData[length].regOffsetArr[pllLength])
		    {
			    pllRegCfg[offset] = (uintptr_t)CSL_REG32_RD((gSafetyCheckers_PmPllData[length].baseAddr) + gSafetyCheckers_PmPllData[length].regOffsetArr[pllLength]);

                offset++;
			    pllLength++;
            }
        }
    }

    return (status);
}

/**
 * Design: SAFETY_CHECKERS-247, SAFETY_CHECKERS-251
 */
int32_t SafetyCheckers_pmVerifyPllRegCfg(const uintptr_t *pllRegCfg, uint32_t size)
{
    uint32_t offset = 0U;
    uint32_t totalLength = 0U;
    uint32_t readData = 0U;
    uint32_t mismatchCnt = 0U;
    uint32_t length;
    uint32_t pllLength;
    int32_t  status = SAFETY_CHECKERS_SOK;

    /* Check if pllRegCfg is NULL */
    if (pllRegCfg == NULL)
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (length = 0U; length < (sizeof(gSafetyCheckers_PmPllData) / sizeof(SafetyCheckers_PmPllData)); length++)
        {
            pllLength = 0U;
            while((gSafetyCheckers_PmPllData[length].length) != gSafetyCheckers_PmPllData[length].regOffsetArr[pllLength])
		    {
                pllLength++;
                totalLength++;
            }
        }

        if (totalLength > size)
        {
            /* Check for the buffer size */
            status = SAFETY_CHECKERS_INSUFFICIENT_BUFF;
        }
    }

    if(status == SAFETY_CHECKERS_SOK)
    {
        for (length = 0U; length < (sizeof(gSafetyCheckers_PmPllData) / sizeof(SafetyCheckers_PmPllData)); length++)
        {
		    pllLength = 0U;
		    while ((gSafetyCheckers_PmPllData[length].length) != gSafetyCheckers_PmPllData[length].regOffsetArr[pllLength])
            {
                readData = (uint32_t)CSL_REG32_RD((gSafetyCheckers_PmPllData[length].baseAddr) + gSafetyCheckers_PmPllData[length].regOffsetArr[pllLength]);

                mismatchCnt |= (uint32_t)(pllRegCfg[offset] ^ readData);

                offset++;
			    pllLength++;
            }
        }

        if(mismatchCnt != 0U)
	    {
		    status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
	    }
    }

    return (status);
}

/**
 * Design: SAFETY_CHECKERS-246, SAFETY_CHECKERS-251
 */
int32_t SafetyCheckers_pmRegisterLock(void)
{
    uint32_t index;
    uint32_t lockStat;
    uint32_t pllLockCnt = 0U;
    int32_t  status = SAFETY_CHECKERS_FAIL;

    for (index = 0U; index < (sizeof(gSafetyCheckers_PmPllData) / sizeof(SafetyCheckers_PmPllData)); index++)
    {
        /* Lock the PLL register access */
        CSL_REG32_WR((gSafetyCheckers_PmPllData[index].baseAddr + SAFETY_CHECKERS_PM_LOCK_KEY0_OFFSET), SAFETY_CHECKERS_PM_KICK_LOCK);
        CSL_REG32_WR((gSafetyCheckers_PmPllData[index].baseAddr + SAFETY_CHECKERS_PM_LOCK_KEY1_OFFSET), SAFETY_CHECKERS_PM_KICK_LOCK);

        /* Confirm the PLL registers are locked */
        lockStat = (uint32_t)CSL_REG32_RD(gSafetyCheckers_PmPllData[index].baseAddr + SAFETY_CHECKERS_PM_LOCK_KEY0_OFFSET);

        /* check for PLL lock confirmation*/
        if((lockStat & 0x1U) == 0x0U)
        {
            pllLockCnt = (uint32_t)(pllLockCnt) + 1U;
        }
    }

    if(pllLockCnt == (sizeof(gSafetyCheckers_PmPllData) / sizeof(SafetyCheckers_PmPllData)))
    {
        status = SAFETY_CHECKERS_SOK;
    }

    return (status);
}
