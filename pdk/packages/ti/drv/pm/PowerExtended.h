/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 * \ingroup PM_RTOS PMRTOS
 *
 * ============================================================================
 *  @file       PowerExtended.h
 *
 *  @brief      Power manager interface extension
 *
 *  The Power header file should be included in an application as follows:
 *  @code
 *  #include <ti/drv/pm/Power.h>
 *  #include <ti/drv/pm/PowerExtended.h>
 *  @endcode
 *
 *  Refer to @ref Power.h for a complete description of APIs.
 *
 *  ============================================================================
 * @{
 *
 */
 
#ifndef POWEREXTENDED_H
#define POWEREXTENDED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Power constraints */
/*! Power Constraint to Disallow sleep */
#define Power_DISALLOW_SLEEP             (0U)
/*! Power Constraint to Disallow deep sleep */
#define Power_DISALLOW_DEEPSLEEP         (1U)
/*! Power Constraint to Disallow shutdown */
#define Power_DISALLOW_SHUTDOWN          (2U)
/*! Power Constraint to Disallow performance level low */
#define Power_DISALLOW_PERFLEVEL_LOW     (3U)
/*! Power Constraint to Disallow performance level nom */
#define Power_DISALLOW_PERFLEVEL_NOM     (4U)
/*! Power Constraint to Disallow performance level OD */
#define Power_DISALLOW_PERFLEVEL_OD      (5U)
/*! Power Constraint to Disallow performance level High */
#define Power_DISALLOW_PERFLEVEL_HIGH    (6U)
/*! Power Constraint to Disallow performance level changes */
#define Power_DISALLOW_PERF_CHANGES      (7U)
/*! Total number of Power Constraints */
#define Power_NUMCONSTRAINTS             (8U)

/* Performance levels */
/*! Power Performance level low */
#define Power_PERFLEVEL_LOW              (0U)
/*! Power Performance level Nom */
#define Power_PERFLEVEL_NOM              (1U)
/*! Power Performance level Od */
#define Power_PERFLEVEL_OD               (2U)
/*! Power Performance level High */
#define Power_PERFLEVEL_HIGH             (3U)
/*! Total number of Power Performance levels */
#define Power_NUMPERFLEVELS              (4U)

/* Power events */
/*! Power Event to enter sleep */
#define Power_EV_ENTERING_SLEEP          (1U)
/*! Power Event to enter deep sleep */
#define Power_EV_ENTERING_DEEPSLEEP      (2U)
/*! Power Event to enter shutdown */
#define Power_EV_ENTERING_SHUTDOWN       (4U)
/*! Power Event to wake up from sleep */
#define Power_EV_AWAKE_SLEEP             (8U)
/*! Power Event to wake up from deep sleep */
#define Power_EV_AWAKE_DEEPSLEEP         (16U)
/*! Power Event to start changing performance level */
#define Power_EV_START_CHANGE_PERF_LEVEL (32U)
/*! Power Event to finish changing performance level */
#define Power_EV_DONE_CHANGE_PERF_LEVEL  (64U)
/*! Power Event to finish AVS configuration */
#define Power_EV_DONE_AVSCONFIG          (128U)
/*! Power Event to finish DPLL configuration */
#define Power_EV_DONE_DPLLCONFIG         (256U)
/*! Power Event to finish setting module power state */
#define Power_EV_DONE_MODULEPOWERSTATE   (512U)

/*! Total number of power events */
#define Power_NUMEVENTS                  (10U)
/*! Macro for Power Events */
#define Power_EVENT_ALL                  (1023U)

/* Power sleep states */
/*! Power State for sleep */
#define Power_SLEEP                      (0x1)
/*! Power State for deep sleep */
#define Power_DEEPSLEEP                  (0x2)

/* Power shutdown states */
/*! Power State for shutdown */
#define Power_SHUTDOWN                   (0x0)

/*! Additional error code for api failure condition 
 *  Error type defines invalid input */
#define Power_INVALIDINPUT               (0xFFFFFFFFU)

/*!
 *  @brief      Power policy deinitialization function pointer
 */
typedef void (*Power_PolicyDeinitFxn)(void);

/*!
 *  @brief  Power de-initialization function
 *
 *  This function de-initializes Power manager internal state configured in
 *  Power_init.
 *
 *  @return Power_SOK
 */
unsigned int Power_deinit(void);

/*!
 *  @brief  Disable the configured power policy running when the CPU is idle
 *
 *  Calling this function unsets a flag, disabling the configured power
 *  policy function from being invoked on each pass through the Idle loop.
 *
 */
void Power_disablePolicy(void);

/*!
 *  @brief  Get the current temperature range at a temperature sensor
 *
 *  Returns the current temperature range measured at a temperature sensor.
 *  The sensor to read is based on the set point index.
 *
 *  In the case, a temperature sensor is able to return an exact value
 *  the pTempMin and pTempMax dereferenced values will be equivalent.
 *
 *  Temperatures are returned in milli-degree Celsius.
 *
 *  @param  setPointIdx      set point index (device-specific)
 *
 *  @param  pTempMin         pointer to returned min temperature
 *
 *  @param  pTempMax         pointer to returned max temperature
 *
 *  @return Power_SOK
 *
 *  @sa     Power_getCurrentTemp
 */
unsigned int Power_getCurrentTemp(unsigned int  setPointIdx,
                                  int          *pTempMin,
                                  int          *pTempMax);

/*!
 *  @brief  Get a programmed temperature threshold
 *
 *  Returns the programmed threshold temperature range.  The threshold whose
 *  temperature to return is decided based on the set point index.
 *
 *  In the case, a temperature threshold is able to return an exact value
 *  the pTempMin and pTempMax dereferenced values will be equivalent.
 *
 *  Temperatures are returned in milli-degree Celsius.
 *
 *  @param  setPointIdx      set point index (device-specific)
 *
 *  @param  pTempMin         pointer to min set point temperature
 *
 *  @param  pTempMax         pointer to max set point temperature
 *
 *  @return Power_SOK
 *
 *  @sa     Power_getThermalSetPoint
 */
unsigned int Power_getThermalSetPoint(unsigned int  setPointIdx,
                                      int          *pTempMin,
                                      int          *pTempMax);

/*!
 *  @brief  Programs temperature threshold
 *
 *  Configures a temperature threshold.  The threshold temperature to
 *  configure is decided based on the set point index. A registered thermal
 *  event handling function will be invoked when a temperature event is
 *  triggered.  The function will take actions appropriate for the triggered
 *  thermal event (i.e. for high - reducing power consumption in order to lower
 *  the device temperature; for low - returning device operation to normal
 *  after cooldown is achieved from a previous thermal high point).
 *
 *  The temperature is input in milli-degree Celsius.
 *
 *  @param  setPointIdx     set point index (device-specific)
 *
 *  @param  temp            temperature to set
 *
 *  @return Power_SOK
 *
 *  @sa     Power_enableThermalSetPoint
 */
unsigned int Power_enableThermalSetPoint(unsigned int setPointIdx,
                                         int          temp);

/*!
 *  @brief  Disables a previously configured temperature threshold
 *
 *  Disables a previously configured temperature threshold.  The threshold
 *  to disable is decided based on the set point index.
 *
 *  @param  setPointIdx     set point index (device-specific)
 *
 *  @return Power_SOK
 *
 *  @sa     Power_disableThermalSetPoint
 */
unsigned int Power_disableThermalSetPoint(unsigned int setPointIdx);

/*!
 *  @brief  Power function to be added plugged as thermal alert ISR handler
 *
 *  This function should be registered as the handler for the device's thermal
 *  alert mechanism. (The method to do this depends upon the operating system
 *  being used.)  This function will invoke basic thermal event handling logic
 *  and then notifications will be sent to any clients who've registered
 *  (with Power_registerNotify()) to invoke client-specific thermal management
 *  logic.
 *
 */
void Power_ThermalAlertFunc(void);

#ifdef __cplusplus
}
#endif

#endif /* POWEREXTENDED_H */

/* @} */

/**
 *  \defgroup PM_RTOS PMRTOS
 *
 *  PMRTOS provides application interface to power management
 *     - The Power manager facilitates the transition of the CPU from
 *       active state to one of the sleep states and vice versa.
 *     - It provides drivers the ability to set and release dependencies
 *       on hardware resources and keeps a reference count on each resource
 *       to know when to enable or disable the peripheral clock to the
 *       resource.
 *     - It provides drivers the ability to register a callback function
 *       upon a specific power event.
 *     - Drivers and apps can set or release constraints to prevent the CPU
 *       from transitioning into a particular sleep state.
 *  
 *  <b> Refer to @ref Power.h for a complete description of APIs. </b>
 *
 */
