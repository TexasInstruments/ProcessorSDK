/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \file     safety_checkers_csirx.c
 *
 *  \brief    This file contains CSIRX safety checker library functions
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#if defined (SOC_J722S)
#include <cslr.h>
#include <drivers/hw_include/soc_config.h>
#if defined(DRV_VERSION_CSIRX_V0)
#include <drivers/csirx/v0/include/csirx_drvPriv.h>
#endif      /* DRV_VERSION_CSIRX_V0 */
#if defined(DRV_VERSION_CSIRX_V1)
#include <drivers/csirx/v1/include/csirx_drvPriv.h>
#endif      /* DRV_VERSION_CSIRX_V1 */
#else
#include <ti/csl/cslr.h>
#include <ti/drv/csirx/src/csirx_drvPriv.h>
#endif
#include <safety_checkers_csirx.h>
#include <safety_checkers_common.h>

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

extern SafetyCheckers_CsirxRegData gSafetyCheckers_CsirxRegData[SAFETY_CHECKERS_CSIRX_NUM_REGTYPE_MAX];

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 * Design: SAFETY_CHECKERS-402
 */
int32_t SafetyCheckers_csirxGetRegCfg(uintptr_t *regCfg,
                                      uint32_t regType,
                                      uint32_t instance)
{
    uint32_t regNum, mask = SAFETY_CHECKERS_CSIRX_MASK_NONE;
    int32_t  status = SAFETY_CHECKERS_SOK;
    SafetyCheckers_CsirxInstData *instData;

    /* Check if regCfg is NULL */
    if(NULL == regCfg)
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        instData = &gSafetyCheckers_CsirxRegData[regType].instData[instance];
        for(regNum=0U; regNum<instData->length; regNum++)
        {
            /* DPHY lane configuration register contains a dynamic register-field
             * which will change based on rx high speed clock active status. Hence omitting
             * the field with the help of mask
             */
            if (regType == SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG)
            {
                mask = SAFETY_CHECKERS_CSIRX_MASK_LANE_CONFIG;
            }
            regCfg[regNum] = (uintptr_t)CSL_REG32_RD(instData->baseAddr +
                                                     instData->regOffsetArr[regNum]) & mask;
        }
    }

    return status;
}

/**
 * Design: SAFETY_CHECKERS-401
 */
int32_t SafetyCheckers_csirxVerifyRegCfg(const uintptr_t *regCfg,
                                         uint32_t regType,
                                         uint32_t instance)
{
    uint32_t readData = 0U;
    uint32_t mismatchCnt = 0U;
    uint32_t regNum, mask = SAFETY_CHECKERS_CSIRX_MASK_NONE;
    int32_t  status = SAFETY_CHECKERS_SOK;
    SafetyCheckers_CsirxInstData *instData;

    /* Check if regCfg is NULL */
    if(NULL == regCfg)
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        instData = &gSafetyCheckers_CsirxRegData[regType].instData[instance];
        for(regNum=0U; regNum<instData->length; regNum++)
        {
            if (regType == SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG)
            {
                mask = SAFETY_CHECKERS_CSIRX_MASK_LANE_CONFIG;
            }
            readData = (uintptr_t)CSL_REG32_RD(instData->baseAddr + instData->regOffsetArr[regNum]) & mask;
            mismatchCnt |= regCfg[regNum] ^ readData;
        }

        if(0U != mismatchCnt)
        {
            status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
        }
    }

    return status;
}

/**
 * Design: SAFETY_CHECKERS-401
 */
int32_t SafetyCheckers_csirxVerifyCsiAvailBandwidth(void *drvHandle, uint32_t fps)
{
    int32_t status = SAFETY_CHECKERS_SOK;
    uint32_t count = 0U, frameSize = 0U;
    CsirxDrv_VirtContext *virtContext = NULL;
    CsirxDrv_InstObj *instObj = NULL;
    if(NULL == drvHandle)
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckers_CsirxFdmChannel *channel = (SafetyCheckers_CsirxFdmChannel*)drvHandle;
        virtContext = (CsirxDrv_VirtContext *)channel->drvHandle;
        instObj = virtContext->instObj;

        for(count=0U; count<instObj->createParams.numCh; count++)
        {
            frameSize += (instObj->createParams.chCfg[count].outFmt.pitch[0U]*instObj->createParams.chCfg[count].outFmt.height);
        }

        if(SAFETY_CHECKERS_CSIRX_MAX_FRAME_SIZE < (fps* frameSize))
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }

    return status;
}

/**
 * Design: SAFETY_CHECKERS-402
 */
int32_t SafetyCheckers_csirxGetVimCfg(void *drvHandle, SafetyCheckers_CsirxVimCfg *vimCfg)
{
    int32_t status = SAFETY_CHECKERS_SOK;
    uint32_t coreintrNum;
    CsirxDrv_VirtContext *virtContext = NULL;
    CsirxDrv_InstObj *instObj = NULL;
    SafetyCheckers_CsirxFdmChannel *channel;

    if((NULL == drvHandle) || (NULL == vimCfg))
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        channel = (SafetyCheckers_CsirxFdmChannel*)drvHandle;
        virtContext = (CsirxDrv_VirtContext *)channel->drvHandle;
        instObj = virtContext->instObj;

        for(uint32_t count=0U; count<CSIRX_EVENT_GROUP_MAX; count++)
        {
            if(1U == instObj->eventObj[count].eventInitDone)
            {
                coreintrNum = instObj->eventObj[count].coreIntrNum;
                status += SafetyCheckers_csirxGetVimRegCfgIntrNum(coreintrNum,
                                                                  vimCfg);
            }
        }
    }

    return status;
}

/**
 * Design: SAFETY_CHECKERS-401
 */
int32_t SafetyCheckers_csirxVerifyVimCfg(void *drvHandle, SafetyCheckers_CsirxVimCfg *vimCfg)
{
    int32_t status = SAFETY_CHECKERS_SOK;
    CsirxDrv_VirtContext *virtContext = NULL;
    CsirxDrv_InstObj *instObj = NULL;

    if((NULL == drvHandle) || (NULL == vimCfg))
    {
        status = SAFETY_CHECKERS_FAIL;
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        SafetyCheckers_CsirxFdmChannel *channel = (SafetyCheckers_CsirxFdmChannel*)drvHandle;
        virtContext = (CsirxDrv_VirtContext *)channel->drvHandle;
        instObj = virtContext->instObj;

        for(uint32_t count=0U; count<CSIRX_EVENT_GROUP_MAX; count++)
        {
            if(SAFETY_CHECKERS_CSIRX_EVENT_INIT_DONE == instObj->eventObj[count].eventInitDone)
            {
                status += SafetyCheckers_csirxVerifyVimRegCfgIntrNum(vimCfg);
            }
        }
    }

    return status;
}

/**
 * Design: SAFETY_CHECKERS-402
 */
int32_t SafetyCheckers_csirxGetSensorCfg(void *i2cHandle,
                                         uint32_t slaveAddr,
                                         uint16_t (*regData)[3U])
{
    uint8_t regVal = 0U, regAddr8;
    SafetyCheckers_CsirxI2cTrxnCfg i2cTrxnCfg;
    uint16_t cnt = 0U;
    int32_t  status = SAFETY_CHECKERS_SOK;

    if((NULL == i2cHandle) || (NULL == regData))
    {
        status = SAFETY_CHECKERS_FAIL;
    }
    else
    {
        while(SAFETY_CHECKERS_CSIRX_SENSOR_CFG_END_MARKER != regData[cnt][0U])
        {
            regAddr8 = (uint8_t)(regData[cnt][0U] & 0xFFU);
            i2cTrxnCfg.regAddr = regAddr8;
            i2cTrxnCfg.regData = &regVal;
            i2cTrxnCfg.numOfBytes = 0x1U;
            i2cTrxnCfg.i2cTimeout = 0x1000U;
            status = SafetyCheckers_csirxi2c8BitRegRd(i2cHandle, slaveAddr, &i2cTrxnCfg);
            regData[cnt][1U] = (uint16_t)(regVal & 0xFFU);
            cnt++;
        }
    }

    return status;
}

/**
 * Design: SAFETY_CHECKERS-401
 */
int32_t SafetyCheckers_csirxVerifySensorCfg(void *i2cHandle,
                                            uint32_t slaveAddr,
                                            uint16_t (*regData)[3])
{
    uint8_t regVal = 0U, regAddr8;
    SafetyCheckers_CsirxI2cTrxnCfg i2cTrxnCfg;
    uint8_t regValVerif;
    uint32_t cnt = 0U;
    int32_t  status = SAFETY_CHECKERS_SOK;
    uint32_t mismatchCnt = 0U;

    if((NULL == i2cHandle) || (NULL == regData))
    {
        status = SAFETY_CHECKERS_FAIL;
    }
    else
    {
        while(0xFFFU != regData[cnt][0U])
        {
            regAddr8 = (uint8_t)(regData[cnt][0U] & 0xFFU);
            regValVerif = (uint8_t)(regData[cnt][1U] & 0xFFU);
            i2cTrxnCfg.regAddr = regAddr8;
            i2cTrxnCfg.regData = &regVal;
            i2cTrxnCfg.numOfBytes = 0x1U;
            i2cTrxnCfg.i2cTimeout = 0x1000U;
            status = SafetyCheckers_csirxi2c8BitRegRd(i2cHandle, slaveAddr, &i2cTrxnCfg);
            mismatchCnt |= regValVerif ^ regVal;
            if(mismatchCnt != 0U)
            {
                status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
                break;
            }
            cnt++;
        }
    }

    return status;
}

