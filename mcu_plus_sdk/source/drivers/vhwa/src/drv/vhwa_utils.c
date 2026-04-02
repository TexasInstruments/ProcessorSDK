/**
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file vhwa_utils.c
 *
 *  \brief Utility APIs Implementation
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "src/drv/vhwa_utils.h"
#include <include/vhwa_common.h>
#include "src/csl/include/csl_lse.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */



/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
#if !defined(SOC_AM62A) && !defined(SOC_AM62AX)
uint64_t gDmpacSl2Addr = DMPAC_SL2_START_ADDRESS;
uint32_t gDmpacSl2Size = (uint32_t)DMPAC_SL2_SIZE;
#endif /* DMPAC not applicable for AM62A */
uint64_t gVpacSl2Addr = VPAC_SL2_START_ADDRESS;
uint32_t gVpacSl2Size = (uint32_t)VPAC_SL2_SIZE;
uint32_t gVpacDrvierOpen = 0;
uint32_t gDmpacDrvierOpen = 0;

VhwaAl_SemaphoreP_Struct gHwaLock[VHWA_MAX_LOCK] = {0};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

uint32_t Vhwa_calcSl2Pitch(uint32_t imgPitch)
{
    uint32_t sl2Pitch;

    /* First convert Img Pitch to Sl2 Aligned pitch */
    sl2Pitch = CSL_lseMakePitchAligned(imgPitch);

    return (sl2Pitch);
}

uint32_t Vhwa_ceil(uint32_t num, uint32_t den)
{
     return ((0u == (num%den)) ? (num/den) : ((num/den) + (uint32_t)1u));
}

uint64_t Vhwa_allocateSl2(uint32_t allocSize, uint32_t sl2Inst)
{
    uint64_t sl2Addr = (uint64_t)NULL;

    #if !defined(SOC_AM62A) && !defined(SOC_AM62AX)

    if(sl2Inst == (uint32_t)VHWA_SL2_INST_DMPAC)
    {
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale:
        The allocation size checks in Vhwa_allocateSl2 prevent SL2 memory overrun for DMPAC IPs. These checks are necessary for system safety,
        but cannot be covered in tests because the allocation parameters are hardcoded and not configurable by the test application.
        Effect on this unit:
        If SL2 allocation fails due to insufficient memory, the function returns NULL and prevents overrun.
        The application can then handle the error without causing system instability.
        <justification end> */
        if(gDmpacSl2Size >= allocSize)
        /* LDRA_JUSTIFY_END */
        {
            sl2Addr = gDmpacSl2Addr;
            gDmpacSl2Addr += allocSize;
            gDmpacSl2Size -= allocSize;
            gDmpacDrvierOpen++;
        }
    }
    else
    #endif /* DMPAC not applicable for AM62A */
/* LDRA_JUSTIFY_START
<metric start> branch <metric end>
<justification start> Rationale: The negative test framework and test apps cannot reach this portion as this function is calling from
                      module's API.c files only for 'VHWA_SL2_INST_DMPAC' and 'VHWA_SL2_INST_VPAC' values of sl2Inst.
                    Effect on this unit - control cannot reach negative value here.
<justification end> */
    if(sl2Inst == (uint32_t)VHWA_SL2_INST_VPAC)
/* LDRA_JUSTIFY_END */
    {
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale:
        The allocation size checks in Vhwa_allocateSl2 prevent SL2 memory overrun for VPAC IPs. These checks are necessary for system safety,
        but cannot be covered in tests because the allocation parameters are hardcoded and not configurable by the test application.
        Effect on this unit:
        If SL2 allocation fails due to insufficient memory, the function returns NULL and prevents overrun.
        The application can then handle the error without causing system instability.
        <justification end> */
        if(gVpacSl2Size >= allocSize)
        /* LDRA_JUSTIFY_END */
        {
            sl2Addr = gVpacSl2Addr;
            gVpacSl2Addr += allocSize;
            gVpacSl2Size -= allocSize;
            gVpacDrvierOpen++;
        }
    }
/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> Rationale: The negative test framework and test apps cannot reach this portion as this function is calling from
                      module's API.c files only for 'VHWA_SL2_INST_DMPAC' and 'VHWA_SL2_INST_VPAC' values of sl2Inst.
                    Effect on this unit - control cannot reach here.
<justification end> */
    else
    {
        /* Invalid SL2_INST */
    }
/* LDRA_JUSTIFY_END */

    return sl2Addr;
}

uint64_t Vhwa_allocateSl2Aligned(uint32_t allocSize, uint32_t sl2Inst, uint32_t alignment)
{
    uint64_t sl2Addr = (uint64_t)NULL;
    int32_t  status = FVID2_SOK;

    /* Validate that alignment is a power of 2 */
    /* Power of 2 check: a number is power of 2 if (n != 0) && ((n & (n-1)) == 0) */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if ((alignment == 0U) || ((alignment & (alignment - 1U)) != 0U))
    {
        /* alignment is not a power of 2 - return 0 to indicate error */
        /* This prevents silent memory corruption from incorrect alignment mask calculation */
        /* Driver should check for 0 return and print appropriate error message */
        status = FVID2_EINVALID_PARAMS;
    }
    /* LDRA_JUSTIFY_END */

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This failure case is out of scope for the imaging test framework.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(FVID2_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        #if !defined(SOC_AM62A) && !defined(SOC_AM62AX)

        if(sl2Inst == (uint32_t)VHWA_SL2_INST_DMPAC)
        {
            uint64_t alignedAddr = gDmpacSl2Addr;
            uint32_t alignmentOffset = 0U;
            uint32_t alignmentMask = alignment - 1U;

            /* Ensure address is aligned to requested alignment */
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if ((gDmpacSl2Addr & alignmentMask) != 0U)
            {
                /* Round up to next alignment boundary */
                alignedAddr = (gDmpacSl2Addr + alignmentMask) & ~alignmentMask;
                alignmentOffset = (uint32_t)(alignedAddr - gDmpacSl2Addr);
            }
            /* LDRA_JUSTIFY_END */

            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if(gDmpacSl2Size >= (allocSize + alignmentOffset))
            /* LDRA_JUSTIFY_END */
            {
                sl2Addr = alignedAddr;
                gDmpacSl2Addr = alignedAddr + allocSize;
                gDmpacSl2Size -= (allocSize + alignmentOffset);
                gDmpacDrvierOpen++;
            }
        }
        else
        #endif /* DMPAC not applicable for AM62A */
        if(sl2Inst == (uint32_t)VHWA_SL2_INST_VPAC)
        {
            uint64_t alignedAddr = gVpacSl2Addr;
            uint32_t alignmentOffset = 0U;
            uint32_t alignmentMask = alignment - 1U;

            /* Ensure address is aligned to requested alignment */
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if ((gVpacSl2Addr & alignmentMask) != 0U)
            {
                /* Round up to next alignment boundary */
                alignedAddr = (gVpacSl2Addr + alignmentMask) & ~alignmentMask;
                alignmentOffset = (uint32_t)(alignedAddr - gVpacSl2Addr);
            }
            /* LDRA_JUSTIFY_END */

            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            The test framework does not support the configuration required to trigger this error scenario.
            Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
            This behaviour is part of the application design. An error print statement can be added in a future release if required.
            <justification end> */
            if(gVpacSl2Size >= (allocSize + alignmentOffset))
            /* LDRA_JUSTIFY_END */
            {
                sl2Addr = alignedAddr;
                gVpacSl2Addr = alignedAddr + allocSize;
                gVpacSl2Size -= (allocSize + alignmentOffset);
                gVpacDrvierOpen++;
            }
        }
        else
        {
            /* Invalid SL2_INST */
        }
    }

    return sl2Addr;
}

void Vhwa_FreeSl2(uint32_t sl2Inst)
{
    #if !defined(SOC_AM62A) && !defined(SOC_AM62AX)
    if(sl2Inst == (uint32_t)VHWA_SL2_INST_DMPAC)
    {
        gDmpacDrvierOpen--;
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        if(0U == gDmpacDrvierOpen)
        /* LDRA_JUSTIFY_END */
        {
            gDmpacSl2Addr = DMPAC_SL2_START_ADDRESS;
            gDmpacSl2Size = (uint32_t)DMPAC_SL2_SIZE;
        }
    }
    else
    #endif /* DMPAC not applicable for AM62A */
/* LDRA_JUSTIFY_START
<metric start> branch <metric end>
<justification start> Rationale: The negative test framework and test apps cannot reach this portion as this function is calling from
                      module's API.c files only for 'VHWA_SL2_INST_DMPAC' and 'VHWA_SL2_INST_VPAC' values of sl2Inst.
                    Effect on this unit - control cannot reach negative value here.
<justification end> */
    if(sl2Inst == (uint32_t)VHWA_SL2_INST_VPAC)
/* LDRA_JUSTIFY_END */
    {
        gVpacDrvierOpen--;
        if(0U == gVpacDrvierOpen)
        {
            gVpacSl2Addr = VPAC_SL2_START_ADDRESS;
            gVpacSl2Size = (uint32_t)VPAC_SL2_SIZE;
        }
    }
/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> Rationale: The negative test framework and test apps cannot reach this portion as this function is calling from
                      module's API.c files only for 'VHWA_SL2_INST_DMPAC' and 'VHWA_SL2_INST_VPAC' values of sl2Inst.
                    Effect on this unit - control cannot reach here.
<justification end> */
    else
    {
        /* Invalid SL2_INST */
    }
/* LDRA_JUSTIFY_END */
}

uint32_t Vhwa_calcHorzSizeInBytes(uint32_t width, uint32_t ccsf)
{
    uint32_t sizeInBytes;

    switch ((Fvid2_ColorCompStorageFmt)ccsf)
    {
        default:
            sizeInBytes = width;
            break;
        case FVID2_CCSF_BITS8_PACKED:
            sizeInBytes = width;
            break;
        case FVID2_CCSF_BITS12_PACKED:
            sizeInBytes = (width * 3U / 2U);
            break;
        case FVID2_CCSF_BITS8_UNPACKED16_MSB_ALIGNED:
        case FVID2_CCSF_BITS8_UNPACKED16:
            sizeInBytes = width * 2U;
            break;
        case FVID2_CCSF_BITS10_UNPACKED16_MSB_ALIGNED:
        case FVID2_CCSF_BITS10_UNPACKED16:
            sizeInBytes = width * 2U;
            break;
        case FVID2_CCSF_BITS12_UNPACKED16_MSB_ALIGNED:
        case FVID2_CCSF_BITS12_UNPACKED16:
            sizeInBytes = width * 2U;
            break;
        case FVID2_CCSF_BITS14_UNPACKED16:
            sizeInBytes = width * 2U;
            break;
        case FVID2_CCSF_BITS16_PACKED:
            sizeInBytes = width * 2U;
            break;
    }

    return (sizeInBytes);
}
void vhwa_GetErrIntrStat(volatile const uint32_t *intdErrReg,volatile uint32_t *intdClrReg,  uint32_t *eeStat, uint32_t errEventsMask)
{
    volatile uint32_t regVal;

    *eeStat = 0U;
    /* Check if any of the error event has occurred */
    regVal = *intdErrReg;  /* Direct dereference respects const volatile qualifiers */
    regVal &= errEventsMask;

    if (0U != regVal)
    {
        /* First clear all interrupts, so that the interrupt does
            * not come again for these events */
        CSL_REG32_WR(intdClrReg, regVal);

        /* Store the error status */
        *eeStat = regVal;
    }

}
void vhwa_GetWdtimerErrIntrStat(const volatile uint32_t *intdErrReg,volatile uint32_t *intdClrReg,  uint32_t *wdTimer_errStat,
        uint32_t wdTimer_bit)
{
    volatile uint32_t regVal;

    *wdTimer_errStat = 0U;
    /* Check if any of the error event has occurred */
    regVal = *intdErrReg;  /* Direct dereference respects const volatile qualifiers */
/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start>
Rationale: The component level negative test framework and test applications cannot reach this portion.
The test framework does not support the configuration required to trigger this error scenario.
Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
This behaviour is part of the application design. An error print statement can be added in a future release if required.
<justification end> */
    if (0U != (regVal & ((uint32_t)1u << wdTimer_bit)))
    {
        regVal = ((uint32_t)1u << wdTimer_bit);
        /* First clear all interrupts, so that the interrupt does
            * not come again for these events */
        CSL_REG32_WR(intdClrReg, regVal);

        /* Store the Wdtimer error status */
        *wdTimer_errStat = regVal;
    }
/* LDRA_JUSTIFY_END */
}
void Vhwa_enableHtsIntr(CSL_vpac_intd_cfgRegs *intdRegs, uint32_t irqNum,
    uint32_t htsPipelineNum)
{
    volatile uint32_t regVal;

    /* Check for Null pointer */
    GT_assert(VhwaMscTrace, (NULL != intdRegs));

    /* Enable HTS Interrupt in INTD Module */
    regVal = ((uint32_t)1u << htsPipelineNum);
    CSL_REG32_WR(&intdRegs->ENABLE_REG_LEVEL_VPAC_OUT[irqNum][VHWA_VPAC_INTD_PIPE_INTR_REG], regVal);
}

void Vhwa_disableHtsIntr(CSL_vpac_intd_cfgRegs *intdRegs, uint32_t irqNum,
    uint32_t htsPipelineNum)
{
    volatile uint32_t regVal;

    /* Check for Null pointer */
    GT_assert(VhwaMscTrace, (NULL != intdRegs));

    /* Disable HTS Interrupt in INTD Module */
    regVal = ((uint32_t)1u << htsPipelineNum);
    CSL_REG32_WR(&intdRegs->ENABLE_CLR_REG_LEVEL_VPAC_OUT[irqNum][VHWA_VPAC_INTD_PIPE_INTR_REG], regVal);

}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> Rationale: The test framework and test apps cannot reach this portion
    The function is unused because interrupt status clear is directly done form ISR,, this code base is not being used/enabled with the current kernel support.
    In future this might be a modular way to clear the interrupt status.
Effect on this unit - None;Unused feature, cannot be enabled, control cannot reach here.
<justification end> */
void Vhwa_clearHtsIntr(CSL_vpac_intd_cfgRegs *intdRegs, uint32_t irqNum,
    uint32_t htsPipelineNum)
{
    volatile uint32_t     regVal;

    /* Check for Null pointer */
    GT_assert(VhwaMscTrace, (NULL != intdRegs));

    regVal = CSL_REG32_RD(&intdRegs->STATUS_REG_LEVEL_VPAC_OUT[irqNum][3U]);
    if (0u != (regVal & ((uint32_t)1u << htsPipelineNum)))
    {
        regVal = (uint32_t)1u << htsPipelineNum;
        CSL_REG32_WR(&intdRegs->STATUS_CLR_REG_LEVEL_VPAC_OUT[irqNum][3U],
            regVal);
    }
}
/* LDRA_JUSTIFY_END */

#if !defined (SOC_AM62A) && !defined(SOC_AM62AX)
void Vhwa_enableDmpacHtsIntr(CSL_dmpac_intd_cfgRegs *intdRegs, uint32_t irqNum,
    uint32_t htsPipelineNum)
{
    volatile uint32_t regVal;

    /* Enable HTS Interrupt in INTD Module */
    regVal = ((uint32_t)1u << htsPipelineNum);
    CSL_REG32_WR(&intdRegs->ENABLE_REG_LEVEL_DMPAC_OUT[irqNum][VHWA_DMPAC_INTD_PIPE_INTR_REG], regVal);
}

void Vhwa_disableDmpacHtsIntr(CSL_dmpac_intd_cfgRegs *intdRegs, uint32_t irqNum,
    uint32_t htsPipelineNum)
{
    volatile uint32_t regVal;

    /* Disable HTS Interrupt in INTD Module */
    regVal = ((uint32_t)1u << htsPipelineNum);
    CSL_REG32_WR(&intdRegs->ENABLE_CLR_REG_LEVEL_DMPAC_OUT[irqNum][VHWA_DMPAC_INTD_PIPE_INTR_REG], regVal);

}

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start> Rationale: The test framework and test apps cannot reach this portion
    The function is unused because interrupt status clear is directly done form ISR,, this code base is not being used/enabled with the current kernel support.
    In future this might be a modular way to clear the interrupt status.
Effect on this unit - None;Unused feature, cannot be enabled, control cannot reach here.
<justification end> */
void Vhwa_clearDmpacHtsIntr(CSL_dmpac_intd_cfgRegs *intdRegs, uint32_t irqNum,
    uint32_t htsPipelineNum)
{
    volatile uint32_t     regVal;

    regVal = CSL_REG32_RD(&intdRegs->STATUS_REG_LEVEL_DMPAC_OUT[irqNum][1U]);
    if (0u != (regVal & ((uint32_t)1u << htsPipelineNum)))
    {
        regVal = (uint32_t)1u << htsPipelineNum;
        CSL_REG32_WR(&intdRegs->STATUS_CLR_REG_LEVEL_DMPAC_OUT[irqNum][1U],
            regVal);
    }
}
/* LDRA_JUSTIFY_END */
#endif

int32_t Vhwa_commonCreateHwaLock(uint32_t idx)
{
    int32_t             status = FVID2_EFAIL;
    #if !defined(MCU_PLUS_SDK)
    SemaphoreP_Params   params;
    #endif

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (idx < VHWA_MAX_LOCK)
    /* LDRA_JUSTIFY_END */
    {
        /* Allocate lock semaphore */
        #if defined(MCU_PLUS_SDK)
        if(FVID2_SOK != SemaphoreP_constructBinary(&gHwaLock[idx], 1U))
        #else
        SemaphoreP_Params_init(&params);
        params.mode = SemaphoreP_Mode_BINARY;
        gHwaLock[idx] = SemaphoreP_create(1U, &params);
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (NULL == gHwaLock[idx])
        #endif
        {
            GT_0trace(VhwaDofTrace, GT_ERR,
                "Failed to allocate HWA Lock semaphore for Index%d!!\n");
            status = FVID2_EALLOC;
        }
        /* LDRA_JUSTIFY_END */
        else
        {
            status = FVID2_SOK;
        }
    }

    return (status);
}

void Vhwa_commonDeleteHwaLock(uint32_t idx)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (idx < VHWA_MAX_LOCK)
    /* LDRA_JUSTIFY_END */
    {
        #if defined(MCU_PLUS_SDK)
            SemaphoreP_destruct(&gHwaLock[idx]);
        #else
        /* Delete the lock semaphore */
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        if (NULL != gHwaLock[idx])
        /* LDRA_JUSTIFY_END */
        {
            (void)SemaphoreP_delete(gHwaLock[idx]);
            gHwaLock[idx] = NULL;
        }
        #endif
    }
}

int32_t Vhwa_commonHwaLockAquire(uint32_t idx, uint32_t timeout)
{
    int32_t status = FVID2_EFAIL;
    int32_t semStatus;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (idx < VHWA_MAX_LOCK)
    /* LDRA_JUSTIFY_END */
    {
        /* Take the instance semaphore, so that no other
            * handle can submit request from the task context. */
        semStatus = VhwaAl_SemaphoreP_pend(gHwaLock[idx], timeout);

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if(SYSTEMP_SUCCESS == semStatus)
        /* LDRA_JUSTIFY_END */
        {
            status = FVID2_SOK;
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        else
        {
            /* Convert Semaphore status to FVID2 status */
            if (SYSTEMP_TIMEOUT == semStatus)
            {
                status = FVID2_EAGAIN;
            }
            else
            {
                status = FVID2_EFAIL;
            }
        }
        /* LDRA_JUSTIFY_END */
    }

    return status;
}

void Vhwa_commonHwaLockRelease(uint32_t idx)
{
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    if (idx < VHWA_MAX_LOCK)
    /* LDRA_JUSTIFY_END */
    {
        /* New request can now be submitted to the IP */
        (void)VhwaAl_SemaphoreP_post(gHwaLock[idx]);
    }
}


/* ========================================================================== */
/*                           Local Functions                                  */
/* ========================================================================== */
