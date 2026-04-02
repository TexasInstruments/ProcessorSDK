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
 *  \file     safety_checkers_rm.c
 *
 *  \brief    This file contains RM Safety checker library functions
 *
 */
 
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <cslr.h>
#include <safety_checkers_rm.h>
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
 * Design: SAFETY_CHECKERS-244
 */
int32_t SafetyCheckers_rmGetRegCfg(uintptr_t *rmRegCfg, uint32_t size)
{
    uint32_t offset = 0U;
    uint32_t totalLength = 0U;
    uint32_t totalModule;
    uint32_t regNum;
    uint32_t regArrayLen;
	int32_t status = SAFETY_CHECKERS_SOK;
	
	if (rmRegCfg == NULL)
	{
		status = SAFETY_CHECKERS_FAIL;
	}

    if (status == SAFETY_CHECKERS_SOK)
	{
		for (totalModule = 0U; totalModule < (sizeof(gSafetyCheckers_RmRegData) / sizeof(SafetyCheckers_RmRegData)); totalModule++)
		{
			totalLength += (uint32_t)((gSafetyCheckers_RmRegData[totalModule].regNum) * (gSafetyCheckers_RmRegData[totalModule].regArrayLen));
		}
		
		if (totalLength > size)
		{
			status = SAFETY_CHECKERS_INSUFFICIENT_BUFF;
		}
	}

    if (status == SAFETY_CHECKERS_SOK)
	{
		for (totalModule = 0U; totalModule < (sizeof(gSafetyCheckers_RmRegData) / sizeof(SafetyCheckers_RmRegData)); totalModule++)
		{
			for (regNum = 0U; regNum < gSafetyCheckers_RmRegData[totalModule].regNum; regNum++)
			{
				for (regArrayLen = 0U; regArrayLen < gSafetyCheckers_RmRegData[totalModule].regArrayLen; regArrayLen++)
				{
					rmRegCfg[offset] = (uintptr_t)CSL_REG32_RD(gSafetyCheckers_RmRegData[totalModule].baseAddr +\
					((gSafetyCheckers_RmRegData[totalModule].regStartOffset * regArrayLen) + gSafetyCheckers_RmRegData[totalModule].regOffsetArr[regNum]));

					offset++;
				}
			}
		}
	}
	
    return status;
}

/**
 * Design: SAFETY_CHECKERS-245, SAFETY_CHECKERS-251
 */
int32_t SafetyCheckers_rmVerifyRegCfg (const uintptr_t *rmRegCfg, uint32_t size)
{
    uint32_t offset = 0U;
    uint32_t mismatchCnt = 0U;
    uint32_t totalLength = 0U;
    uint32_t totalModule;
    uint32_t regNum;
    uint32_t regArrayLen;
	int32_t status = SAFETY_CHECKERS_SOK;
	
	if (rmRegCfg == NULL)
	{
		status = SAFETY_CHECKERS_FAIL;
	}

    if (status == SAFETY_CHECKERS_SOK)
	{
		for (totalModule = 0U; totalModule < (sizeof(gSafetyCheckers_RmRegData) / sizeof(SafetyCheckers_RmRegData)); totalModule++)
		{
			totalLength += (uint32_t)((gSafetyCheckers_RmRegData[totalModule].regNum) * (gSafetyCheckers_RmRegData[totalModule].regArrayLen));
		}

		if (totalLength > size)
		{
			status = SAFETY_CHECKERS_INSUFFICIENT_BUFF;
		}
	}

    if (status == SAFETY_CHECKERS_SOK)
	{
		for (totalModule = 0U; totalModule < (sizeof(gSafetyCheckers_RmRegData) / sizeof(SafetyCheckers_RmRegData)); totalModule++)
		{
			for (regNum = 0U; regNum < gSafetyCheckers_RmRegData[totalModule].regNum; regNum++)
			{
				for (regArrayLen = 0U; regArrayLen < gSafetyCheckers_RmRegData[totalModule].regArrayLen; regArrayLen++)
				{
					uint32_t readData = (uint32_t)CSL_REG32_RD(gSafetyCheckers_RmRegData[totalModule].baseAddr +\
					((gSafetyCheckers_RmRegData[totalModule].regStartOffset * regArrayLen) + gSafetyCheckers_RmRegData[totalModule].regOffsetArr[regNum]));			
					mismatchCnt |= (uint32_t)(rmRegCfg[offset] ^ readData);
					offset++;
				}
			}
		}
	}

   if(mismatchCnt != 0U)
   {
       status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
   }

   return status;
}
