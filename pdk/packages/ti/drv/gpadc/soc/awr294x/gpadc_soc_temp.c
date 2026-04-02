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
 *   \file gpadc_soc_temp.c
 *
 *   \brief The file implements the Platform specific Temperature sensors
 *          readout
 */

 /******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stdint.h>
#include <string.h>
#include <ti/osal/DebugP.h>
#include <ti/csl/soc/awr294x/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/awr294x/src/cslr_top_ctrl.h>
#include "gpadc.h"
#include "gpadc_soc.h"
#include "gpadc_soc_temp.h"
/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/*! \brief
 * GPADC Temperature Sensors Param Lookup Table
 * ConfigValue, ParamValue, SkipSamples, CollectSamples
 */
GPADC_TempSensMuxType GPADC_TempSensConfigParamTab[MAX_GPADC_TEMP_SENSORS] = {
    /*! ConfigValue, ParamValue, SkipSamples, CollectSamples */
    {    0x40000U,     0x2U,        0U,           255U},  /* #0 GPADC_DIG_DSP_TEMP_SENSOR */
    {    0x80000U,     0x2U,        0U,           255U},  /* #1 GPADC_DIG_HWA_TEMP_SENSOR */
    {   0x100000U,     0x2U,        0U,           255U}   /* #2 GPADC_DIG_HSM_TEMP_SENSOR */
};

GPADC_TempSensTrimType tempSensTrimSlopeValues;

GPADC_EfuseTempTrimType efuseTempTrimValues;

CSL_top_ctrlRegs *topCtrlRegs = (CSL_top_ctrlRegs *)CSL_MSS_TOPRCM_U_BASE;
/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */
static uint16_t GPADC_EfuseExtractTrims (volatile uint32_t reg, uint8_t msb, uint8_t lsb);
static int32_t GPADC_calculateTemp(uint16_t gpadcTempCode, uint8_t index);
static void GPADC_computeTempSlope(void);

void GPADC_initTempMeasurement(void)
{
    int16_t trimTemp;
    memset(&efuseTempTrimValues,0,sizeof(efuseTempTrimValues));

    efuseTempTrimValues.FuseROMVer = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_14, \
    		EFUSE1_ROW_14_FUSEROM_VER_STOP_BIT, EFUSE1_ROW_14_FUSEROM_VER_START_BIT);
	trimTemp = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_36, \
    		EFUSE1_ROW_36_TRIM_TEMPERATURE_30C_STOP_BIT , EFUSE1_ROW_36_TRIM_TEMPERATURE_30C_START_BIT);

    efuseTempTrimValues.TrimTemp30C = trimTemp;

    trimTemp = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_33, \
    		EFUSE1_ROW_33_TRIM_TEMPERATURE_125C_STOP_BIT , EFUSE1_ROW_33_TRIM_TEMPERATURE_125C_START_BIT);

    efuseTempTrimValues.TrimTemp125C = trimTemp;


	if((0U == efuseTempTrimValues.FuseROMVer) && (0U == efuseTempTrimValues.TrimTemp30C) \
			&& (0U == efuseTempTrimValues.TrimTemp125C))
	{
		tempSensTrimSlopeValues.TrimTemp30C = ZERO;
		tempSensTrimSlopeValues.TrimTemp125C = ZERO;

		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_DSP_TEMP_SENSOR] = ZERO_PT_TRIM_FIXED_DIG_TEMP_SENSOR_TRIM_30C;
		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_HWA_TEMP_SENSOR] = ZERO_PT_TRIM_FIXED_DIG_TEMP_SENSOR_TRIM_30C;
		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_HSM_TEMP_SENSOR] = ZERO_PT_TRIM_FIXED_DIG_TEMP_SENSOR_TRIM_30C;

		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_DSP_TEMP_SENSOR] = ZERO;
		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_HWA_TEMP_SENSOR] = ZERO;
		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_HSM_TEMP_SENSOR] = ZERO;

		tempSensTrimSlopeValues.Slope[GPADC_DIG_DSP_TEMP_SENSOR] = ZERO_PT_TRIM_FIXED_SLOPE;
		tempSensTrimSlopeValues.Slope[GPADC_DIG_HWA_TEMP_SENSOR] = ZERO_PT_TRIM_FIXED_SLOPE;
		tempSensTrimSlopeValues.Slope[GPADC_DIG_HSM_TEMP_SENSOR] = ZERO_PT_TRIM_FIXED_SLOPE;

		tempSensTrimSlopeValues.InterceptTemp = ZERO_PT_TRIM_FIXED_TRIM_TEMP;
	}
	else if((1U == efuseTempTrimValues.FuseROMVer) || (0U != efuseTempTrimValues.TrimTemp30C) \
			|| (0U != efuseTempTrimValues.TrimTemp125C))
	{
		tempSensTrimSlopeValues.TrimTemp30C = efuseTempTrimValues.TrimTemp30C;
		tempSensTrimSlopeValues.TrimTemp125C = efuseTempTrimValues.TrimTemp125C;

		efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_DSP_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_37, \
				EFUSE1_ROW_37_DIG_DSP_TEMP_SENSOR_TRIM0_30C_STOP_BIT, EFUSE1_ROW_37_DIG_DSP_TEMP_SENSOR_TRIM0_30C_START_BIT);
		efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HWA_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_37, \
				EFUSE1_ROW_37_DIG_HWA_TEMP_SENSOR_TRIM1_30C_STOP_BIT, EFUSE1_ROW_37_DIG_HWA_TEMP_SENSOR_TRIM1_30C_START_BIT);
		efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HSM_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_38, \
				EFUSE1_ROW_38_DIG_HSM_TEMP_SENSOR_TRIM2_30C_STOP_BIT, EFUSE1_ROW_38_DIG_HSM_TEMP_SENSOR_TRIM2_30C_START_BIT);


		efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_DSP_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_34, \
				EFUSE1_ROW_34_DIG_DSP_TEMP_SENSOR_TRIM0_125C_STOP_BIT, EFUSE1_ROW_34_DIG_DSP_TEMP_SENSOR_TRIM0_125C_START_BIT);
		efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HWA_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_34, \
				EFUSE1_ROW_34_DIG_HWA_TEMP_SENSOR_TRIM1_125C_STOP_BIT, EFUSE1_ROW_34_DIG_HWA_TEMP_SENSOR_TRIM1_125C_START_BIT);
		efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HSM_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_35, \
				EFUSE1_ROW_35_DIG_HSM_TEMP_SENSOR_TRIM2_125C_STOP_BIT, EFUSE1_ROW_35_DIG_HSM_TEMP_SENSOR_TRIM2_125C_START_BIT);

		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_DSP_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_DSP_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_HWA_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HWA_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_HSM_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HSM_TEMP_SENSOR];

		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_DSP_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_DSP_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_HWA_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HWA_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_HSM_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HSM_TEMP_SENSOR];

		tempSensTrimSlopeValues.Slope[GPADC_DIG_DSP_TEMP_SENSOR] = ONE_PT_TRIM_FIXED_SLOPE;
		tempSensTrimSlopeValues.Slope[GPADC_DIG_HWA_TEMP_SENSOR] = ONE_PT_TRIM_FIXED_SLOPE;
		tempSensTrimSlopeValues.Slope[GPADC_DIG_HSM_TEMP_SENSOR] = ONE_PT_TRIM_FIXED_SLOPE;

		tempSensTrimSlopeValues.InterceptTemp = ( tempSensTrimSlopeValues.TrimTemp30C - EFUSE_TRIM_TEMPERATURE_CONST )/ EFUSE_TRIM_TEMPERATURE_DIV_CONST;
	}
	else if(2U == efuseTempTrimValues.FuseROMVer)
	{
		tempSensTrimSlopeValues.TrimTemp30C = efuseTempTrimValues.TrimTemp30C;
		tempSensTrimSlopeValues.TrimTemp125C = efuseTempTrimValues.TrimTemp125C;

		efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_DSP_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_37, \
				EFUSE1_ROW_37_DIG_DSP_TEMP_SENSOR_TRIM0_30C_STOP_BIT, EFUSE1_ROW_37_DIG_DSP_TEMP_SENSOR_TRIM0_30C_START_BIT);
		efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HWA_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_37, \
				EFUSE1_ROW_37_DIG_HWA_TEMP_SENSOR_TRIM1_30C_STOP_BIT, EFUSE1_ROW_37_DIG_HWA_TEMP_SENSOR_TRIM1_30C_START_BIT);
		efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HSM_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_38, \
				EFUSE1_ROW_38_DIG_HSM_TEMP_SENSOR_TRIM2_30C_STOP_BIT, EFUSE1_ROW_38_DIG_HSM_TEMP_SENSOR_TRIM2_30C_START_BIT);


		efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_DSP_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_34, \
				EFUSE1_ROW_34_DIG_DSP_TEMP_SENSOR_TRIM0_125C_STOP_BIT, EFUSE1_ROW_34_DIG_DSP_TEMP_SENSOR_TRIM0_125C_START_BIT);
		efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HWA_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_34, \
				EFUSE1_ROW_34_DIG_HWA_TEMP_SENSOR_TRIM1_125C_STOP_BIT, EFUSE1_ROW_34_DIG_HWA_TEMP_SENSOR_TRIM1_125C_START_BIT);
		efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HSM_TEMP_SENSOR] = GPADC_EfuseExtractTrims(topCtrlRegs->EFUSE1_ROW_35, \
				EFUSE1_ROW_35_DIG_HSM_TEMP_SENSOR_TRIM2_125C_STOP_BIT, EFUSE1_ROW_35_DIG_HSM_TEMP_SENSOR_TRIM2_125C_START_BIT);

		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_DSP_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_DSP_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_HWA_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HWA_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept30C[GPADC_DIG_HSM_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept30C[GPADC_DIG_HSM_TEMP_SENSOR];

		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_DSP_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_DSP_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_HWA_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HWA_TEMP_SENSOR];
		tempSensTrimSlopeValues.TrimIntercept125C[GPADC_DIG_HSM_TEMP_SENSOR] = efuseTempTrimValues.TrimIntercept125C[GPADC_DIG_HSM_TEMP_SENSOR];


		GPADC_computeTempSlope();

		tempSensTrimSlopeValues.InterceptTemp = ( tempSensTrimSlopeValues.TrimTemp30C - EFUSE_TRIM_TEMPERATURE_CONST )/ EFUSE_TRIM_TEMPERATURE_DIV_CONST;

	}

}


GPADC_StdReturnType GPADC_readTemperature(uint8_t numAverages, GPADC_TempSensValueType * tempValuesPtr)
{
	uint16_t gpadcTempVal[3] = {0}, gpadcCode;
	int32_t gpadcTempValSum[3] = {0}, tempVal;
	GPADC_ConfigType tempSensorsCfg;
	uint8_t index, index2;
	GPADC_channelsGroupSelectType channels;
	GPADC_ConvResultType convRes;
	GPADC_StdReturnType retVal = E_OK;
    uint32_t regWrSts = 0U;
    static uint8_t avgSamples = 0;
    avgSamples = numAverages;

    memset(&tempSensorsCfg,0,sizeof(tempSensorsCfg));

    tempSensorsCfg.triggSrc = GPADC_TRIGG_SRC_SW;
    tempSensorsCfg.convMode = GPADC_ONESHOT_CONV_MODE;

	for (index = 0; index < MAX_GPADC_TEMP_SENSORS; index++)
	{
		tempSensorsCfg.channelConfig[index].channelID = (GPADC_MeasExtSrcType)index;
		tempSensorsCfg.channelConfig[index].isConfigured = TRUE;
		tempSensorsCfg.channelConfig[index].isBufferedMode = TRUE;
		tempSensorsCfg.channelConfig[index].useLuTable = FALSE;
		tempSensorsCfg.channelConfig[index].channelConfigValue = GPADC_TempSensConfigParamTab[index].channelConfigValue;
		tempSensorsCfg.channelConfig[index].channelParamValue = GPADC_TempSensConfigParamTab[index].channelParamValue;
		tempSensorsCfg.channelConfig[index].skipSamples = GPADC_TempSensConfigParamTab[index].skipSamples;
		tempSensorsCfg.channelConfig[index].collectSamples = GPADC_TempSensConfigParamTab[index].collectSamples;
	}

	channels.bits.b9_ChannelSelectionBitMap = 0x007;

    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_TsSeInpBufEn, 1U, regWrSts);
    M_REG_STRUCT_SWRITE(MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR->bits.b1_TsDiffInpBufEn, 0U, regWrSts);

	GPADC_Init(&tempSensorsCfg);

	GPADC_SetupResultBuffer(&gpadcTempVal[0]);

	for( index = 0; index < avgSamples; index++)
	{
		convRes = GPADC_StartGroupConversion(channels);

		if(GPADC_CONV_DONE == convRes)
		{
			retVal = E_OK;
		}
		else if(GPADC_CONV_CHANNEL_CONFIG_MISSING == convRes)
		{
			retVal = E_NOT_OK;
			break;
		}
		else
		{
			retVal = E_NOT_OK;
			break;
		}

		for( index2 = 0; index2 < MAX_GPADC_TEMP_SENSORS; index2++)
		{
			gpadcCode = gpadcTempVal[index2];
			tempVal = GPADC_calculateTemp(gpadcCode, index2);
			gpadcTempValSum[index2] = gpadcTempValSum[index2] + tempVal;
		}
	}

	tempValuesPtr->DigDspTempValue = gpadcTempValSum[GPADC_DIG_DSP_TEMP_SENSOR] / avgSamples;
	tempValuesPtr->DigHwaTempValue = gpadcTempValSum[GPADC_DIG_HWA_TEMP_SENSOR] / avgSamples;
	tempValuesPtr->DigHsmTempValue = gpadcTempValSum[GPADC_DIG_HSM_TEMP_SENSOR] / avgSamples;

	return retVal;
}

static int32_t GPADC_calculateTemp(uint16_t gpadcTempCode, uint8_t index)
{
	int32_t tempVal;

	tempVal = ((((int32_t)gpadcTempCode - (int32_t)tempSensTrimSlopeValues.TrimIntercept30C[index]) \
			/ (int32_t)tempSensTrimSlopeValues.Slope[index]) \
			+ tempSensTrimSlopeValues.InterceptTemp);
	return tempVal;
}

static void GPADC_computeTempSlope(void)
{
    uint8_t index;

    for( index = 0; index < MAX_GPADC_TEMP_SENSORS; index++)
    {
    	tempSensTrimSlopeValues.Slope[index] = (tempSensTrimSlopeValues.TrimIntercept125C[index] - tempSensTrimSlopeValues.TrimIntercept30C[index]) \
    			/ (tempSensTrimSlopeValues.TrimTemp125C - tempSensTrimSlopeValues.TrimTemp30C);
    }

}

static uint16_t GPADC_EfuseExtractTrims (volatile uint32_t reg, uint8_t msb, uint8_t lsb)
{
    uint32_t    mask;
    uint8_t     bits;
    uint16_t    value;

    /* Compute the mask: */
    bits = (msb - lsb + 1U);
    mask = (uint32_t)((uint32_t)1U << bits);
    mask = mask - 1U;

    value = (uint16_t)((reg >> lsb) & mask);
    return value;
}

/*
 * END OF gpadc_soc_temp.c FILE
 */
