/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *   \file gpadc_soc.c
 *
 *   \brief The file implements the Platform specific GPADC Driver Configurations
 */

 /******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stdint.h>
#include <string.h>
#include <ti/osal/DebugP.h>
#include <ti/csl/arch/r5/csl_arm_r5_pmu.h>
#include "gpadc_soc.h"
/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/*! \brief
 * GPADC Param Lut
 * ConfigValue, {ParamValue, CollectSamples, SkipSamples, ChirpBreak, Reserved}, time(10MHz clock)
 */
GPADC_CfgAndParamValuesType GPADC_ConfigParamLuTab[MAX_GPADC_MEAS_SOURCES] = {
    /*! UnBuffConfigVal, BuffConfigVal, {ParamVal, CollectSamples, b4_SkipSamples, Reserved}, time */
    {    0x400U,         0x2U,          { 124U,        255U,           0U,            0U },     74U }, /* #0 M_GPADC_MEAS_EXT_CH1 */
    {    0x800U,         0x4U,          { 124U,        255U,           0U,            0U },     74U }, /* #1 M_GPADC_MEAS_EXT_CH2 */
    {   0x1000U,         0x8U,          { 124U,        255U,           0U,            0U },     74U }, /* #2 M_GPADC_MEAS_EXT_CH3 */
    {   0x2000U,        0x10U,          { 124U,        255U,           0U,            0U },     74U }, /* #3 M_GPADC_MEAS_EXT_CH4 */
    {   0x4000U,     0x10000U,          { 124U,        255U,           0U,            0U },     74U }, /* #4 M_GPADC_MEAS_EXT_CH5 */
    {   0x8000U,    0x200000U,          { 124U,        255U,           0U,            0U },     74U }, /* #5 M_GPADC_MEAS_EXT_CH6 */
    {0x1000000U,    0x400000U,          { 124U,        255U,           0U,            0U },     74U }, /* #6 M_GPADC_MEAS_EXT_CH7 */
    {0x2000000U,    0x800000U,          { 124U,        255U,           0U,            0U },     74U }, /* #7 M_GPADC_MEAS_EXT_CH8 */
    {0x8000000U,   0x4000000U,          { 124U,        255U,           0U,            0U },     74U }, /* #8 M_GPADC_MEAS_EXT_CH9 */
};

GPADC_DriverObjectType GPADCDrvObj;

/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */

void GPADC_DriverInit(const GPADC_ConfigType *CfgPtr)
{
    uint8_t divider;
    uint32_t profRamAdd;
    uint32_t regWrSts = 0U;
    const GPADC_ConfigType *ConfigPtr;
    uint8_t  channelIndex = 0U;
    uint8_t channelNumber;
    uint32_t skipSamples = 0U;
    uint8_t skipSamplesReg = 0U;


	ConfigPtr = CfgPtr;
    memset(&GPADCDrvObj,0,sizeof(GPADCDrvObj));

    GPADCDrvObj.convMode = ConfigPtr->convMode;
    GPADCDrvObj.triggSrc = ConfigPtr->triggSrc;
    GPADCDrvObj.driverStatus = GPADC_IDLE;

    for (channelIndex = 0U; channelIndex < MAX_GPADC_MEAS_SOURCES; channelIndex++)
    {
    	if(ConfigPtr->channelConfig[channelIndex].isConfigured)
    	{
    		channelNumber = ConfigPtr->channelConfig[channelIndex].channelID;
    		GPADCDrvObj.driverChannelConfig[channelNumber].isChannelConfigured = TRUE;
    		GPADCDrvObj.driverChannelConfig[channelNumber].isChannelBufferedMode = ConfigPtr->channelConfig[channelIndex].isBufferedMode;


    		if(ConfigPtr->channelConfig[channelIndex].useLuTable)
    		{
        		if(ConfigPtr->channelConfig[channelIndex].isBufferedMode)
        		{
        			GPADCDrvObj.driverChannelConfig[channelNumber].channelConfigValue = GPADC_ConfigParamLuTab[channelNumber].BuffConfigValue;
        		}
        		else
        		{
        			GPADCDrvObj.driverChannelConfig[channelNumber].channelConfigValue = GPADC_ConfigParamLuTab[channelNumber].UnbuffConfigValue;
        		}

    			GPADCDrvObj.driverChannelConfig[channelNumber].channelParamValue.b32_Val = GPADC_ConfigParamLuTab[channelNumber].ParamInfo.b32_Val;
    		}
    		else
    		{
    			GPADCDrvObj.driverChannelConfig[channelNumber].channelConfigValue = ConfigPtr->channelConfig[channelIndex].channelConfigValue;

    			GPADCDrvObj.driverChannelConfig[channelNumber].channelParamValue.bits.b8_ParamValue = ConfigPtr->channelConfig[channelIndex].channelParamValue;

    			GPADCDrvObj.driverChannelConfig[channelNumber].channelParamValue.bits.b8_CollectSamples = ConfigPtr->channelConfig[channelIndex].collectSamples;
    			skipSamples = ConfigPtr->channelConfig[channelIndex].skipSamples;
    			skipSamplesReg = GPADC_ConvSkipTimeToCode(skipSamples);
    			GPADCDrvObj.driverChannelConfig[channelNumber].channelParamValue.bits.b7_SkipSamples = (((uint32_t)skipSamplesReg & (uint32_t)0x7FU));
    			GPADCDrvObj.driverChannelConfig[channelNumber].channelParamValue.bits.b9_Reserved = 0U; // Clear reserved 9 bits
    		}
    	}
    }


    /*
     * Source sysclk 200MHz
     * Configure sys clock divider as 0x000 in multibit which is 200/1
     */
    M_REG_STRUCT_SWRITE(MSS_GPADC_CLK_DIV_VAL_PTR->bits.b24_Clkdivr, 0x131313U,  regWrSts);

    /*
     * writing '111' will gate clock for MSS GPADC
     */
    M_REG_STRUCT_SWRITE(MSS_GPADC_CLK_GATE_PTR->bits.b3_Gated, 0U,  regWrSts);

    /*
     * Configure the clock divider for GPADC Module
     * GPADC Clock = 10MHz = source_clock/Divider
     * source_clock can be 200 or xtal clock
     */
    divider = (20U);
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg8.bits.b8_GpadcClkDiv, divider,  regWrSts);


    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg1.bits.b1_GpadcInit, 1U, regWrSts);
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg1.bits.b1_GpadcFsmBypass, 1U, regWrSts);

    /* Enable the GPADC FSM clock */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b1_GpadcFsmClkEnable, 1U, regWrSts);

    /* Enable the GPADC clock */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg8.bits.b1_GpadcClkEnable, 1U, regWrSts);

    /* Write the Mode of operation */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b2_DcbistMode, M_GPADC_MODE_DISABLE, \
                                                                                    regWrSts);

    /* Write config value for IFM Mode */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg2.bits.b32_ConfigValueIfm, 0U, regWrSts);

    /* Write Skip Samples for IFM Mode */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg3.bits.b7_SkipSamplesIfm, 0U, regWrSts);

    /* Write Collect Samples for IFM Mode */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg3.bits.b8_CollectSamplesIfm, 0U, regWrSts);

    /* Write Param value for IFM Mode */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg3.bits.b8_ParamValIfm, 0U, regWrSts);

    /* Write Packet RAM Base address for CTM Mode */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_PacketRamAdd[0].b32_Reg, 0U, regWrSts);
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_PacketRamAdd[1].b32_Reg, 0U, regWrSts);
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_PacketRamAdd[2].b32_Reg, 0U, regWrSts);

    /*!
     * Profile 15 Instruction Ram address update with M_GPADC_MON_INSTR_RAM_ST_IND for monitoring
     */
    profRamAdd = ((uint32_t)M_GPADC_MON_INSTR_RAM_ST_IND << 24U) & 0xFF000000U;
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_PacketRamAdd[3].b32_Reg, profRamAdd, regWrSts);

    if(GPADC_CONTINUOUS_CONV_MODE == GPADCDrvObj.convMode)
    {
	    M_REG_STRUCT_SWRITE(MSS_CTRL_MSS_GPADC_MEM_INIT_PTR->bits.b1_mem0_init, 1U,  regWrSts);
    }
}

void GPADC_DriverDeInit(void)
{
    uint32_t regWrSts = 0U;

    /*
     * Release Reset GPADC for digital FSM
     * Writing '111' or '0x07' will reset MSS GPADC
     */
    M_REG_STRUCT_SWRITE(MSS_GPADC_RST_CTRL_PTR->bits.b3_Assert, M_GPADC_FSM_ASSERT_RESET, regWrSts);

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_PTR->bits.b1_ClkTmuxEsdCtrl, 0U, regWrSts);
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_PTR->bits.b1_AnaTestEn, 0U, regWrSts);

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_PTR->bits.b1_AnaogTestTmuxEsdCtrl, 0U, regWrSts);

    /* Reset GPADC Ana */
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcReset, M_GPADC_ASSERT_RESET, regWrSts);
    /* Disable GPADC Ana */
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcEn, M_GPADC_DISABLE, regWrSts);

    /*! Assert if Register read back test failed */
    M_ASSERT_REGSWRITE_RESULT(regWrSts);
}

void GPADC_ResetRelease(void)
{
    uint32_t regWrSts = 0U;
    /* Release Reset GPADC for digital FSM */
    M_REG_STRUCT_SWRITE(MSS_GPADC_RST_CTRL_PTR->bits.b3_Assert, M_GPADC_FSM_DEASSERT_RESET, regWrSts);

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_PTR->bits.b1_ClkTmuxEsdCtrl, 1U, regWrSts);
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_PTR->bits.b1_AnaTestEn, 1U, regWrSts);

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_PTR->bits.b1_AnaogTestTmuxEsdCtrl, 1U, regWrSts);

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcRefBufEn, 1U, regWrSts);

    /* Reset GPADC Ana */
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcReset, M_GPADC_ASSERT_RESET, regWrSts);
    /* Enable GPADC Ana */
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcEn, M_GPADC_ENABLE, regWrSts);

    /* Release GPADC Reset Ana */
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcReset, M_GPADC_DEASSERT_RESET, regWrSts);

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b3_AdcRefSel_2_0, 1U, regWrSts);

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcStartConv, 1U, regWrSts);

    /*! Assert if Register read back test failed */
    M_ASSERT_REGSWRITE_RESULT(regWrSts);
}

GPADC_ConvResultType GPADC_StartGroup(GPADC_channelsGroupSelectType channels)
{
	uint8_t channelIndex = 0;
	GPADC_ConvResultType retVal;
	uint32_t cfgVal;
	uint32_t paramVal;
	GPADC_ResultType gpAdcResult;
    uint32_t regWrSts = 0U;

    GPADCDrvObj.driverStatus = GPADC_BUSY;
    retVal = GPADC_CONV_DONE;

	/* Max Check for the GPADC supported measurement list */
	for(channelIndex =0; channelIndex < MAX_GPADC_MEAS_SOURCES; channelIndex++)
	{
		if(channels.b16_Val & (0x01 << channelIndex))
		{
			if(GPADCDrvObj.driverChannelConfig[channelIndex].isChannelConfigured)
			{
				if(GPADCDrvObj.driverChannelConfig[channelIndex].isChannelBufferedMode)
				{
					M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcInpBufEn, 1U, regWrSts);
				}
				else
				{
					M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcInpBufEn, 0U, regWrSts);
				}
				/* Check if the GPADC is not in use before using it */
				M_ASSERT(M_GPADC_MODE_DISABLE == M_GPADC_REGS_PTR->r_Reg0.bits.b2_DcbistMode);

					/* Change the Mode of operation to IFM */
				M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b2_DcbistMode, M_GPADC_MODE_IFM, regWrSts);
				M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b1_GpadcDebugModeEnable, 1U, regWrSts);

				/* Get the Config Value from DriverObj */
				cfgVal = GPADCDrvObj.driverChannelConfig[channelIndex].channelConfigValue;
				/* Get the Param Value from DriverObj */
				paramVal = GPADCDrvObj.driverChannelConfig[channelIndex].channelParamValue.b32_Val;

				/* Call the GPADC function to convert the param from ana to dig */
				GPADC_Convert(cfgVal, paramVal, &gpAdcResult);

				GPADCDrvObj.ResultBufferPtr[channelIndex] = gpAdcResult.Avg;

			}
			else
			{
				GPADCDrvObj.ResultBufferPtr[channelIndex] = 0U;
				retVal = GPADC_CONV_CHANNEL_CONFIG_MISSING;
			}
		}
		else
		{
			GPADCDrvObj.ResultBufferPtr[channelIndex] = 0U;
		}
	}

    GPADCDrvObj.driverStatus = GPADC_COMPLETED;
    return retVal;
}

GPADC_ConvResultType GPADC_StartSingleChannel(GPADC_MeasExtSrcType channelID, uint16_t *gpadcValue)
{
    GPADC_ConvResultType retVal;
    uint32_t regWrSts = 0U;
	uint32_t cfgVal;
	uint32_t paramVal;
	GPADC_ResultType gpAdcResult;

    if(GPADCDrvObj.driverChannelConfig[channelID].isChannelConfigured)
	{
		if(GPADCDrvObj.driverChannelConfig[channelID].isChannelBufferedMode)
		{
			M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcInpBufEn, 1U, regWrSts);
		}
		else
		{
			M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_AdcInpBufEn, 0U, regWrSts);
		}
		/* Check if the GPADC is not in use before using it */
		M_ASSERT(M_GPADC_MODE_DISABLE == M_GPADC_REGS_PTR->r_Reg0.bits.b2_DcbistMode);

			/* Change the Mode of operation to IFM */
		M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b2_DcbistMode, M_GPADC_MODE_IFM, regWrSts);
		M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b1_GpadcDebugModeEnable, 1U, regWrSts);

		/* Get the Config Value from DriverObj */
		cfgVal = GPADCDrvObj.driverChannelConfig[channelID].channelConfigValue;
		/* Get the Param Value from DriverObj */
		paramVal = GPADCDrvObj.driverChannelConfig[channelID].channelParamValue.b32_Val;

		/* Call the GPADC function to convert the param from ana to dig */
		GPADC_Convert(cfgVal, paramVal, &gpAdcResult);

		*gpadcValue = gpAdcResult.Avg;

		retVal = GPADC_CONV_DONE;
	}
	else
	{
		*gpadcValue = 0U;
		retVal = GPADC_CONV_CHANNEL_CONFIG_MISSING;
	}

    return retVal;
}

void GPADC_StopGroup(void)
{
	uint32_t regWrSts = 0U;

	/* Disable GPADC after use */
	M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b2_DcbistMode, M_GPADC_MODE_DISABLE, regWrSts);
    GPADCDrvObj.driverStatus = GPADC_IDLE;
}

void GPADC_Cfg(uint32_t cfgVal, uint32_t paramVal)
{
    uint32_t regWrSts = 0U;

    /* Write 32 bit Dynamic value to config reg */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg2.bits.b32_ConfigValueIfm, cfgVal, regWrSts);

    /* Write Param Value, Collect Samples , Skip Samples */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg3.b32_Reg, paramVal, regWrSts);

    /* Raise a fatal error if any of above register writes failed */
    M_ASSERT_REGSWRITE_RESULT(regWrSts);

    return;
}

void GPADC_SWTrigger(void)
{
    uint32_t pmuCount;
    uint32_t countDelta = 0U;
    uint32_t regWrSts = 0U;

    /* Get the PMU count */
    pmuCount = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);

    /* Trigger the ADC Conversion in IFM mode */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg1.bits.b1_GpadcStartBypVal, 1U, regWrSts);

    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg1.bits.b1_GpadcTrigger, 0U, regWrSts);
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg1.bits.b1_GpadcTrigger, 1U, regWrSts);

    /* Wait for GPADC conversion to complete */
    while (countDelta < M_GPADC_TIMEOUT_MAX)
    {
        /* Break if the Status bit is set to 1 */
        if (1U == M_GPADC_REGS_PTR->r_Reg17.bits.b1_GpadcIfmDoneStatus)
        {
            break;
        }

        /* Compute the PMU count delta */
        countDelta = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM) - pmuCount;
    }

    /* Raise an exception if timeout occured */
    M_ASSERT(1U == M_GPADC_REGS_PTR->r_Reg17.bits.b1_GpadcIfmDoneStatus);

    /* Clear the IFM status bit */
    M_REG_STRUCT_SCLEAR(M_GPADC_REGS_PTR->r_Reg18.bits.b1_GpadcIfmDoneClr, 1U, regWrSts);

}

void GPADC_ReadSamplingResult(uint8_t numSamples, GPADC_ResultType *gpAdcResult)
{
    uint32_t regWrSts = 0U;
    uint32_t sumReg;

    /* Read the Results and populate the Structure */
    if (M_NULL != gpAdcResult)
    {
        /* Read the Sum of ADC samples */
        sumReg = (uint32_t)(M_GPADC_REGS_PTR->r_Reg14.bits.b20_SumIfm);
        gpAdcResult->Sum = sumReg;

        /* Read the Min of ADC samples */
        gpAdcResult->Min = (uint16_t)(M_GPADC_REGS_PTR->r_Reg15.bits.b10_MinGpadc);

        /* Read the Max of ADC samples */
        gpAdcResult->Max = (uint16_t)(M_GPADC_REGS_PTR->r_Reg15.bits.b10_MaxGpadc);

        /* Read the Avg of ADC samples */
        if ((uint8_t)0U != numSamples)
        {
            gpAdcResult->Avg = (uint16_t)(sumReg/ (uint32_t)numSamples);
        }
        else
        {
            /* Spin here if number of samples is zero */
            M_ASSERT(M_NULL);
        }
    }
    else
    {
        /* Spin here if Null Pointer */
        M_ASSERT(M_NULL);
    }

    /* Write Param Value to 0 to avoid any contention */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg3.b32_Reg, 0U, regWrSts);

    /* Disable GPADC after use */
    M_REG_STRUCT_SWRITE(M_GPADC_REGS_PTR->r_Reg0.bits.b2_DcbistMode, M_GPADC_MODE_DISABLE, regWrSts);

    return;
}

void GPADC_Convert(uint32_t cfgVal, uint32_t paramVal, GPADC_ResultType *gpAdcResult)
{

    uint8_t numSamples;

    /* Configure the GPADC by writing Config reg and Param reg*/
    GPADC_Cfg(cfgVal, paramVal);

    /* Trigger the GPADC for Conversion */
    GPADC_SWTrigger();

    numSamples = (uint8_t)((paramVal & 0xFF00U) >> 8U);

    /* Get the GPADC conversion results */
    GPADC_ReadSamplingResult(numSamples, gpAdcResult);
}

uint8_t GPADC_ConvSkipTimeToCode(uint32_t skipSamples)
{
    uint32_t result;
    uint8_t  twoPwr = 0U,
           skipSampProg;

    /*! find the nearest a*2^b values to program to Hw */
    result = skipSamples;
    while (result >= (uint32_t)16U)
    {
        twoPwr++;
        /*Invalid LDRA warning - c_twoPwr can not be zero */
        /*LDRA_INSPECTED 127 D */
        result = (skipSamples + ((uint32_t)1U << (twoPwr - 1U))) / ((uint32_t)1U << twoPwr);

        /*! Max two to the power (b) is 3 bits */
        if (twoPwr == 7U)
        {
            break;
        }
    }

    /*! Max value of a is 4 bits */
    if (result >= 16U)
    {
        result = 15U;
    }

    /*! The programming skip sample values for first parameter */
    skipSampProg = (((twoPwr & 0x07U) << 4U) | ((uint8_t)result & 0xFU));

    return skipSampProg;
}

/*
 * END OF gpadc_soc.c FILE
 */
