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
 * \file   V3/gpio.c
 *
 * \brief  GPIO Device Abstraction Layer APIs
 *
 *         This file contains the prototypes of the APIs present in the
 *         device abstraction layer file of GPIO.
 *         This also contains some related macros.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/** \brief This is to disable HW_SYNC_BARRIER for register access */
#define MEM_BARRIER_DISABLE

#include <stdint.h>
#include <ti/csl/csl_gpio.h>

/* ========================================================================== */
/*                          Function Definitions                             */
/* ========================================================================== */
void GPIO_moduleEnable(uint32_t baseAdd)
{
    /* Clearing the DISABLEMODULE bit in the Control(CTRL) register. */
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOGCR, CSL_GPIO_GIOGCR_RESET_MAX);
}

void GPIO_ignorePolarity (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIOINTDET);
    regValue |= 1U << ((port * 8U) + pin);
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOINTDET, regValue);
}


void GPIO_honorPolarity (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIOINTDET);
    regValue &= ~(1U << ((port * 8U) + pin));
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOINTDET, regValue);
}

void GPIO_configPolarity (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t polLevel)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIOPOL);
    regValue &= ~(1U << ((port * 8U) + pin));
    regValue |= (uint32_t) polLevel << ((port * 8U) + pin);
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOPOL, regValue);
}

void GPIO_enableInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = (uint32_t) 1 << ((port * 8U) + pin);
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOENASET, regValue);
}

void GPIO_disableInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = (uint32_t) 1 << ((port * 8U) + pin);
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOENACLR, regValue);
}

void GPIO_disableMultipleInterrupt (uint32_t baseAdd, uint32_t disableFlag)
{
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOENACLR, disableFlag);
}

void GPIO_clearInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIOFLG);
    regValue |= (uint32_t) 1 << ((port * 8U) + pin);
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOFLG, regValue);
}

void GPIO_markHighLevelInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIOLVLSET);
    regValue |= (uint32_t) 1 << ((port * 8U) + pin);
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOLVLSET, regValue);
}

void GPIO_markLowLevelInterrupt (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIOLVLCLR);
    regValue |= (uint32_t) 1 << ((port * 8U) + pin);
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOLVLCLR, regValue);
}

uint32_t GPIO_getHighLevelPendingInterrupt(uint32_t baseAdd)
{
    return (CSL_REG32_RD(baseAdd + CSL_GPIO_GIOOFFA));
}

uint32_t GPIO_getLowLevelPendingInterrupt(uint32_t baseAdd)
{
    return (CSL_REG32_RD(baseAdd + CSL_GPIO_GIOOFFB));
}

void GPIO_setOutputDataDirection (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t dataDirection)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIODIR(port));
    regValue &= ~(1U << pin);
    regValue |= (uint32_t) dataDirection << pin;
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIODIR(port), regValue);
}

void GPIO_setOutputDataDirectionMultiple (uint32_t baseAdd, uint32_t port, uint32_t dataDirectionFlag)
{
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIODIR(port), dataDirectionFlag);
}

uint32_t GPIO_getData (uint32_t baseAdd, uint32_t port, uint32_t pin)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIODIN(port));
    regValue &= 1U << pin;
    return (regValue >> pin);
}

void GPIO_sendData (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t value)
{
    if (value == 1U)
    {
        CSL_REG32_WR(baseAdd + CSL_GPIO_GIOSET(port), (1U << pin));
    }
    else
    {
        CSL_REG32_WR(baseAdd + CSL_GPIO_GIOCLR(port), (1U << pin));
    }
}

void GPIO_sendDataMultiple (uint32_t baseAdd, uint32_t port, uint32_t outValueFlag)
{
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIODOUT(port), outValueFlag);
}

void GPIO_setOpenDrainStatus (uint32_t baseAdd, uint32_t port, uint32_t pin, uint32_t openDrainStatus)
{
    uint32_t regValue = CSL_REG32_RD(baseAdd + CSL_GPIO_GIOPDR(port));
    regValue &= ~(1U << pin);
    regValue |= (uint32_t) openDrainStatus << pin;
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOPDR(port), regValue);
}

void GPIO_setOpenDrainStatusMultiple (uint32_t baseAdd, uint32_t port, uint32_t openDrainStatusFlag)
{
    CSL_REG32_WR(baseAdd + CSL_GPIO_GIOPDR(port), openDrainStatusFlag);
}



/******************************* End Of File ********************************/
