/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 *  \file pdm_utils_pll.c
 *
 *  \brief This file implements APIs to provide PLL info
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include "pdm_utils.h"
#include "pdm_utils_soc.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Offset for configuration registers of next MCU domain PLL */
#define PDM_UTILS_MCU_PLL0_NEXT_PLL                   (0x1000UL)
/* Offset for configuration registers of next MAIN domain PLL */
#define PDM_UTILS_PLL0_NEXT_PLL                       (0x1000UL)
/* Register offset for clock select register from base address */
#define PDM_UTILS_PLL0_CLK_SEL_OFFSET                 (0x8080U)
/* Register offset for PLL config register from base address */
#define PDM_UTILS_PLL0_CFG_OFFSET                     (0x8U)
/* Register offset for PLL control register from base address */
#define PDM_UTILS_PLL0_CTRL_OFFSET                    (0x20U)
/* Register offset for PLL frequency control register1 from base address */
#define PDM_UTILS_PLL0_FREQ_CTRL0_OFFSET              (0x30U)
/* Register offset for PLL frequency control register2 from base address */
#define PDM_UTILS_PLL0_FREQ_CTRL1_OFFSET              (0x34U)
/* Register offset for PLL divider control register from base address */
#define PDM_UTILS_PLL0_DIV_CTRL_OFFSET                (0x38U)
/* Register offset for PLL HSDIV control from base address */
#define PDM_UTILS_PLL0_HSDIV0_CTRL_OFFSET             (0x80U)
/* Register offset for next HSDIV configuration registers from base address */
#define PDM_UTILS_PLL0_HSDIV0_NEXT_HSDIV              (0x4U)
/* Identifier for MCU domain PLL */
#define PDM_UTILS_MCU_DOMAIN_PLL                      (0x0U)
/* Identifier for MAIN domain PLL */
#define PDM_UTILS_MAIN_DOMAIN_PLL                     (0x1U)

/* Mask for Freq1 register to obtain post_div1 value */
#define PDM_UTILS_PLL0_DIV_CTRL_FREQ1_MASK            (0x70000U)
/* Shift value for Freq1 register to obtain post_div1 value */
#define PDM_UTILS_PLL0_DIV_CTRL_FREQ1_SHIFT           (16U)

/* Mask for Freq2 register to obtain post_div2 value */
#define PDM_UTILS_PLL0_DIV_CTRL_FREQ2_MASK            (0x7000000U)
/* Shift value for Freq2 register to obtain post_div2 value */
#define PDM_UTILS_PLL0_DIV_CTRL_FREQ2_SHIFT           (24U)

/* Mask for PLL divider register to obtain pre_div value */
#define PDM_UTILS_PLL0_DIV_CTRL_PREDIV_MASK           (0x3FU)
/* Shift for PLL divider register to obtain pre_div value */
#define PDM_UTILS_PLL0_DIV_CTRL_PREDIV_SHIFT          (0U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Array to hold MAIN PLL IDs present in this device
 * This is defined in pdm_utils_soc.c file, used here to get the available MAIN PLL IDs
 */
extern uint32_t gPdmUtilsMCUPLLIds[PDM_UTILS_NUM_MCU_PLL];

/* Array to hold MCU PLL IDs present in this device
 * This is defined in pdm_utils_soc.c file, used here to get the available MCU PLL IDs
 */
extern uint32_t gPdmUtilsMainPLLIds[PDM_UTILS_NUM_MAIN_PLL];

/* ========================================================================== */
/*                       Function Declarations                                */
/* ========================================================================== */

/**
 * \brief This API gives base address of configuration registers for a given PLL
 *        Note that PLL data structure should have the valid PLL number and domain values for this API to work.
 *
 * \param pllData PLL data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */

static int32_t PdmUtils_getPllRegBaseAddr(PdmUtils_PllData *pllData);

/**
 * \brief This API gives HF Oscillator frequency of this device
 *
 *
 * \return frequency of the HF Oscillator of this device
 */
static uint64_t PdmUtils_getHfoscFreq(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */



int32_t PdmUtils_getNumHsdiv(PdmUtils_PllData *pllData)
{
    uint32_t pllCfg;
    uint32_t numHsDivs;
    int32_t retVal =  PDM_UTILS_PASS;

    if(NULL == pllData)
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        if(PDM_UTILS_MCU_DOMAIN_PLL == pllData->domainID)
        {
            pllCfg = CSL_REG32_RD(PDM_UTILS_MCU_PLL0_CFG_BASE +
                                  (pllData->pllId * PDM_UTILS_MCU_PLL0_NEXT_PLL) +
                                  PDM_UTILS_PLL0_CFG_OFFSET);
        }
        else
        {
            pllCfg = CSL_REG32_RD(PDM_UTILS_PLL0_CFG_BASE +
                                  (pllData->pllId * PDM_UTILS_PLL0_NEXT_PLL) +
                                  PDM_UTILS_PLL0_CFG_OFFSET);
        }
        /* Bits 20 to 32 represent HSDIVs connected to this PLL */
        numHsDivs = ((pllCfg & CSL_MAIN_PLL_MMR_CFG_PLL0_CFG_HSDIV_PRSNT_MASK) >>
                     CSL_MAIN_PLL_MMR_CFG_PLL0_CFG_HSDIV_PRSNT_SHIFT);
        for(pllData->numHsDivs=0U; numHsDivs>0U; pllData->numHsDivs++)
        {
            numHsDivs &= (numHsDivs - 1);
        }
    }

    return retVal;
}

int32_t PdmUtils_getPllStatus(PdmUtils_PllData *pllData)
{
    uint32_t pllCtrl;
    int32_t retVal = PDM_UTILS_PASS;

    if(NULL == pllData)
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        if(PDM_UTILS_MCU_DOMAIN_PLL == pllData->domainID)
        {
            pllCtrl = CSL_REG32_RD(PDM_UTILS_MCU_PLL0_CFG_BASE +
                                   (pllData->pllId * PDM_UTILS_MCU_PLL0_NEXT_PLL) +
                                   PDM_UTILS_PLL0_CTRL_OFFSET);
        }
        else
        {
            pllCtrl = CSL_REG32_RD(PDM_UTILS_PLL0_CFG_BASE +
                                   (pllData->pllId * PDM_UTILS_PLL0_NEXT_PLL) +
                                   PDM_UTILS_PLL0_CTRL_OFFSET);
        }
        /* Bit 15 represent PLL status (Enabled - 1, Disabled - 0) */
        pllData->enabled = ((pllCtrl &
                             (CSL_MAIN_PLL_MMR_CFG_PLL0_CTRL_PLL_EN_MASK)) >>
                            CSL_MAIN_PLL_MMR_CFG_PLL0_CTRL_PLL_EN_SHIFT);
    }

    return retVal;
}

int32_t PdmUtils_getPllPostDivClkStatus(PdmUtils_PllData *pllData)
{
    uint32_t pllCtrl;
    int32_t retVal = PDM_UTILS_PASS;

    if(NULL == pllData)
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        if(PDM_UTILS_MCU_DOMAIN_PLL == pllData->domainID)
        {
            pllCtrl = CSL_REG32_RD(PDM_UTILS_MCU_PLL0_CFG_BASE +
                                   (pllData->pllId * PDM_UTILS_MCU_PLL0_NEXT_PLL) +
                                   PDM_UTILS_PLL0_CTRL_OFFSET);
        }
        else
        {
            pllCtrl = CSL_REG32_RD(PDM_UTILS_PLL0_CFG_BASE +
                                   (pllData->pllId * PDM_UTILS_PLL0_NEXT_PLL) +
                                   PDM_UTILS_PLL0_CTRL_OFFSET);
        }
        /* Bit 4 represent PLLPOSTDIV Clock status (Enabled - 1, Disabled - 0) */
        pllData->postDivClkEnabled = ((pllCtrl &
                                       CSL_MAIN_PLL_MMR_CFG_PLL0_CTRL_CLK_POSTDIV_EN_MASK) >>
                                      CSL_MAIN_PLL_MMR_CFG_PLL0_CTRL_CLK_POSTDIV_EN_SHIFT);
    }

    return retVal;
}

static int32_t PdmUtils_getPllRegBaseAddr(PdmUtils_PllData *pllData)
{
    int32_t retVal;

    if(NULL == pllData)
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        if(PDM_UTILS_MCU_DOMAIN_PLL == pllData->domainID)
        {
            retVal = PDM_UTILS_MCU_PLL0_CFG_BASE + (pllData->pllId * PDM_UTILS_MCU_PLL0_NEXT_PLL);
        }
        else
        {
            retVal = PDM_UTILS_PLL0_CFG_BASE + (pllData->pllId * PDM_UTILS_PLL0_NEXT_PLL);
        }
    }
    return retVal;
}

int32_t PdmUtils_getPllDividerData(PdmUtils_PllData *pllData)
{
    uint32_t regBaseAddr;
    float pllDivDatafraq;
    int32_t retVal = PDM_UTILS_PASS;

    if(NULL == pllData)
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        regBaseAddr = PdmUtils_getPllRegBaseAddr(pllData);
        pllData->pllDivData.integerDiv = CSL_REG32_RD(regBaseAddr + PDM_UTILS_PLL0_FREQ_CTRL0_OFFSET);
        pllData->pllDivData.fracDiv = CSL_REG32_RD(regBaseAddr + PDM_UTILS_PLL0_FREQ_CTRL1_OFFSET);
        pllData->pllDivData.postDiv1 = (CSL_REG32_RD(regBaseAddr + PDM_UTILS_PLL0_DIV_CTRL_OFFSET) &
                                       PDM_UTILS_PLL0_DIV_CTRL_FREQ1_MASK) >>
                                       PDM_UTILS_PLL0_DIV_CTRL_FREQ1_SHIFT;
        pllData->pllDivData.postDiv2 = (CSL_REG32_RD(regBaseAddr + PDM_UTILS_PLL0_DIV_CTRL_OFFSET) &
                                       PDM_UTILS_PLL0_DIV_CTRL_FREQ2_MASK) >>
                                       PDM_UTILS_PLL0_DIV_CTRL_FREQ2_SHIFT;
        pllData->pllDivData.preDiv = (CSL_REG32_RD(regBaseAddr + PDM_UTILS_PLL0_DIV_CTRL_OFFSET) &
                                     PDM_UTILS_PLL0_DIV_CTRL_PREDIV_MASK) >>
                                     PDM_UTILS_PLL0_DIV_CTRL_PREDIV_SHIFT;
        pllDivDatafraq = PdmUtils_getHfoscFreq() * ((pllData->pllDivData.integerDiv) + (float)(pllData->pllDivData.fracDiv)/(1 << 24));
        pllData->vcoClkFreq = (uint64_t)(pllDivDatafraq);
    }

    return retVal;

}

int32_t PdmUtils_getPllHsdivInfo(PdmUtils_PllData *pllData)
{
    uint32_t regBaseAddr;
    uint32_t hsDivCfg;
    int32_t retVal = PDM_UTILS_PASS;
    uint32_t hsdiv=0U, count=0U, hsdivMask, pllCfg;

    if(NULL == pllData)
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        regBaseAddr = PdmUtils_getPllRegBaseAddr(pllData);
        retVal = PdmUtils_getNumHsdiv(pllData);
        if(PDM_UTILS_MCU_DOMAIN_PLL == pllData->domainID)
        {
            pllCfg = CSL_REG32_RD(PDM_UTILS_MCU_PLL0_CFG_BASE +
                                  (pllData->pllId * PDM_UTILS_MCU_PLL0_NEXT_PLL) +
                                  PDM_UTILS_PLL0_CFG_OFFSET);
        }
        else
        {
            pllCfg = CSL_REG32_RD(PDM_UTILS_PLL0_CFG_BASE +
                                  (pllData->pllId * PDM_UTILS_PLL0_NEXT_PLL) +
                                  PDM_UTILS_PLL0_CFG_OFFSET);
        }
        /* Bits 20 to 32 represent HSDIVs connected to this PLL */
        hsdivMask = ((pllCfg & CSL_MAIN_PLL_MMR_CFG_PLL0_CFG_HSDIV_PRSNT_MASK) >>
                      CSL_MAIN_PLL_MMR_CFG_PLL0_CFG_HSDIV_PRSNT_SHIFT);

        while(hsdivMask > 0)
        {
            /* Check if the bit at hsdiv location is set */
            if(1U == (hsdivMask & 0x1U))
            {
                /* Get status of hsdiv */
                hsDivCfg = CSL_REG32_RD(regBaseAddr +
                                        PDM_UTILS_PLL0_HSDIV0_CTRL_OFFSET +
                                        (hsdiv * PDM_UTILS_PLL0_HSDIV0_NEXT_HSDIV));
                /* Populate hsdiv ID */
                pllData->pllHsDivData[count].id = hsdiv;
                /* (0 to 6) bits gives the clkout divider value */
                pllData->pllHsDivData[count].hsdiv = (hsDivCfg & (CSL_MAIN_PLL_MMR_CFG_PLL0_HSDIV_CTRL0_HSDIV_MASK)) + 1U ;
                /* 15th bit gives the clkout enable status */
                pllData->pllHsDivData[count].enabled = (hsDivCfg & CSL_MAIN_PLL_MMR_CFG_PLL0_HSDIV_CTRL0_CLKOUT_EN_MASK) >>
                                                       CSL_MAIN_PLL_MMR_CFG_PLL0_HSDIV_CTRL1_CLKOUT_EN_SHIFT ;
                /* If hsdiv is enabled, calculate the frequency from VCO frequency */
                if (pllData->pllHsDivData[count].enabled == 1U)
                {
                    if(hsdiv >= 5)
                    {
                        pllData->pllHsDivData[count].freq = (pllData->vcoClkFreq/1000000) /
                                                            ((pllData->pllHsDivData[count].hsdiv) *
                                                             (pllData->pllDivData.postDiv1));
                    }
                    else
                    {
                        pllData->pllHsDivData[count].freq = ((pllData->vcoClkFreq/1000000) /
                                                             (pllData->pllHsDivData[count].hsdiv));
                    }
                }

                count++;
            }

            hsdivMask = hsdivMask>>0x1;
            hsdiv++;
        }
    }

    return retVal;
}

int32_t PdmUtils_getPllDataBook(void *ptr, uint32_t size)
{
    int32_t retVal = PDM_UTILS_PASS;
    PdmUtils_PllData *pllData;
    PdmUtils_PllDataBook *pllDataBook;
    int32_t count;

    if ((NULL == ptr) || (size < (sizeof(PdmUtils_PllDataBook))))
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {

        pllDataBook = (PdmUtils_PllDataBook*)ptr;
        pllData = pllDataBook->pllData;
        pllDataBook->numMcuPll = PDM_UTILS_NUM_MCU_PLL;
        pllDataBook->numMainPll = PDM_UTILS_NUM_MAIN_PLL;

        /* MAIN PLL data */
        for(count=0U; count<PDM_UTILS_NUM_MAIN_PLL; count++)
        {
            pllData->domainID = PDM_UTILS_MAIN_DOMAIN_PLL;
            pllData->pllId = gPdmUtilsMainPLLIds[count];
            PdmUtils_getPllInfo(pllData);
            pllData++;
        }

        /* MCU PLL data */
        for(count=0U; count<PDM_UTILS_NUM_MCU_PLL; count++)
        {
            pllData->domainID = PDM_UTILS_MCU_DOMAIN_PLL;
            pllData->pllId = gPdmUtilsMCUPLLIds[count];
            PdmUtils_getPllInfo(pllData);
            pllData++;
        }
    }

    return retVal;
}

int32_t PdmUtils_getPllInfo(PdmUtils_PllData *pllData)
{
    int32_t retVal = PDM_UTILS_PASS;

    if(NULL == pllData)
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        retVal += PdmUtils_getPllStatus(pllData);
        retVal += PdmUtils_getPllDividerData(pllData);
        retVal += PdmUtils_getPllPostDivClkStatus(pllData);
        retVal += PdmUtils_getPllHsdivInfo(pllData);
    }

    return retVal;
}

#if defined(BUILD_MCU1_0) || defined (BUILD_WKUP_R5)
static uint64_t PdmUtils_getHfoscFreq()
{
    struct clk *clkOutput=NULL;
    uint64_t clkFreq=0;

    clkOutput = clk_lookup(PDM_UTILS_HFOSC_CLOCK);
    if (NULL != clkOutput)
    {
        clkFreq = clk_get_freq(clkOutput);
    }

    return clkFreq;
}
#else
static uint64_t PdmUtils_getHfoscFreq()
{
    return PLL_HFOSC_REFCLK;
}
#endif

