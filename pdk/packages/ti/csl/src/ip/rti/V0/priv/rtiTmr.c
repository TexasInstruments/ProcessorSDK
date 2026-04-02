/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file  rtiTmr.c
 *
 *  \brief Timer APIs.
 *
 *   This file contains the device abstraction layer APIs for RTI Timer.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <ti/csl/csl.h>
#include <ti/csl/cslr_rti.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr_rti.h>
#include <ti/csl/src/ip/rti/V0/rtiTmr.h>

/* ========================================================================== */
/*                      Internal Function Declarations                        */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 * Set the stall mode
 */
int32_t RTIGcSetStallMode(uint32_t baseAddr, uint32_t gcStallMode)
{
    int32_t retVal = CSL_EFAIL;
    if (baseAddr  != ((uint32_t) NULL))
    {
        retVal = CSL_PASS;
        if (gcStallMode == RTI_GC_STALL_MODE_OFF)
        {
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL,
                          RTI_RTIGCTRL_COS,
                          RTI_GC_STALL_MODE_OFF);
        }
        else
        {
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL,
                          RTI_RTIGCTRL_COS,
                          RTI_GC_STALL_MODE_ON);
        }
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Start the timer
 */
int32_t RTITmrEnable(uint32_t baseAddr, uint32_t cntIndex)
{
    int32_t retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (cntIndex <= RTI_TMR_CNT_BLK_INDEX_MAX))
    {
        /* Start the timer */
        if (cntIndex == RTI_TMR_CNT_BLK_INDEX_0)
        {
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT0EN, 1);
        }
        else
        {
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT1EN, 1);
        }
        retVal = CSL_PASS;
    }

    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Stop the timer
 */
int32_t RTITmrDisable(uint32_t baseAddr, uint32_t cntIndex)
{
    int32_t retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (cntIndex <= RTI_TMR_CNT_BLK_INDEX_MAX))
    {
        /* Start the timer */
        if (cntIndex == RTI_TMR_CNT_BLK_INDEX_0)
        {
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT0EN, 0);
        }
        else
        {
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT1EN, 0);
        }
        retVal = CSL_PASS;
    }

    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 */
int32_t RTITmrConfigure(uint32_t baseAddr, rtiTmrConfig_t *pConfig)
{
    int32_t  retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (pConfig != NULL) &&
        (pConfig->cntBlkIndex <= RTI_TMR_CNT_BLK_INDEX_MAX) &&
        (pConfig->cmpBlkIndex <= RTI_TMR_CMP_BLK_INDEX_MAX) &&
        (pConfig->prescaler >= RTI_TMR_MIN_PRESCALER_VAL))
    {
        if (pConfig->cntBlkIndex == RTI_TMR_CNT_BLK_INDEX_0)
        {
            /* Disable the counter block */
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT0EN, 0);

            /* Clear free-running counter */
            HW_WR_REG32(baseAddr + RTI_RTIFRC0, 0);

            /* Configure FRC clock source for counter block 0 only */
            if (pConfig->clkSrc == RTI_TMR_CLK_SRC_NTU)
            {
                uint32_t ntuSel = pConfig->ntu;
                HW_WR_FIELD32(baseAddr + RTI_RTITBCTRL, RTI_RTITBCTRL_TBEXT, 1);
                if ((ntuSel != RTI_TMR_NTU_1) &&
                    (ntuSel != RTI_TMR_NTU_2) &&
                    (ntuSel != RTI_TMR_NTU_3))
                {
                    ntuSel = RTI_TMR_NTU_0;
                }
                HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_NTUSEL, ntuSel);
            }
            else
            {
                HW_WR_FIELD32(baseAddr + RTI_RTITBCTRL, RTI_RTITBCTRL_TBEXT, 0);
                HW_WR_REG32(baseAddr + RTI_RTIUC0, 0);

                /* Configure the upper counter comapre register */
                HW_WR_REG32(baseAddr + RTI_RTICPUC0, pConfig->prescaler);
            }
        }
        else /* Couner Block 1 */
        {
            /* Disable the counter block */
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT1EN, 0);

            /* Clear both counters */
            HW_WR_REG32(baseAddr + RTI_RTIUC1, 0);
            HW_WR_REG32(baseAddr + RTI_RTIFRC1, 0);

            /* Configure the upper counter comapre register */
            HW_WR_REG32(baseAddr + RTI_RTICPUC1, pConfig->prescaler);
        }

        /* Configure the compare blocks */
        switch (pConfig->cmpBlkIndex)
        {
            case RTI_TMR_CMP_BLK_INDEX_0:
                HW_WR_REG32(baseAddr + RTI_RTICOMP0, pConfig->period);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP0, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL0, pConfig->cntBlkIndex);
                HW_WR_FIELD32(baseAddr + RTI_RTISETINT, RTI_RTISETINT_SETINT0, 1);
                break;

            case RTI_TMR_CMP_BLK_INDEX_1:
                HW_WR_REG32(baseAddr + RTI_RTICOMP1, pConfig->period);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP1, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL1, pConfig->cntBlkIndex);
                HW_WR_FIELD32(baseAddr + RTI_RTISETINT, RTI_RTISETINT_SETINT1, 1);
                break;

            case RTI_TMR_CMP_BLK_INDEX_2:
                HW_WR_REG32(baseAddr + RTI_RTICOMP2, pConfig->period);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP2, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL2, pConfig->cntBlkIndex);
                HW_WR_FIELD32(baseAddr + RTI_RTISETINT, RTI_RTISETINT_SETINT2, 1);
                break;

            case RTI_TMR_CMP_BLK_INDEX_3:
                HW_WR_REG32(baseAddr + RTI_RTICOMP3, pConfig->period);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP3, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL3, pConfig->cntBlkIndex);
                HW_WR_FIELD32(baseAddr + RTI_RTISETINT, RTI_RTISETINT_SETINT3, 1);
                break;
        }
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Clear Timer
 */
int32_t RTITmrClear(uint32_t baseAddr, uint32_t cntIndex, uint32_t cmpIndex)
{
    int32_t  retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (cntIndex <= RTI_TMR_CNT_BLK_INDEX_MAX) &&
        (cmpIndex <= RTI_TMR_CMP_BLK_INDEX_MAX))
    {
        if (cntIndex == RTI_TMR_CNT_BLK_INDEX_0)
        {
            /* Disable the counter block */
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT0EN, 0);

            /* Clear all counter related registers */
            HW_WR_REG32(baseAddr + RTI_RTIUC0, 0);
            HW_WR_REG32(baseAddr + RTI_RTIFRC0, 0);
            HW_WR_REG32(baseAddr + RTI_RTICPUC0, 0);

            /* Select up counter as the source of FRC */
            HW_WR_FIELD32(baseAddr + RTI_RTITBCTRL, RTI_RTITBCTRL_TBEXT, 0);
        }
        else /* Couner Block 1 */
        {
            /* Disable the counter block */
            HW_WR_FIELD32(baseAddr + RTI_RTIGCTRL, RTI_RTIGCTRL_CNT1EN, 0);

            /* Clear all counter related registers */
            HW_WR_REG32(baseAddr + RTI_RTIUC1, 0);
            HW_WR_REG32(baseAddr + RTI_RTIFRC1, 0);
            HW_WR_REG32(baseAddr + RTI_RTICPUC1, 0);
        }

        /* Clear the compare blocks */
        switch (cmpIndex)
        {
            case RTI_TMR_CMP_BLK_INDEX_0:
                HW_WR_REG32(baseAddr + RTI_RTICOMP0, 0);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP0, 0);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL0, 0);
                break;

            case RTI_TMR_CMP_BLK_INDEX_1:
                HW_WR_REG32(baseAddr + RTI_RTICOMP1, 0);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP1, 0);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL1, 0);
                break;

            case RTI_TMR_CMP_BLK_INDEX_2:
                HW_WR_REG32(baseAddr + RTI_RTICOMP2, 0);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP2, 0);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL2, 0);
                break;

            case RTI_TMR_CMP_BLK_INDEX_3:
                HW_WR_REG32(baseAddr + RTI_RTICOMP3, 0);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP3, 0);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL3, 0);
                break;
        }
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Get/Read the counter value from the counter registers
 */
int32_t RTITmrCounterGet(uint32_t baseAddr, uint32_t cntIndex, rtiTmrCnts_t *pCnts)
{
    int32_t retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (cntIndex <= RTI_TMR_CNT_BLK_INDEX_MAX) &&
        (pCnts != NULL))
    {
        /* Start the timer */
        if (cntIndex == RTI_TMR_CNT_BLK_INDEX_0)
        {
            pCnts->frc =  HW_RD_REG32(baseAddr + RTI_RTIFRC0);
            pCnts->up = HW_RD_REG32(baseAddr + RTI_RTIUC0);
        }
        else
        {
            pCnts->frc =  HW_RD_REG32(baseAddr + RTI_RTIFRC1);
            pCnts->up = HW_RD_REG32(baseAddr + RTI_RTIUC1);
        }
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Configure Capture operation
 */
int32_t RTITmrCaptureConfig(uint32_t baseAddr, rtiTmrCapCtrl_t *pConfig)
{
    int32_t retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (pConfig != NULL) &&
        (pConfig->capSrcCnt0 <= RTI_TMR_CAPTURE_EVT_MAX) &&
        (pConfig->capSrcCnt1 <= RTI_TMR_CAPTURE_EVT_MAX))
    {
        /* Configure Capture source events */
        HW_WR_FIELD32(baseAddr + RTI_RTICAPCTRL, RTI_RTICAPCTRL_CAPCNTR0, pConfig->capSrcCnt0);
        HW_WR_FIELD32(baseAddr + RTI_RTICAPCTRL, RTI_RTICAPCTRL_CAPCNTR1, pConfig->capSrcCnt1);
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Get/Read the counter value from the capture registers
 */
int32_t RTITmrCaptureCounterGet(uint32_t baseAddr, uint32_t cntIndex, rtiTmrCnts_t *pCnts)
{
    int32_t retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (cntIndex <= RTI_TMR_CNT_BLK_INDEX_MAX) &&
        (pCnts != NULL))
    {
        /* Start the timer */
        if (cntIndex == RTI_TMR_CNT_BLK_INDEX_0)
        {
            pCnts->frc =  HW_RD_REG32(baseAddr + RTI_RTICAFRC0);
            pCnts->up = HW_RD_REG32(baseAddr + RTI_RTICAUC0);
        }
        else
        {
            pCnts->frc =  HW_RD_REG32(baseAddr + RTI_RTICAFRC1);
            pCnts->up = HW_RD_REG32(baseAddr + RTI_RTICAUC1);
        }
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Configure Compare operation
 */
int32_t RTITmrCompareConfig(uint32_t baseAddr, uint32_t cmpIndex, rtiTmrCmpCtrl_t *pConfig)
{
    int32_t retVal = CSL_EFAIL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (cmpIndex <= RTI_TMR_CMP_BLK_INDEX_MAX) &&
        (pConfig != NULL) &&
        (pConfig->cntBlkIndex <= RTI_TMR_CNT_BLK_INDEX_MAX))
    {
        /* Configure the compare blocks */
        switch (cmpIndex)
        {
            case RTI_TMR_CMP_BLK_INDEX_0:
                HW_WR_REG32(baseAddr + RTI_RTICOMP0, pConfig->cmpVal);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP0, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL0, pConfig->cntBlkIndex);
                break;

            case RTI_TMR_CMP_BLK_INDEX_1:
                HW_WR_REG32(baseAddr + RTI_RTICOMP1, pConfig->cmpVal);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP1, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL1, pConfig->cntBlkIndex);
                break;

            case RTI_TMR_CMP_BLK_INDEX_2:
                HW_WR_REG32(baseAddr + RTI_RTICOMP2, pConfig->cmpVal);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP2, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL2, pConfig->cntBlkIndex);
                break;

            case RTI_TMR_CMP_BLK_INDEX_3:
                HW_WR_REG32(baseAddr + RTI_RTICOMP3, pConfig->cmpVal);
                HW_WR_REG32(baseAddr + RTI_RTIUDCP3, pConfig->period);
                HW_WR_FIELD32(baseAddr + RTI_RTICOMPCTRL, RTI_RTICOMPCTRL_COMPSEL3, pConfig->cntBlkIndex);
                break;
        }
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 */
uint32_t RTITmrCompareGet(uint32_t baseAddr, uint32_t cmpIndex)
{
    uint32_t regVal = 0U;
    uint32_t regOffset;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (cmpIndex <= RTI_TMR_CMP_BLK_INDEX_MAX))
    {
        switch (cmpIndex)
        {
            case RTI_TMR_CMP_BLK_INDEX_0:
                regOffset = RTI_RTICOMP0;
                break;

            case RTI_TMR_CMP_BLK_INDEX_1:
                regOffset = RTI_RTICOMP1;
                break;

            case RTI_TMR_CMP_BLK_INDEX_2:
                regOffset = RTI_RTICOMP2;
                break;

            case RTI_TMR_CMP_BLK_INDEX_3:
                regOffset = RTI_RTICOMP3;
                break;
        }
        regVal = HW_RD_REG32(baseAddr + regOffset);
    }
    return (regVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Read the status of INTFLAG register.
 */
uint32_t RTITmrIntStatusGet(uint32_t baseAddr)
{
    uint32_t intFlags = 0U;

    if (baseAddr != (uint32_t)(NULL))
    {
        intFlags = HW_RD_REG32(baseAddr + RTI_RTIINTFLAG);
    }

    return (intFlags);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Clear the status of interrupt events
 */
int32_t RTITmrIntStatusClear(uint32_t baseAddr, uint32_t intFlags)
{
    int32_t retVal = CSL_EFAIL;
    uint32_t flags = intFlags & RTI_TMR_INT_STATUS_ALL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (flags    != 0U))
    {
        /* Clear the interrupt status from IRQSTATUS register */
        HW_WR_REG32(baseAddr + RTI_RTIINTFLAG, flags);
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Enable the Timer interrupts
 */
int32_t RTITmrIntEnable(uint32_t baseAddr, uint32_t intFlags)
{
    int32_t retVal = CSL_EFAIL;
    uint32_t flags = intFlags & RTI_TMR_INT_REQ_ALL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (flags    != 0U))
    {
        /* Enable the Timer interrupts represented by intFlags */
        HW_WR_REG32(baseAddr + RTI_RTISETINT, flags);
        retVal = CSL_PASS;
    }
    return(retVal);
}

/**
 * Requirement: REQ_TAG(PDK-5145)
 * Design: did_csl_rtitimer
 * Disable the Timer interrupts
 */
int32_t RTITmrIntDisable(uint32_t baseAddr, uint32_t intFlags)
{
    int32_t retVal = CSL_EFAIL;
    uint32_t flags = intFlags & RTI_TMR_INT_REQ_ALL;

    if ((baseAddr != ((uint32_t) NULL)) &&
        (flags    != 0U))
    {
        /* Enable the Timer interrupts represented by intFlags */
        HW_WR_REG32(baseAddr + RTI_RTICLEARINT, flags);
        retVal = CSL_PASS;
    }
    return(retVal);
}

/********************************* End of file ******************************/
