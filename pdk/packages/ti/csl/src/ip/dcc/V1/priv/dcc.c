/*
 *   Copyright (c) Texas Instruments Incorporated 2015
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
 *  \file     dcc.c
 *
 *  \brief    This file contains the implementation of the APIs present in the
 *            device abstraction layer file of DCC.
 *            This also contains some related macros.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/** \brief This is to disable HW_SYNC_BARRIER for register access */
#define MEM_BARRIER_DISABLE

#include <stdint.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr_dcc.h>
#include <ti/csl/csl_dcc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
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
int32_t DCCSetConfig(uint32_t baseAddr, const dccConfigParams_t *configParams)
{
    int32_t status = STW_EFAIL;
    uint32_t clksrcVal;
    if ((DCC_SRC0_COUNT_MAX >= configParams->seedSrc0) &&
        (DCC_SRC0_VALID_MAX >= configParams->seedValid0) &&
        (DCC_SRC1_COUNT_MAX >= configParams->seedSrc1))
    {
        /* Configure DCC mode of operation */
        HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL, CSL_DCC2_DCCGCTRL_SINGLESHOT,
                      configParams->mode);
        RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCGCTRL)), 
                      DCC2_DCCGCTRL_SINGLESHOT, configParams->mode, DCCReadbackErrCbk);
        /* Select clock source for COUNT0 */
        clksrcVal = ((configParams->clkSrc0 & CSL_DCC2_DCCCLKSRC0_CLKSRC0_MASK) |
                     (CSL_DCC2_DCCCLKSRC0_KEY_ENABLE << CSL_DCC2_DCCCLKSRC0_KEY_SHIFT));
        HW_WR_REG32(baseAddr + CSL_DCC2_DCCCLKSRC0, clksrcVal);
        RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCCLKSRC0)), 
                      DCC2_DCCCLKSRC0_CLKSRC0, configParams->clkSrc0, DCCReadbackErrCbk);

        if(configParams->alternateClkSrc1 == (uint32_t)TRUE)
        {
            /* Enable alternate clock source selection for COUNT1 */
            HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCCLKSRC1, CSL_DCC2_DCCCLKSRC1_KEY,
                          CSL_DCC2_DCCCLKSRC1_KEY_DISABLE);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCCLKSRC1)),
                          DCC2_DCCCLKSRC1_KEY, CSL_DCC2_DCCCLKSRC1_KEY_DISABLE, DCCReadbackErrCbk);
        }
        else
        {
            /* Select clock source for COUNT1*/
            /* Enable clock source selection for COUNT1 */
            clksrcVal = ((configParams->clkSrc1 % 16U << CSL_DCC2_DCCCLKSRC1_CLKSRC1_SHIFT) |
                         (CSL_DCC2_DCCCLKSRC1_KEY_ENABLE << CSL_DCC2_DCCCLKSRC1_KEY_SHIFT));
            HW_WR_REG32(baseAddr + CSL_DCC2_DCCCLKSRC1, clksrcVal);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCCLKSRC1)),
                          DCC2_DCCCLKSRC1_CLKSRC1, configParams->clkSrc1, DCCReadbackErrCbk);
        }
        /* Configure COUNT0 preload/seed value */
        HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCCNTSEED0, CSL_DCC2_DCCCNT0_COUNT0,
                      configParams->seedSrc0);
        RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCCNTSEED0)),
                          DCC2_DCCCNT0_COUNT0, configParams->seedSrc0, DCCReadbackErrCbk);
        if(configParams->seedValid0 < 4U)
        {
            /* Configure VALID0 preload/seed value */
            HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCVALIDSEED0, CSL_DCC2_DCCVALID0_VALID0,
                          4U);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCVALIDSEED0)),
                          DCC2_DCCVALID0_VALID0, 4U, DCCReadbackErrCbk);
        }
        else
        {
            /* Configure VALID0 preload/seed value */
            HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCVALIDSEED0, CSL_DCC2_DCCVALID0_VALID0,
                          configParams->seedValid0);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCVALIDSEED0)),
                          DCC2_DCCVALID0_VALID0, configParams->seedValid0, DCCReadbackErrCbk);
        }
        /* Configure COUNT1 preload/seed value */
        HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCCNTSEED1, CSL_DCC2_DCCCNT1_COUNT1,
                      configParams->seedSrc1);
        RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCCNTSEED1)),
                          DCC2_DCCCNT1_COUNT1, configParams->seedSrc1, DCCReadbackErrCbk);
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }
    return status;
}

void DCCEnable(uint32_t baseAddr, uint32_t enable)
{
    if ((uint32_t) TRUE == enable)
    {
        /* Enable DCC */
        HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL, CSL_DCC2_DCCGCTRL_DCCENA,
                      CSL_DCC2_DCCGCTRL_DCCENA_ENABLE);
        RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCGCTRL)),
                          DCC2_DCCGCTRL_DCCENA, CSL_DCC2_DCCGCTRL_DCCENA_ENABLE, DCCReadbackErrCbk);
    }
    else
    {
        /* Disable DCC */
        HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL, CSL_DCC2_DCCGCTRL_DCCENA,
                      CSL_DCC2_DCCGCTRL_DCCENA_DISABLE);
        RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCGCTRL)),
                          DCC2_DCCGCTRL_DCCENA, CSL_DCC2_DCCGCTRL_DCCENA_DISABLE, DCCReadbackErrCbk);
    }
}

int32_t DCCEnableIntr(uint32_t baseAddr, uint32_t intrType)
{
    int32_t status = STW_EFAIL;
    switch (intrType)
    {
        case DCC_INTERRUPTS_ERROR:
            /* Enable ERROR interrupt */
            HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL, CSL_DCC2_DCCGCTRL_ERRENA,
                          CSL_DCC2_DCCGCTRL_ERRENA_ENABLE);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCGCTRL)),
                          DCC2_DCCGCTRL_ERRENA, CSL_DCC2_DCCGCTRL_ERRENA_ENABLE, DCCReadbackErrCbk);
            status = STW_SOK;
            break;
        case DCC_INTERRUPTS_DONE:
            /* Enable DONE interrupt(only for single shot mode) */
            HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL, CSL_DCC2_DCCGCTRL_DONEENA,
                          CSL_DCC2_DCCGCTRL_DONEENA_ENABLE);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCGCTRL)),
                          DCC2_DCCGCTRL_DONEENA, CSL_DCC2_DCCGCTRL_DONEENA_ENABLE, DCCReadbackErrCbk);
            status = STW_SOK;
            break;
        default:
            status = STW_EBADARGS;
            break;
    }
    return status;
}

int32_t DCCDisableIntr(uint32_t baseAddr, uint32_t intrType)
{
    int32_t status = STW_EFAIL;
    switch (intrType)
    {
        case DCC_INTERRUPTS_ERROR:
            /* Disable ERROR interrupt */
            HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL, CSL_DCC2_DCCGCTRL_ERRENA,
                          CSL_DCC2_DCCGCTRL_ERRENA_DISABLE);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCGCTRL)),
                          DCC2_DCCGCTRL_ERRENA, CSL_DCC2_DCCGCTRL_ERRENA_DISABLE, DCCReadbackErrCbk);
            status = STW_SOK;
            break;
        case DCC_INTERRUPTS_DONE:
            /* Disable DONE interrupt(only for single shot mode) */
            HW_WR_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL, CSL_DCC2_DCCGCTRL_DONEENA,
                          CSL_DCC2_DCCGCTRL_DONEENA_DISABLE);
            RB_CHECK_FIELD32_VAL(*((volatile uint32_t *)((uintptr_t)baseAddr + CSL_DCC2_DCCGCTRL)),
                          DCC2_DCCGCTRL_DONEENA, CSL_DCC2_DCCGCTRL_DONEENA_DISABLE, DCCReadbackErrCbk);
            status = STW_SOK;
            break;
        default:
            status = STW_EBADARGS;
            break;
    }
    return status;
}

uint32_t DCCGetIntrStatus(uint32_t baseAddr)
{
    /* Read and return DCC status register */
    return (HW_RD_REG32(baseAddr + CSL_DCC2_DCCSTATUS));
}

void DCCClearIntrStatus(uint32_t baseAddr, uint32_t intrMask)
{
    /* Clear status of given interrupt */
    HW_WR_REG32(baseAddr + CSL_DCC2_DCCSTATUS, ((uint32_t) 0x1U << intrMask));
}

void DCCGetConfig(uint32_t baseAddr, dccConfigParams_t *configParams)
{
    /* Get the configured mode of operation for DCC module */
    configParams->mode = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCGCTRL,
                                       CSL_DCC2_DCCGCTRL_SINGLESHOT);
    /* Get the clock source for COUNT0 */
    configParams->clkSrc0 = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCCLKSRC0,
                                          CSL_DCC2_DCCCLKSRC0_CLKSRC0);
    /* Get the clock source for COUNT1 */
    configParams->clkSrc1 = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCCLKSRC1,
                                          CSL_DCC2_DCCCLKSRC1_CLKSRC1);
    /* Get the current value of COUNT0 */
    configParams->seedSrc0 = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCCNTSEED0,
                                           CSL_DCC2_DCCCNT0_COUNT0);
    /* Get the current value of VALID0 */
    configParams->seedValid0 = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCVALIDSEED0,
                                             CSL_DCC2_DCCVALID0_VALID0);
    /* Get the current value of COUNT1 */
    configParams->seedSrc1 = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCCNTSEED1,
                                           CSL_DCC2_DCCCNT1_COUNT1);
}

void DCCGetRevisionId(uint32_t baseAddr, dccRevisionId_t *revId)
{
    /* Get the scheme */
    revId->scheme = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCREV, CSL_DCC2_DCCREV_SCHEME);
    /* Get the Functional release number*/
    revId->func = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCREV, CSL_DCC2_DCCREV_FUNC);
    /* Get the Design Release Number */
    revId->rtlReleaseNum = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCREV, CSL_DCC2_DCCREV_RTL);
    /* Get the Major Revision Number */
    revId->major = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCREV, CSL_DCC2_DCCREV_MAJOR);
    /* Get a special version of the module */
    revId->custom = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCREV, CSL_DCC2_DCCREV_CUSTOM);
    /* Get the Minor revision number */
    revId->minor = HW_RD_FIELD32(baseAddr + CSL_DCC2_DCCREV, CSL_DCC2_DCCREV_MINOR);
}

__attribute__((weak)) void DCCReadbackErrCbk(uint32_t expVal, uint32_t readVal)
{
    // this is a default empty function
}
