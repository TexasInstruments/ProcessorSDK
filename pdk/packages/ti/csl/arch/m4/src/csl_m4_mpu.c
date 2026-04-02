/*
 *  Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   csl_r5.c
 *
 *  \brief  Pulsar/R5 SS related functions.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <csl_arm_mpu.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/*  Declarations    */
/**
 * \brief  TEX[2:0], C and B values.
 *         CSL_ArmM4MemAttr is used as intex here.
 *         gMemAttr[x][0]: TEX[2:0] values
 *         gMemAttr[x][1]: C bit value
 *         gMemAttr[x][2]: B bit value
 */
static const uint32_t gMemAttr[7U][3U] =
{
/*    TEX[2:0], C,     B bits */
    {   0x0U,   0x0U,  0x0U,}, /* Strongly-ordered.*/
    {   0x0U,   0x0U,  0x1U,}, /* Shareable Device.*/
    {   0x0U,   0x1U,  0x0U,}, /* Outer and Inner write-through, no write-allocate. */
    {   0x0U,   0x1U,  0x1U,}, /* Outer and Inner write-back, no write-allocate. */
    {   0x1U,   0x0U,  0x0U,}, /* Outer and Inner Non-cacheable. */
    {   0x1U,   0x1U,  0x1U,}, /* Outer and Inner write-back, write-allocate.*/
    {   0x2U,   0x0U,  0x0U,}, /* Non-shareable Device.*/
};


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
int32_t CSL_armM4MPUEnable(uint32_t enable) {
    int32_t                    retVal = CSL_PASS;
    CSL_ARM_M4_MPU_Registers * mpu = (CSL_ARM_M4_MPU_Registers *) (CSL_ARM_M4_MPU_BASE_ADDRESS);
    switch(enable) {
        case 0:
            mpu->MPU_CTRL &= ~(0x1);
            break;
        case 1:
            mpu->MPU_CTRL |= (0x1);
            break;
    }
    return (retVal);
}

int32_t CSL_armM4GetNumMPURegions(uint32_t *pNumMPURegions) {
    int32_t                    retVal = CSL_EFAIL;
    CSL_ARM_M4_MPU_Registers * mpu = (CSL_ARM_M4_MPU_Registers *) (CSL_ARM_M4_MPU_BASE_ADDRESS);
    if (pNumMPURegions != NULL_PTR)
    {
        *pNumMPURegions = (mpu->MPU_TYPE >> 8) & 0b11111111;
         retVal         = CSL_PASS;
    }
    return (retVal);
}


int32_t CSL_armM4MPUCfgRegion(const CSL_ARMM4MpuRegionCfg_t *pMPUCfg)
{
    uint32_t                   tex;
    uint32_t                   baseAddrRegVal;
    uint32_t                   attrSizeRegVal;
    int32_t                    retVal = CSL_EBADARGS;
    CSL_ARM_M4_MPU_Registers  *pMpu = (CSL_ARM_M4_MPU_Registers *) (CSL_ARM_M4_MPU_BASE_ADDRESS);


    if (CSL_ARM_M4_MPU_REGION_SIZE_32B <= pMPUCfg->size)
    {
        baseAddrRegVal = 0U;
        attrSizeRegVal = 0U;
        baseAddrRegVal   |= ( pMPUCfg->baseAddr &
                                CSL_ARM_M4_MPU_RBAR_ADDR_MASK);
        attrSizeRegVal   |= ( pMPUCfg->enable <<
                                CSL_ARM_M4_MPU_RASR_REGION_EN_SHIFT);
        attrSizeRegVal   |= ( pMPUCfg->size <<
                                CSL_ARM_M4_MPU_RASR_REGION_SIZE_SHIFT);
        attrSizeRegVal   |= ( pMPUCfg->subRegionEnable <<
                                CSL_ARM_M4_MPU_RASR_SUB_REGION_DISABLE_SHIFT);

        attrSizeRegVal   |= ( pMPUCfg->exeNeverControl <<
                               CSL_ARM_M4_MPU_RASR_ATTRS_XN_SHIFT);
        attrSizeRegVal   |= ( pMPUCfg->accessPermission <<
                               CSL_ARM_M4_MPU_RASR_ATTRS_AP_SHIFT);
        attrSizeRegVal   |= ( pMPUCfg->shareable <<
                               CSL_ARM_M4_MPU_RASR_ATTRS_S_SHIFT);

        if (pMPUCfg->cacheable == (uint32_t)TRUE)
        {
            /*
              When cacheable is on
              TEX[2] = 1
              TEX[1:0] = cachePolicy value of iteration
              C,B     = cachePolicy value of iteration
            */
            tex = (1U << 2U);
            tex |= (pMPUCfg->cachePolicy);
            attrSizeRegVal |=
                            ( tex << CSL_ARM_M4_MPU_RASR_ATTRS_TEX_SHIFT);
            attrSizeRegVal |= ( pMPUCfg->cachePolicy <<
                               CSL_ARM_M4_MPU_RASR_ATTRS_CB_SHIFT);
        }
        else
        {
            /*
              When cacheable is off
              TEX[2:0] = gMemAttr[memAttr value of iteration][0 = TEX values]
                 All of these have TEX[2] = 0
              C       = gMemAttr[memAttr value of iteration][1 = C]
              B       = gMemAttr[memAttr value of iteration][2 = B]
              Effectively, when cacheable is off, this part of it will set it equal to gMemAttr
                 They all have TEX[2] = 0
            */
            tex = gMemAttr[pMPUCfg->memAttr][0U];
            attrSizeRegVal |=
                            ( tex << CSL_ARM_M4_MPU_RASR_ATTRS_TEX_SHIFT);
            attrSizeRegVal |=
                            ( gMemAttr[pMPUCfg->memAttr][1U] <<
                            CSL_ARM_M4_MPU_RASR_ATTRS_C_SHIFT);
            attrSizeRegVal |=
                            ( gMemAttr[pMPUCfg->memAttr][2U] <<
                            CSL_ARM_M4_MPU_RASR_ATTRS_B_SHIFT);
        }
        pMpu->MPU_RNR = pMPUCfg->regionId;
        pMpu->MPU_RBAR = baseAddrRegVal;
        pMpu->MPU_RASR = attrSizeRegVal;
        retVal = CSL_PASS;
    }
    return (retVal);
}

/********************************* End of file ******************************/
