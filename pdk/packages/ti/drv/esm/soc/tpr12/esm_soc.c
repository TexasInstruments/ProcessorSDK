/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   esm_soc.c
 *
 *  \brief  TPR12 device specific hardware attributes.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <ti/csl/soc.h>
#include <ti/drv/esm/soc/esm_soc.h>
#include <ti/drv/esm/src/esm_internal.h>

#include <ti/csl/soc/tpr12/src/cslr_mss_ctrl.h>
#include <ti/csl/soc/tpr12/src/cslr_dss_ctrl.h>

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

#define CSL_ESM_PER_CNT    1U

/* Master control block for ESM driver */
ESM_DriverMCB   gEsmMCB[CSL_ESM_PER_CNT];

#define ESM_NUM_EVENTS_GROUP1 ESM_NUM_INTR_PER_GROUP
/**
 * @brief   This is TPR12 ESM platform configuration for MSS/DSS Subsystem.
 */
ESM_HwAttrs gESMHwCfgAttrs[CSL_ESM_PER_CNT] =
{
    {
#if defined (__TI_ARM_V7R4__)
        (CSL_esmRegs*)CSL_MSS_ESM_U_BASE,
        ESM_NUM_EVENTS_GROUP1,
        CSL_MSS_INTR_MSS_ESM_HI,
        CSL_MSS_INTR_MSS_ESM_LO
#elif defined (_TMS320C6X)
        (CSL_esmRegs*)CSL_DSS_ESM_U_BASE,
        ESM_NUM_EVENTS_GROUP1,
        /* Not used: the DSS ESM high priority interrrupt is an NMI and
         * first captured by the RTOS NMI exception handler  */
        0,
        CSL_DSS_INTR_DSS_ESM_LO
#else
#error "TPR12 ESM: unsupported core"
#endif
    }
};


CSL_PUBLIC_CONST ESM_Config ESM_config[] =
{
    {
        (void *)&gEsmMCB[0],               /* ESM Driver Object             */
        (void *)&gESMHwCfgAttrs[0]         /* ESM Hw configuration          */
    },
    {
        NULL,
        NULL
    }
};

/**************************************************************************
 ************************* Functions **************************************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      This function is used to gate/ungate an ESM error
 *
 *  @param[in]  groupNumber
 *      Group number for the ESM error
 *  @param[in]  errorNumber
 *      ESM error number
 *  @param[in]  gating
 *      Gating of the ESM error: 0 - ungating; 1 - gating
  *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
/* Definitions for operating on one ESM_GATING register */
#define MASK   (0xFU) /* Four bits are used for each ESM error, creating a mask of 0xF      */
#define SHIFT  (0x4U) /* 4-bit Shift from one ESM error to the next one                     */
#define COUNT  (0x8U) /* Each ESM_GATING register handles 8 ESM errors                      */

/* 4 byte address offset from one ESM_GATING register to the next one */
#define OFFSET (0x4U)

/* 4 ESM_GATING registers for group 2, followed by the registers fo group 3 */
#define GROUP  (0x4U)

int32_t ESM_socConfigErrorGating(uint8_t groupNumber, uint8_t errorNumber, uint8_t gating)
{
    uint32_t regVal;
    uint32_t regIndex;
    uint32_t regAddr;

#if defined (__TI_ARM_V7R4__)
    CSL_mss_ctrlRegs* ptrCtrlRegs = (CSL_mss_ctrlRegs*)CSL_MSS_CTRL_U_BASE;
#elif defined (_TMS320C6X)
    CSL_mss_ctrlRegs* ptrCtrlRegs = (CSL_mss_ctrlRegs*)CSL_DSS_CTRL_U_BASE;
#else
#error "TPR12 ESM: unsupported core"
#endif

    /* Error gating is for Group 2  and Group 3 errors only */
    if ((groupNumber < 2) || (groupNumber > 3))
    {
        return -1;
    }

    /* For Group 2 and Group 3, there are up to 32 errors for each*/
    if (errorNumber > 32)
    {
        return -1;
    }

    regIndex = errorNumber / COUNT + (groupNumber-2) * GROUP;
    regAddr  = (uint32_t)&ptrCtrlRegs->ESM_GATING0 + regIndex * OFFSET;

    regVal = CSL_REG_RD((volatile uint32_t *)regAddr);
    regVal &= ~(MASK << (SHIFT*(errorNumber % COUNT)));
    if (gating)
    {
        regVal |= (MASK << (SHIFT*(errorNumber % COUNT)));
    }
    CSL_REG_WR((volatile uint32_t *)regAddr, regVal);

    return 0;
}

/**
 * \brief  This API gets the SoC level of ESM initial configuration
 *
 * \param  index     ESM instance index.
 * \param  cfg       Pointer to ESM SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t ESM_socGetInitCfg(uint32_t index, ESM_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_ESM_PER_CNT)
    {
        *cfg = gESMHwCfgAttrs[index];
    }
    else
    {
        ret = (-((int32_t)1));
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of ESM intial configuration
 *
 * \param  index     ESM instance index.
 * \param  cfg       Pointer to ESM SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t ESM_socSetInitCfg(uint32_t index, const ESM_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_ESM_PER_CNT)
    {
        gESMHwCfgAttrs[index] = *cfg;
    }
    else
    {
        ret = (-((int32_t)1));
    }

    return ret;
}

