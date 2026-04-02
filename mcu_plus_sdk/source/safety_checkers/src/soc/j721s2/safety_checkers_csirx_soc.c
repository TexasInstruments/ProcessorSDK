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
 *  \file     safety_checkers_csirx_soc.c
 *
 *  \brief    This file contains CSIRX safety checker library functions
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/cslr.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/drv/csirx/src/csirx_drvPriv.h>
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

SafetyCheckers_CsirxRegData gSafetyCheckers_CsirxRegData[SAFETY_CHECKERS_CSIRX_NUM_REGTYPE_MAX] =
{
    {
        SAFETY_CHECKERS_CSIRX_REG_TYPE_STRM_CTRL,
        {
            {
                0U,
                SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_BASE_ADDRESS(0U),
                gSafetyCheckers_StrmCtrlRegOffset,
                SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_LENGTH
            },
            {
                1U,
                SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_BASE_ADDRESS(1U),
                gSafetyCheckers_StrmCtrlRegOffset,
                SAFETY_CHECKERS_CSIRX_STRM_CTRL_REGS_LENGTH
            },
        }
    },
    {
        SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_CONFIG,
        {
            {
                0U,
                SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_BASE_ADDRESS(0U),
                gSafetyCheckers_DphyConfigRegOffset,
                SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_LENGTH
            },
            {
                1U,
                SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_BASE_ADDRESS(1U),
                gSafetyCheckers_DphyConfigRegOffset,
                SAFETY_CHECKERS_CSIRX_DPHY_CONFIG_REGS_LENGTH
            },
        }
    },
    {
        SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_PLL,
        {
            {
                0U,
                SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_BASE_ADDRESS(0U),
                gSafetyCheckers_DphyPllRegOffset,
                SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_LENGTH
            },
            {
                1U,
                SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_BASE_ADDRESS(1U),
                gSafetyCheckers_DphyPllRegOffset,
                SAFETY_CHECKERS_CSIRX_DPHY_PLL_REGS_LENGTH
            },
        }
    },
    {
        SAFETY_CHECKERS_CSIRX_REG_TYPE_DPHY_LANE_CONFIG,
        {
            {
                0U,
                SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_BASE_ADDRESS(0U),
                gSafetyCheckers_DphyLaneConfigRegOffset,
                SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_REGS_LENGTH
            },
            {
                1U,
                SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_BASE_ADDRESS(1U),
                gSafetyCheckers_DphyLaneConfigRegOffset,
                SAFETY_CHECKERS_CSIRX_DPHY_LANE_CONFIG_REGS_LENGTH
            },
        }
    },
    {
        SAFETY_CHECKERS_CSIRX_REG_TYPE_VIRTUAL_CHANNEL,
        {
            {
                0U,
                SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_BASE_ADDRESS(0U),
                gSafetyCheckers_VirtualChannelConfigRegOffset,
                SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_REGS_LENGTH
            },
            {
                1U,
                SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_BASE_ADDRESS(1U),
                gSafetyCheckers_VirtualChannelConfigRegOffset,
                SAFETY_CHECKERS_CSIRX_VIRTUAL_CHANNEL_CONFIG_REGS_LENGTH
            },
        }
    },
    {
        SAFETY_CHECKERS_CSIRX_REG_TYPE_DATATYPE_FRAMESIZE,
        {
            {
                0U,
                SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_BASE_ADDRESS(0U),
                gSafetyCheckers_DataTypeFrameSizeRegOffset,
                SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_REGS_LENGTH
            },
            {
                1U,
                SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_BASE_ADDRESS(1U),
                gSafetyCheckers_DataTypeFrameSizeRegOffset,
                SAFETY_CHECKERS_CSIRX_DATATYPE_FRAMESIZE_REGS_LENGTH
            },
        }
    },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


int32_t SafetyCheckers_csirxGetQoSCfg(SafetyCheckers_CsirxQoSSettings *qosSettings,
                                      void *drvHandle)
{

    int32_t  status = SAFETY_CHECKERS_SOK;
    uint32_t count=0U, rxChNum, rCfgRegAddr, rPriCtrlRegAddr;
    Udma_ChHandle rxChHdle;
    CsirxDrv_VirtContext *virtContext = NULL;
    CsirxDrv_InstObj *instObj = NULL;

    if((NULL == qosSettings) || (NULL == drvHandle))
    {
        status = SAFETY_CHECKERS_FAIL;
    }
    else
    {
        SafetyCheckers_CsirxFdmChannel *channel = (SafetyCheckers_CsirxFdmChannel*)drvHandle;
        virtContext = (CsirxDrv_VirtContext *)channel->drvHandle;
        instObj = virtContext->instObj;

        for(count = 0U; count<instObj->createParams.numCh; count++)
        {
            rxChHdle = (Udma_ChHandle)&instObj->chObj[count].rxChObj;
            rxChNum = Udma_chGetNum(rxChHdle);
            rCfgRegAddr = ((SAFETY_CHECKERS_CSIRX_QOS_BASE_ADDRESS_0 +
			   (rxChNum*SAFETY_CHECKERS_CSIRX_QOS_CH_REG_SIZE)) + SAFETY_CHECKERS_CSIRX_QOS_RCFG); 
            qosSettings[count].chanType = (uint8_t)CSL_REG32_FEXT(rCfgRegAddr,
                                                                  BCDMA_RXCCFG_CHAN_RCFG_CHAN_TYPE);

            rPriCtrlRegAddr = ((SAFETY_CHECKERS_CSIRX_QOS_BASE_ADDRESS_0 + 
                (rxChNum*SAFETY_CHECKERS_CSIRX_QOS_CH_REG_SIZE)) + SAFETY_CHECKERS_CSIRX_QOS_RPRICTRL);
            qosSettings[count].priority = (uint8_t)CSL_REG32_FEXT(rPriCtrlRegAddr,
                                                                  BCDMA_RXCCFG_CHAN_RPRI_CTRL_PRIORITY);
            qosSettings[count].busOrderId = (uint8_t)CSL_REG32_FEXT(rPriCtrlRegAddr,
                                                                    BCDMA_RXCCFG_CHAN_RPRI_CTRL_ORDERID);
        }
    }

    return status;
}

int32_t SafetyCheckers_csirxVerifyQoSCfg(SafetyCheckers_CsirxQoSSettings *qosSettings,
                                         void *drvHandle)
{

    int32_t  status = SAFETY_CHECKERS_SOK;
    uint32_t count=0U, mismatchCnt = 0U, rCfgRegAddr, rxChNum, rPriCtrlRegAddr;
    Udma_ChHandle rxChHdle;
    uint8_t chanTypeVerif, priorityVerif, busOrderIdVerif;
    CsirxDrv_VirtContext *virtContext = NULL;
    CsirxDrv_InstObj *instObj = NULL;

    if((NULL == qosSettings) || (NULL == drvHandle))
    {
        status = SAFETY_CHECKERS_FAIL;
    }
    else
    {
        SafetyCheckers_CsirxFdmChannel *channel = (SafetyCheckers_CsirxFdmChannel*)drvHandle;
        virtContext = (CsirxDrv_VirtContext *)channel->drvHandle;
        instObj = virtContext->instObj;

        for(count = 0U; count<instObj->createParams.numCh; count++)
        {
            rxChHdle = (Udma_ChHandle)&instObj->chObj[count].rxChObj;
            rxChNum = Udma_chGetNum(rxChHdle);
            rCfgRegAddr = ((SAFETY_CHECKERS_CSIRX_QOS_BASE_ADDRESS_0 +
			   (rxChNum*SAFETY_CHECKERS_CSIRX_QOS_CH_REG_SIZE)) + SAFETY_CHECKERS_CSIRX_QOS_RCFG); 
            chanTypeVerif = (uint8_t)CSL_REG32_FEXT(rCfgRegAddr,
                                                    BCDMA_RXCCFG_CHAN_RCFG_CHAN_TYPE);
            mismatchCnt |= chanTypeVerif ^ qosSettings[count].chanType;

            rPriCtrlRegAddr = ((SAFETY_CHECKERS_CSIRX_QOS_BASE_ADDRESS_0 + 
                (rxChNum*SAFETY_CHECKERS_CSIRX_QOS_CH_REG_SIZE)) + SAFETY_CHECKERS_CSIRX_QOS_RPRICTRL);
            priorityVerif = (uint8_t)CSL_REG32_FEXT(rPriCtrlRegAddr,
                                                    BCDMA_RXCCFG_CHAN_RPRI_CTRL_PRIORITY);
            mismatchCnt |= priorityVerif ^ qosSettings[count].priority;
            busOrderIdVerif = (uint8_t)CSL_REG32_FEXT(rPriCtrlRegAddr,
                                                      BCDMA_RXCCFG_CHAN_RPRI_CTRL_ORDERID);
            mismatchCnt |= busOrderIdVerif ^ qosSettings[count].busOrderId;

        }

        if(mismatchCnt != 0U)
        {
            status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
        }
    }

    return status;
}

int32_t SafetyCheckers_csirxGetVimRegCfgIntrNum(uint32_t intrNum,
                                                SafetyCheckers_CsirxVimCfg *vimCfg)
{
    CSL_vimRegs *pRegs = (CSL_vimRegs*)(SAFETY_CHECKERS_CSIRX_UDMA_CSI_VIM_CONFIG_BASE_ADDRESS);
    int32_t  status = SAFETY_CHECKERS_SOK;
    uint32_t bitNum, groupNum;
    uint32_t maxIntrs = 0U, num_groups = 0U;

    if(NULL == vimCfg)
    {
        status = SAFETY_CHECKERS_FAIL;
    }
    else
    {
        maxIntrs   = pRegs->INFO;
        if (0U < maxIntrs)
        {
            num_groups = maxIntrs / CSL_VIM_NUM_INTRS_PER_GROUP;
            vimCfg->pRegs = (CSL_vimRegs*)(SAFETY_CHECKERS_CSIRX_UDMA_CSI_VIM_CONFIG_BASE_ADDRESS);
            vimCfg->intrNum = intrNum;
        }
        else
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        groupNum = intrNum / CSL_VIM_NUM_INTRS_PER_GROUP;
        if((groupNum < num_groups) && (intrNum < maxIntrs))
        {
            bitNum = intrNum & ((uint32_t)(CSL_VIM_NUM_INTRS_PER_GROUP-1U));
    
            /* Read INTMAP */
            vimCfg->intrMap  = CSL_REG32_RD(&pRegs->GRP[groupNum].INTMAP);
            /* Get the interrupt map value */
            vimCfg->intrMap  = vimCfg->intrMap >> bitNum;
            vimCfg->intrMap &= (uint32_t)(0x1U);
    
            /* Read INTTYPE */
            vimCfg->intrType  = CSL_REG32_RD(&pRegs->GRP[groupNum].INTTYPE);
            /* Get the interrupt type value */
            vimCfg->intrType  = vimCfg->intrType  >> bitNum;
            vimCfg->intrType &= (uint32_t)(0x1U);
    
            /* Read PRI */
            vimCfg->pri = CSL_REG32_RD(&pRegs->PRI[intrNum].INT);
    
            /* Read VEC */
            vimCfg->vecAddr = CSL_REG32_RD(&pRegs->VEC[intrNum].INT);
        }
    }

    return status;
}

int32_t SafetyCheckers_csirxVerifyVimRegCfgIntrNum(SafetyCheckers_CsirxVimCfg *vimCfg)
{
    CSL_vimRegs *pRegs = (CSL_vimRegs*)(SAFETY_CHECKERS_CSIRX_UDMA_CSI_VIM_CONFIG_BASE_ADDRESS);
    int32_t  status = SAFETY_CHECKERS_SOK;
    uint32_t bitNum, groupNum, intrNum;
    uint32_t intrMapVal, intrTypeVal, priVal, vecVal;
    uint32_t maxIntrs = 0U, num_groups = 0U;
    uint32_t mismatchCnt = 0U;

    if(NULL == vimCfg)
    {
        status = SAFETY_CHECKERS_FAIL;
    }
    else
    {
        maxIntrs   = pRegs->INFO;
        if (0U < maxIntrs)
        {
            num_groups = maxIntrs / CSL_VIM_NUM_INTRS_PER_GROUP;
            intrNum    = vimCfg->intrNum;
            groupNum   = intrNum / CSL_VIM_NUM_INTRS_PER_GROUP;
        }
        else
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        if((groupNum < num_groups) && (intrNum < maxIntrs))
        {
            bitNum = intrNum & (CSL_VIM_NUM_INTRS_PER_GROUP-1U);
    
            /* Read INTMAP */
            intrMapVal = CSL_REG32_RD(&pRegs->GRP[groupNum].INTMAP);
            /* Get the interrupt map value */
            intrMapVal   = intrMapVal >> bitNum;
            intrMapVal  &= (uint32_t)(0x1U);
            mismatchCnt |= vimCfg->intrMap ^ intrMapVal;
    
            /* Read INTTYPE */
            intrTypeVal  = CSL_REG32_RD(&pRegs->GRP[groupNum].INTTYPE);
            /* Get the interrupt type value */
            intrTypeVal  = intrTypeVal  >> bitNum;
            intrTypeVal &= (uint32_t)(0x1U);
            mismatchCnt |= vimCfg->intrType ^ intrTypeVal;
    
            /* Read PRI */
            priVal = CSL_REG32_RD(&pRegs->PRI[intrNum].INT);
            mismatchCnt |= vimCfg->pri ^ priVal;
    
            /* Read VEC */
            vecVal = CSL_REG32_RD(&pRegs->VEC[intrNum].INT);
            mismatchCnt |= vimCfg->vecAddr ^ vecVal;
        }
    }

    if(0U != mismatchCnt)
    {
        status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
    }

    return status;
}

int32_t SafetyCheckers_csirxi2c8BitRegRd(void   *handle,
                                         uint32_t slaveAddr,
                                         const SafetyCheckers_CsirxI2cTrxnCfg* i2cTrxnCfg)
{

    int32_t  ret = SAFETY_CHECKERS_SOK;
    I2C_Transaction transaction;
    uint8_t regAddrLocal = i2cTrxnCfg->regAddr;

    I2C_Handle i2cHandle = (I2C_Handle)handle;

    /* Initializes the I2C transaction structure with default values */
    I2C_transactionInit(&transaction);

    transaction.slaveAddress = slaveAddr;
    transaction.writeBuf     = &regAddrLocal;
    transaction.writeCount   = 1;
    transaction.readBuf      = NULL;
    transaction.readCount    = 0;
    transaction.timeout      = i2cTrxnCfg->i2cTimeout;

    ret = I2C_transfer(i2cHandle, &transaction);
    if(I2C_STS_SUCCESS != ret)
    {
        ret = SAFETY_CHECKERS_FAIL;
    }
    else
    {
        transaction.writeBuf     = NULL;
        transaction.writeCount   = 0;
        transaction.readBuf      = i2cTrxnCfg->regData;
        transaction.readCount    = i2cTrxnCfg->numOfBytes;

        ret = I2C_transfer(i2cHandle, &transaction);
        if(I2C_STS_SUCCESS != ret)
        {
            ret = SAFETY_CHECKERS_FAIL;
        }
        else
        {
             ret = SAFETY_CHECKERS_SOK;
        }
	
    }

    return ret;
}
