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
 *   \file  drv/gpadc/soc/awr294x/gpadc_soc_temp.h
 *
 *   \brief
 *      This is the platform specific header file for the GPADC driver
 *      which specifies the GPADC register addresses and structures/ unions
 *      used in the GPADC temperature sensor driver
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
 #ifndef DRV_GPADC_SOC_TEMP_H
 #define DRV_GPADC_SOC_TEMP_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

 #include <ti/drv/gpadc/gpadc_cfg.h>

/****************************************************************************************
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define EFUSE_TRIM_TEMPERATURE_CONST           523
#define EFUSE_TRIM_TEMPERATURE_DIV_CONST       10

#define ZERO_PT_TRIM_FIXED_TRIM_TEMP                    110
#define ZERO_PT_TRIM_FIXED_DIG_TEMP_SENSOR_TRIM_30C     305U
#define ZERO                                            0


#define ZERO_PT_TRIM_FIXED_SLOPE -1.05
#define ONE_PT_TRIM_FIXED_SLOPE -1.05

#define EFUSE1_ROW_14_FUSEROM_VER_START_BIT    20U
#define EFUSE1_ROW_14_FUSEROM_VER_STOP_BIT     24U

#define EFUSE1_ROW_36_TRIM_TEMPERATURE_30C_START_BIT    15U
#define EFUSE1_ROW_36_TRIM_TEMPERATURE_30C_STOP_BIT     25U

#define EFUSE1_ROW_33_TRIM_TEMPERATURE_125C_START_BIT    0U
#define EFUSE1_ROW_33_TRIM_TEMPERATURE_125C_STOP_BIT     10U

#define EFUSE1_ROW_37_DIG_DSP_TEMP_SENSOR_TRIM0_30C_START_BIT  5U
#define EFUSE1_ROW_37_DIG_DSP_TEMP_SENSOR_TRIM0_30C_STOP_BIT   14U

#define EFUSE1_ROW_37_DIG_HWA_TEMP_SENSOR_TRIM1_30C_START_BIT  15U
#define EFUSE1_ROW_37_DIG_HWA_TEMP_SENSOR_TRIM1_30C_STOP_BIT   24U

#define EFUSE1_ROW_38_DIG_HSM_TEMP_SENSOR_TRIM2_30C_START_BIT  0U
#define EFUSE1_ROW_38_DIG_HSM_TEMP_SENSOR_TRIM2_30C_STOP_BIT  9U

#define EFUSE1_ROW_34_DIG_DSP_TEMP_SENSOR_TRIM0_125C_START_BIT  5U
#define EFUSE1_ROW_34_DIG_DSP_TEMP_SENSOR_TRIM0_125C_STOP_BIT   14U

#define EFUSE1_ROW_34_DIG_HWA_TEMP_SENSOR_TRIM1_125C_START_BIT  15U
#define EFUSE1_ROW_34_DIG_HWA_TEMP_SENSOR_TRIM1_125C_STOP_BIT   24U

#define EFUSE1_ROW_35_DIG_HSM_TEMP_SENSOR_TRIM2_125C_START_BIT  0U
#define EFUSE1_ROW_35_DIG_HSM_TEMP_SENSOR_TRIM2_125C_STOP_BIT   9U


/******************************************************************************
 * TYPE-DEFINE STRUCT/ENUM/UNION DEFINITIONS
 ******************************************************************************
 */

/**
 * \brief
 *  GPADC Temperature Sensors List
 *
 * \details
 *  Enumeration which describes the temperature sensors available for GPADC measurement
 */
typedef enum
{
    /**
     * \brief   0x0 - DIG_DSP_TEMP_SENSOR
     */
	GPADC_DIG_DSP_TEMP_SENSOR = 0,
    /**
     * \brief   0x1 - DIG_HWA_TEMP_SENSOR
     */
	GPADC_DIG_HWA_TEMP_SENSOR ,
    /**
     * \brief   0x2 - DIG_HSM_TEMP_SENSOR
     */
	GPADC_DIG_HSM_TEMP_SENSOR ,

    /**
     * \brief   0x3 - MAX_TEMP_SENSORS
     */
	MAX_GPADC_TEMP_SENSORS
}GPADC_TempSensorSrcType;

/*! \brief
* Temperature sensors mux values
*/
typedef struct
{
    /**
     * \brief
     *   Unique muxing config value per sensor
     */
    uint32_t channelConfigValue;

    /**
     * \brief
     *  Channel parameters including channel paramVal(subsystem-type), collect samples
     *    and skip samples
     */
    uint8_t channelParamValue;

    /**
     * \brief
     *   Number of samples to be skipped before collecting samples per
     *   input channel
     */
    uint32_t skipSamples;
    /**
     * \brief
     *   Number of samples to be collected for conversion per each input channel
     */
    uint8_t  collectSamples;
} GPADC_TempSensMuxType;

/*! \brief
* The Temperature sensor trim parameters structure
*/
typedef struct
{
   uint16_t      FuseROMVer;
   uint16_t      TrimTemp30C;
   uint16_t      TrimTemp125C;
   uint16_t      TrimIntercept30C[MAX_GPADC_TEMP_SENSORS];
   uint16_t      TrimIntercept125C[MAX_GPADC_TEMP_SENSORS];
} GPADC_EfuseTempTrimType;

/*! \brief
* The Temperature sensor trim parameters structure
*/
typedef struct
{
   uint16_t     TrimTemp30C;
   uint16_t     TrimTemp125C;
   uint16_t     TrimIntercept30C[MAX_GPADC_TEMP_SENSORS];
   uint16_t     TrimIntercept125C[MAX_GPADC_TEMP_SENSORS];
   float        Slope[MAX_GPADC_TEMP_SENSORS];
   float        InterceptTemp;
} GPADC_TempSensTrimType;

/*! \brief
* The Temperature sensor values structure
*/
typedef struct
{
   int16_t     DigDspTempValue;
   int16_t     DigHwaTempValue;
   int16_t     DigHsmTempValue;
} GPADC_TempSensValueType;

/******************************************************************************
 * EXTERN GLOBAL VARIABLES/DATA-TYPES DECLARATIONS
 ******************************************************************************
 */

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */
void GPADC_initTempMeasurement(void);
GPADC_StdReturnType GPADC_readTemperature(uint8_t numAverages, GPADC_TempSensValueType * tempValuesPtr);

#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF DRV_GPADC_SOC_TEMP_H
 */
