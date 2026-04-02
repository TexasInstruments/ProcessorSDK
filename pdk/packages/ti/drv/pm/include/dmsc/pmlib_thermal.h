/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018-2019
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
 *  \ingroup PM_LIB PM LIB API
 *  \defgroup TEMPERATURE Temperature Configuration
 *   The Thermal LIB Configuration provides API to read the junction
 *   temperature of the different BGAP temperature sensors available in the SoC.
 *   This LIB allows setting of the right threshold values for the Temperature
 *   hot and cold interrupts.
 *
 * @{
 */

/**
 *  \file  pmlib_thermal.h
 *
 *  \brief This file contains the interface declaration for the Thermal LIB.
 *         The Thermal LIB supports the following features:
 *         - To return the temperature of a given voltage domain.
 *         - To mask/unmask the hot event for the given sensor.
 *         - To set the band gap threshold hot and cold value for the alert
 *           operation.
 *         - To check if a given voltage domain has the hot/cold alert set.
 */

#ifndef PMLIB_THERMAL_H_
#define PMLIB_THERMAL_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Max number of SENSORS supported by VTM IP*/
#define PMLIB_VTM_MAX_NUM_SENS                                            (0x8U)

/**
 *  \anchor Pmlib_thermalTempRange
 *  \name PM lib thermal Temperature range
 *  @{
 *   Rnge of temperature that can be read by/written to(as event threshold)
 *      the VTM IP
 */
#define PMLIB_VTM_TEMP_MIN_VAL                                          (-40000)
#define PMLIB_VTM_TEMP_MAX_VAL                                          (125000)
/* @} */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                      Global Variables Declarations                         */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/** \brief  Turn on the Thermal Sensor.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors.
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS   If the sensor is turned on.
 *                        - #PM_BADARGS   If sensId is invalid.
 */
int32_t PMLIBThermalTurnOnSensor(uint8_t sensId);

/** \brief  Turn off the Thermal Sensor.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors.
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS   If the sensor is turned off.
 *                        - #PM_BADARGS   If sensId is invalid.
 */
int32_t PMLIBThermalTurnOffSensor(uint8_t sensId);

/** \brief  Get the Band gap Temperature Sensor value for a given voltage domain
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors.
 *
 *  \param  tempInMilliDegree   Pointer to the current temperature. Gives the
 *                              temperature which corresponds to the read ADC value.
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS   If the temperature is read correctly.
 *                        - #PM_BADARGS   If sensId is invalid.
 */
int32_t PMLIBThermalGetCurrTemperature(uint8_t sensId,
                                    int32_t *tempInMilliDegree);

/** \brief  Turn on the Thermal Range Alert. This is used to reset the device
 *          when the temperature is > 125 deg C.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors.
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS   If the alert is turned on.
 *                        - #PM_BADARGS   If sensId is invalid.
 */
int32_t PMLIBThermalEnableOutRangeAlert(uint8_t sensId);

/** \brief  Turn off the Thermal Range Alert. This is used to reset the device
 *          when the temperature is > 125 deg C.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors.
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS   If the alert is turned off.
 *                        - #PM_BADARGS   If sensId is invalid.
 */
int32_t PMLIBThermalDisableOutRangeAlert(uint8_t sensId);

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
/** \brief  Set the Max and Safe TSHUT Thresholds.
 *
 *  \param maxTempMilliDegree  Maximum Temperature at which the reset should be triggered.
 *  \param safeTempMilliDegree Safet Temperature at which the reset is released.
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS   If the thresholds are set.
 *                        - #PM_BADARGS   If the thresholds are not set.
 */
int32_t PMLIBThermalSetMaxThermalOutRange(uint32_t maxTempMilliDegree,
                                          uint32_t safeTempMilliDegree);
#endif

/** \brief  Enable a Hot alert event for the threshold temperature set by the
 *          API PMLIBThermalSetHotThreshold.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS   If the hot event is enabled correctly.
 *                        - #PM_BADARGS   If sensId is invalid.
 */
int32_t PMLIBThermalEnableHotEvent(uint8_t sensId);

/** \brief  Disable a Hot alert event for the threshold temperature set by the
 *          API PMLIBThermalSetHotThreshold.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS  If the hot event is disabled correctly.
 *                        - #PM_BADARGS  If sensId is invalid.
 */
int32_t PMLIBThermalDisableHotEvent(uint8_t sensId);

/** \brief  Enable a Cold alert event for the threshold temperature set by the
 *          API PMLIBThermalSetColdThreshold.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS  If the cold event is enabled correctly.
 *                        - #PM_BADARGS  If sensId is invalid.
 */
int32_t PMLIBThermalEnableColdEvent(uint8_t sensId);

/** \brief  Disable a Cold alert event for the threshold temperature set by the
 *          API PMLIBThermalSetColdThreshold.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \return status          Status of the API call.
 *                        - #PM_SUCCESS  If the cold event is disabled correctly
 *                        - #PM_BADARGS  If sensId is invalid.
 */
int32_t PMLIBThermalDisableColdEvent(uint8_t sensId);

/** \brief  This API configures the high temperature threshold for generating
 *          thermal alerts through programming the
 *          VTM_TMPSENS[n]_CTRL[21:12] TH1_VAL bit fields.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \param  tempInMilliDegree   Temperature Threshold in milli Degree Celsius.
 *
 *  \return status              Status of the API call.
 *                            - #PM_SUCCESS   If the threshold is programmed
 *                                           correctly.
 *                            - #PM_BADARGS   If sensId is not
 *                                           correct or the temperature is
 *                                           out of the range supported.
 */
int32_t PMLIBThermalSetHotThreshold(uint8_t sensId,
                                 int32_t tempInMilliDegree);

/** \brief  This API configures the Low temperature threshold for generating
 *          thermal alerts through programming the
 *          VTM_TMPSENS[n]_CTRL[31:28] TH0_DEC_VAL bit fields.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \param  tempInMilliDegree   Temperature Threshold in milli Degree Celsius.
 *
 *  \return status              Status of the API call.
 *                            - #PM_SUCCESS   If the threshold is programmed
 *                                           correctly.
 *                            - #PM_BADARGS   If sensId is not
 *                                           correct or the temperature is
 *                                           out of the range supported.
 */
int32_t PMLIBThermalSetColdThreshold(uint8_t sensId,
                                  int32_t tempInMilliDegree);

/** \brief  This API returns the configured high temperature threshold
 *          range programmed into the
 *          VTM_TMPSENS[n]_CTRL[21:12] TH1_VAL bit fields.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \param  thresholdTempRange   Pointer to the threshold temperature range.
 *                               Gives the max and min temperature which
 *                               corresponds to the read threshold ADC value.
 *
 *  \return status               Status of the API call.
 *                             - #PM_SUCCESS   If the threshold is read
 *                                            correctly.
 *                             - #PM_BADARGS   If sensId is not
 *                                            correct.
 */
int32_t PMLIBThermalGetHotThreshold(uint8_t sensId,
                                 int32_t *tempInMilliDegree);

/** \brief  This API returns the configured low temperature threshold
 *          range programmed using
 *          VTM_TMPSENS[n]_CTRL[31:28] TH0_DEC_VAL and
 *          VTM_TMPSENS[n]_CTRL[21:12] TH1_VAL bit fields.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \param  thresholdTempRange      Pointer to the threshold temperature range.
 *                                  Gives the max and min temperature which
 *                                  corresponds to the read threshold ADC value.
 *
 *  \return status                  Status of the API call.
 *                                - #PM_SUCCESS   If the threshold is read
 *                                                correctly.
 *                                - #PM_BADARGS   If sensId is not
 *                                               correct.
 */
int32_t PMLIBThermalGetColdThreshold(uint8_t sensId,
                                  int32_t *tempInMilliDegree);

/** \brief  The non masked (raw) comparator outputs are available for reading
 *          through the corresponding bits in the CTRL_CORE_BANDGAP_STATUS_1 and
 *          CTRL_CORE_BANDGAP_STATUS_2 registers. The high temperature threshold
 *          comparator outputs are read through the HOT_x bits using this API.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \param  hotStatus           High temperature threshold comparator output
 *
 *  \return status              Status of the API call.
 *                              - #PM_SUCCESS   If the output is read correctly.
 *                              - #PM_BADARGS   If the voltage domain is not
 *                                           correct or the pointer hotStatus
 *                                           is NULL.
 */
int32_t PMLIBThermalGetHotAlertStatus(uint8_t sensId, uint32_t *hotStatus);

/** \brief  The non masked (raw) comparator outputs are available for reading
 *          through the corresponding bits in the CTRL_CORE_BANDGAP_STATUS_1 and
 *          CTRL_CORE_BANDGAP_STATUS_2 registers. The low temperature threshold
 *          comparator outputs are read through the COLD_x bits using this API.
 *
 *  \param  sensId          Sensor ID. Must be less than the value of
 *                          #PMLIBThermalGetNumSensors .
 *
 *  \param  coldStatus          Low temperature threshold comparator output
 *
 *  \return status              Status of the API call.
 *                             - #PM_SUCCESS   If the output is read correctly.
 *                             - #PM_BADARGS   If the voltage domain is not
 *                                           correct or the pointer coldStatus
 *                                           is NULL.
 */
int32_t PMLIBThermalGetColdAlertStatus(uint8_t   sensId,
                                    uint32_t *coldStatus);


/** \brief  Get number of temperature sensors associated with this VTM IP.
 *
 *  \return numSens          number of sensors.
 */
uint8_t PMLIBThermalGetNumSensors(void);


#ifdef __cplusplus
}
#endif

#endif

/* @} */


