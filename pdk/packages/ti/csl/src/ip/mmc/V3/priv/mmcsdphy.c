/*
*  Copyright (C) 2017 Arasan Chip Systems Inc
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
 *    Neither the name of Arasan Chip Systems Incorporated nor the names of
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
/*********************************************************************
*  
*  Name             : mmcsdphy.c
*  Creation Date : Aug 28, 2017
*  Description     : This file contains functions to read / write the host controller
*                          phy registers
*
*  Change History :
*  <Date>            <Author>    <Version>      < Description >
*  28-Aug-2017    PB              0.1                 File structure creation
*  01-Sept-2017   PB		   1.0		   Added Phy layer function definitions
*
*********************************************************************/
/**
 *  \file mmcsdphy.c
 *
 *  \brief File with mmc sd phy register read/write functions
 *
**/

/*   Includes    */

#include <ti/csl/src/ip/mmc/V3/mmcsdphy.h>
#include <ti/csl/src/ip/mmc/V3/priv/low_level_hw.h>
#include <ti/csl/src/ip/mmc/V3/mmcsd.h>

/*    Globals    */



/*    Function Definitions    */

static int32_t HSMMCSDPhyValidateBaseAddress(uintptr_t baseAddr)
{
	int32_t status = STW_SOK;

	/* TODO: Modify conditional check here as per requirement for validation of base address*/
	if(0x0U == baseAddr)
	{
		status = STW_EOUT_OF_RANGE;
	}

	return status;
}


static int32_t SdhcPhyRegisterWrite(uintptr_t baseAddr, uint32_t regOffset, uint32_t value)
{
	int32_t status = STW_SOK;
    SdhcRegisterWriteDWord(baseAddr + regOffset,value);
	return status;
}


static int32_t SdhcPhyRegisterRead(uintptr_t baseAddr, uint32_t regOffset, uint32_t *value)
{
	int32_t status = STW_SOK;
	*value = SdhcRegisterReadDWord(baseAddr + regOffset);
	return status;
}

int32_t HSMMCSDPhyDisableDLL (uintptr_t baseAddr)
{
	int32_t status=STW_SOK;   
   	status = HSMMCSDPhyValidateBaseAddress(baseAddr);
	if(STW_SOK == status)
	{
       /* Disable DLL */
       SdhcRegisterWriteFieldDWord(baseAddr + MMC_PHY_CTRL1_REG_OFFSET,MMC_PHY_CTRL1_ENDLL_MASK,MMC_PHY_CTRL1_ENDLL_SHIFT,MMC_PHY_CTRL1_ENDLL_DISABLE);
    }	
    return status;
}

HSMMCSD_phy_config mmcsdphyConfig[] =
{
	/*
	uint32_t drvtype;
	uint32_t strbsel;
	uint32_t otapdelayen;
	uint32_t otapdelaysel;
	uint32_t itapdelayen;
	uint32_t itapdelaysel;
	uint32_t sel50;
	uint32_t sel100;
	uint32_t clkbufsel;
	uint32_t txdelay;
	uint32_t rxdelay;
	*/

	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, /* reserved */
	{ 0, 15,  1,  0,  1,  0,  0,  0,  7,  0,  0 }, /* MODE_HS400 (unsupported) */
	{ 0,  0,  1,  5,  1,  0,  0,  0,  7,  0,  0 }, /* MODE_HS200 (unsupported) */
	{ 0,  0,  1,  8,  1,  0,  0,  1,  7,  0,  0 }, /* MODE_HSSDR50 (unsupported) */
	{ 0,  0,  1,  4,  1,  0,  1,  0,  7,  0,  0 }, /* MODE_HSDDR50 (unsupported) */
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, /* MODE_ENHANCED_STROBE (unsupported) */
	{ 0,  0,  1,  7,  1,  0,  0,  0,  7,  0,  0 }, /* MODE_SDR104 */
	{ 0,  0,  1,  8,  1,  0,  0,  1,  7,  0,  0 }, /* MODE_SDR50 */
	{ 0,  0,  1,  4,  1,  0,  1,  0,  7,  0,  0 }, /* MODE_DDR50 */
	{ 0,  0,  1, 15,  1,  1,  0,  0,  7,  1,  1 }, /* MODE_SDR25 */
	{ 0,  0,  1, 15,  1, 10,  0,  0,  7,  1,  1 }, /* MODE_SDR12 */
	{ 0,  0,  0,  0,  1,  1,  0,  0,  7,  1,  1 }, /* MODE_HS */
	{ 0,  0,  0,  0,  1, 10,  0,  0,  7,  1,  1 }  /* MODE_DS */

};

static void HSMMCSDPhy_get_configval(uintptr_t baseAddr,  uint32_t mode, uint32_t* reg4val_ptr, uint32_t* reg5val_ptr)
{
    uint32_t regValue_4, regValue_5;
    HSMMCSD_phy_config *phyConfig;

    switch(mode) {
        case MODE_DS:
        case MODE_HS:
        case MODE_SDR12:
        case MODE_SDR25:
        case MODE_SDR50:
        case MODE_HSSDR50:
        case MODE_DDR50:
        case MODE_HSDDR50:
        case MODE_SDR104:
        case MODE_HS200:
        case MODE_HS400:
            phyConfig = &mmcsdphyConfig[mode];
            break;
        default:
            phyConfig = &mmcsdphyConfig[0];
            break;
    }

    /* PHY_CTRL_4_REG */
    regValue_4 = 0U;
    SdhcPhyRegisterRead(baseAddr, MMC_PHY_CTRL4_REG_OFFSET, &regValue_4);

    regValue_4 = SdhcSetFieldValueDWord(regValue_4,MMC_PHY_CTRL4_OTAPDLYENA_SHIFT,
                        MMC_PHY_CTRL4_OTAPDLYENA_MASK, phyConfig->otapdelayen);
    regValue_4 = SdhcSetFieldValueDWord(regValue_4,MMC_PHY_CTRL4_OTAPDLYSEL_SHIFT,
                        MMC_PHY_CTRL4_OTAPDLYSEL_MASK, phyConfig->otapdelaysel);
    regValue_4 = SdhcSetFieldValueDWord(regValue_4,MMC_PHY_CTRL4_ITAPDLYENA_SHIFT,
                        MMC_PHY_CTRL4_ITAPDLYENA_MASK, phyConfig->itapdelayen);
    regValue_4 = SdhcSetFieldValueDWord(regValue_4,MMC_PHY_CTRL4_ITAPDLY_SHIFT,
                        MMC_PHY_CTRL4_ITAPDLY_MASK, phyConfig->itapdelaysel);
    regValue_4 = SdhcSetFieldValueDWord(regValue_4,MMC_PHY_CTRL4_STRBSEL_SHIFT,
                        MMC_PHY_CTRL4_STRBSEL_MASK, phyConfig->strbsel);

    /* PHY_CTRL_5_REG */
    regValue_5 = 0U;
    SdhcPhyRegisterRead(baseAddr, MMC_PHY_CTRL5_REG_OFFSET, &regValue_5);
    if(phyConfig->txdelay || phyConfig->rxdelay) {
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SEL100_SHIFT,
                        MMC_PHY_CTRL5_SEL100_MASK, 0);
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SEL50_SHIFT,
                        MMC_PHY_CTRL5_SEL50_MASK, 0);
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SELDLYTXCLK_SHIFT,
                        MMC_PHY_CTRL5_SELDLYTXCLK_MASK, phyConfig->txdelay);
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SELDLYRXCLK_SHIFT,
                        MMC_PHY_CTRL5_SELDLYRXCLK_MASK, phyConfig->rxdelay);
    }
    else {
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SEL100_SHIFT,
                        MMC_PHY_CTRL5_SEL100_MASK, phyConfig->sel100);
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SEL50_SHIFT,
                        MMC_PHY_CTRL5_SEL50_MASK, phyConfig->sel50);
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SELDLYTXCLK_SHIFT,
                        MMC_PHY_CTRL5_SELDLYTXCLK_MASK, 0);
        regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_SELDLYRXCLK_SHIFT,
                        MMC_PHY_CTRL5_SELDLYRXCLK_MASK, 0);
    }
    regValue_5 = SdhcSetFieldValueDWord(regValue_5,MMC_PHY_CTRL5_CLKBUFSEL_SHIFT,
                    MMC_PHY_CTRL5_CLKBUFSEL_MASK, phyConfig->clkbufsel);

    *reg4val_ptr = regValue_4;
    *reg5val_ptr = regValue_5;
}

int32_t HSMMCSDPhyConfigure(uintptr_t baseAddr, uint32_t mode, uint32_t clock, uint32_t driverType)
{
	int32_t status=STW_SOK;
	uint32_t dllrdy=0;
	uint32_t reg4, reg5, regValue;

	status = HSMMCSDPhyValidateBaseAddress(baseAddr);
	if(STW_SOK == status)
	{
        HSMMCSDPhy_get_configval(baseAddr, mode, &reg4, &reg5);

        /* set MMC_PHY_CTRL4_ITAPCHGWIN_MASK = 1 when change ITAPDLYSEL */
        status = SdhcPhyRegisterWrite(baseAddr, MMC_PHY_CTRL4_REG_OFFSET, reg4 | MMC_PHY_CTRL4_ITAPCHGWIN_MASK);
        status = SdhcPhyRegisterWrite(baseAddr, MMC_PHY_CTRL4_REG_OFFSET, reg4 & ~MMC_PHY_CTRL4_ITAPCHGWIN_MASK);
        status = SdhcPhyRegisterWrite(baseAddr, MMC_PHY_CTRL5_REG_OFFSET, reg5);

        if(reg5 & (MMC_PHY_CTRL5_SELDLYTXCLK_MASK | MMC_PHY_CTRL5_SELDLYRXCLK_MASK)) {
            /* keep DLL disable */
            return STW_SOK;
        }


	  /*Configure DLL Trim PHY_DLL_TRIM */
	  status = SdhcPhyRegisterRead(baseAddr, MMC_PHY_CTRL1_REG_OFFSET, &regValue);

	  regValue = SdhcSetFieldValueDWord(regValue,MMC_PHY_CTRL1_DLL_TRIM_ICP_SHIFT,
				MMC_PHY_CTRL1_DLL_TRIM_ICP_MASK, PHY_DLL_TRIM_ICP_8);
	            
	   /* Set the impedance val/driver strength */
	   regValue = SdhcSetFieldValueDWord(regValue,MMC_PHY_CTRL1_DR_TY_SHIFT,
					MMC_PHY_CTRL1_DR_TY_MASK, driverType);

      /* Enable DLL */
      regValue = SdhcSetFieldValueDWord(regValue,MMC_PHY_CTRL1_ENDLL_SHIFT,
					MMC_PHY_CTRL1_ENDLL_MASK, MMC_PHY_CTRL1_ENDLL_ENABLE);
	      
	  status = SdhcPhyRegisterWrite(baseAddr, MMC_PHY_CTRL1_REG_OFFSET, regValue);

	  /* Wait until dllrdy bit is set to 1 in the Phy Status (0x30) register */
	  do {
		dllrdy = SdhcRegisterReadFieldDWord(baseAddr + MMC_PHY_STS_1_REG_OFFSET ,MMC_PHY_STS_1_DLLRDY_MASK,MMC_PHY_STS_1_DLLRDY_SHIFT);
	  } while(dllrdy!=PHY_DLL_CTRL_STS_DLL_RDY_TRUE);
    }
	return status;
}

int32_t HSMMCSDPhyLDOSet(uintptr_t baseAddr, uint32_t state)
{
	int32_t status=STW_SOK;
	return status;
}


int32_t HSMMCSDPhyInit(uintptr_t baseAddr)
{
	int32_t status=STW_SOK;
	return status;
}
