/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \ingroup CSL_IP_MODULE
 *  \defgroup CSL_GPIO GPIO
 *
 *  @{
 */
 /**
 * \file   V3/gpio_v3.h
 *
 * \brief  GPIO Device Abstraction Layer APIs
 *
 *         This file contains the prototypes of the APIs present in the
 *         device abstraction layer file of GPIO.
 *         This also contains some related macros.
 *
 **/

#ifndef  V3_GPIO_H_
#define  V3_GPIO_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief This API performs the module enable of the GPIO module.
 *
 * \param  baseAdd   GPIO base address
 *
 * \return None
 */
void GPIO_moduleEnable(uint32_t baseAdd);

/**
 * \brief The function is used to configure the GPIO Port/Pin to ignore the
 *      polarity.
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \return None
 */
void GPIO_ignorePolarity (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to configure the GPIO Port/Pin to honor the
 *      polarity.
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \return None
 */
void GPIO_honorPolarity (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to configure the GPIO Port/Pin to control
 *      the interrupt polarity
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 * \param  polLevel  polarity of the interrupt to be triggered
 *                      1 - the interrupt be triggered on rising edge
 *                      0 - the interrupt be triggered on falling edge
 *
 * \return None
 */
void GPIO_configPolarity (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t polLevel);

/**
 * \brief The function is used to enable the interrupt on the GPIO Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \return None
 */
void GPIO_enableInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to disable the interrupt on the GPIO Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \return None
 */
void GPIO_disableInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to disable multiple interrupts of GPIO
 *
 * \param  baseAdd      GPIO base address
 * \param  disableFlag  Mask of the interrupt disable reg value
 *
 * \return None
 */
void GPIO_disableMultipleInterrupt (uint32_t baseAdd, uint32_t disableFlag);
/**
 * \brief The function is used to clear the interrupt on the GPIO Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \return None
 */
void GPIO_clearInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to mark the interrupt as high level for the
 *      specific GPIO Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \return None
 */
void GPIO_markHighLevelInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to mark the interrupt as low level for the
 *      specific GPIO Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \return None
 */
void GPIO_markLowLevelInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to get the high level interrupt pending.
 *      The driver will need to cycle through all the pending interrupts
 *      before the processing of the interrupt is over
 *
 * \retval          High level pending interrupr number
 *                      0           - No pending interrupt
 *                      Non Zero    - Highest Pending Interrupt
 */
uint32_t GPIO_getHighLevelPendingInterrupt(uint32_t baseAdd);

/**
 * \brief The function is used to get the low level interrupt pending.
 *      The driver will need to cycle through all the pending interrupts
 *      before the processing of the interrupt is over
 *
 * \retval          Low level pending interrupr number
 *                      0           - No pending interrupt
 *                      Non Zero    - Highest Pending Interrupt
 */
uint32_t GPIO_getLowLevelPendingInterrupt(uint32_t baseAdd);

/**
 * \brief The function is used to set the output data direction
 *      associated with a GPIO Port/Pin.
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 * \param  dataDirection    Data Direction set
 *                              1 to enable the output data direction
 *                              0 to disable the output data direction
 *
 * \return None
 */
void GPIO_setOutputDataDirection (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t dataDirection);

/**
 * \brief The function is used to set the output data direction
 *      associated multiple pins of a GPIO Port.
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  dataDirectionFlag    data direction reg Value associated with multiple pins of the port
 *                                  1 to enable the output data direction of corresponding pin
 *                                  0 to disable the output data direction of corresponding pin
 *
 * \return None
 */
 void GPIO_setOutputDataDirectionMultiple (uint32_t baseAdd, uint32_t port, uint32_t dataDirectionFlag);

/**
 * \brief The function is used to get the data input for a specific GPIO
 *      Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 *
 * \retval           Logic level on the pin
 *                       0  - Logic Low
 *                       1  - Logic High
 */
uint32_t GPIO_getData (uint32_t baseAdd, uint32_t port, uint32_t pin);

/**
 * \brief The function is used to send the data to a specific GPIO Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 * \param  value     Bit Value to write
 *
 * \return None
 */
void GPIO_sendData (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t value);

/**
 * \brief The function is used to send the data to multiple pins of a specific GPIO Port
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  outValueFlag out Value to write for multiple pins of the port.
 *
 * \return None
 */
void GPIO_sendDataMultiple (uint32_t baseAdd, uint32_t port, uint32_t outValueFlag);

/**
 * \brief The function is used to enable/disable open drain functionality
 *      for a specific GPIO Port/Pin
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  pin       GPIO Pin number associated with the port
 * \param  openDrainStatus  Set open drain status
 *                              1 to enable open drain functionality
 *                              0 to disable open drain functionality
 *
 * \return None
 */
void GPIO_setOpenDrainStatus (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t openDrainStatus);

/**
 * \brief The function is used to enable/disable open drain functionality
 *      for a multiple pins of a GPIO Port
 *
 * \param  baseAdd   GPIO base address
 * \param  port      GPIO Port Number to be configured
 * \param  openDrainStatusFlag  Set open drain status flag correponding to multiple pins of the port.
 *                              1 to enable open drain functionality for correponding pin
 *                              0 to disable open drain functionality  for correponding pin
 *
 * \return None
 */
void GPIO_setOpenDrainStatusMultiple (uint32_t baseAdd, uint32_t port, uint32_t openDrainStatusFlag);

#ifdef __cplusplus
}
#endif

#endif
 /** @} */
/********************************* End Of File ******************************/
