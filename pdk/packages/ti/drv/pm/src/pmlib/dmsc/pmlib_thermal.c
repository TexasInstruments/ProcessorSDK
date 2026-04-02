/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file  pmlib_thermal.c
 *
 *  \brief This file contains the interface definition for the Thermal LIB.
 *         The Thermal LIB supports the following features:
 *         - To return the tempInMilliDegree of a given voltage domain.
 *         - To mask/unmask the hot event for the given sensor.
 *         - To set the band gap threshold hot and cold value for the alert
 *           operation.
 *         - To check if a given voltage domain has the hot/cold alert set.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr_vtm.h>
#include "pm_types.h"
#include "pm_utils.h"
#include <ti/drv/pm/include/dmsc/pmlib_thermal.h>
#include <ti/osal/TimerP.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor Pmlib_thermalAdcRange
 *  \name PM lib thermal ADC range
 *  @{
 *   Valid range of temperature ADC value that can be read by/written
 *      to(as event threshold) the VTM IP
 */
#define PMLIB_VTM_ADC_CODE_MIN_VAL                                        (540U)
#define PMLIB_VTM_ADC_CODE_MAX_VAL                                        (945U)
/* @} */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief This API is used to convert a given 10-bit ADC reference code to
 *        milli degrees.
 *
 * \param  adcCode     ADC Value read which is to be converted.
 *
 * \return tempInMilliDegree    Temperature in milli degrees.
 */
static int32_t PmlibThermalADCCodeToMilliDegree(uint32_t adcCode);

/**
 * \brief This API is used to convert a given temperature in milli degrees to
 *        10-bit ADC reference code
 *
 * \param tempInMilliDegree    Temperature in milli degrees.
 *
 * \return  adcCode     ADC Value after conversion.
 */
static uint32_t PmlibThermalMilliDegreeToADCCode(int32_t tempInMilliDegree);

/**
 * \brief This API is used to check if ADC reference code is within the valid
 *        range
 *
 * \param  adcCode     ADC Value read which is to be converted.
 *
 * \return status      Flag to indicate if the ADC code is valid or not.
 */
static int32_t PmlibThermalIsADCCodeValid(uint32_t adcCode);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t PMLIBThermalTurnOnSensor(uint8_t sensId)
{
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    pmErrCode_t status = PM_SUCCESS;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        /* Enable the clock for the VBGAPS */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_CLKON_REQ,
                1U);
        /* Select Bandgap Voltage as the reference */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_CBIASSEL,
                0U);
        /* Lift the CLRz for the VBGAPS */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_CLRZ,
                1U);
        /* Turn on the temp sensor */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_TMPSOFF,
                0U);
    }
    else
    {
        status = PM_BADARGS;
    }
    return (int32_t) status;
}
int32_t PMLIBThermalTurnOffSensor(uint8_t sensId)
{
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    pmErrCode_t status = PM_SUCCESS;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        /* Enable the clock for the VBGAPS */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_CLKON_REQ,
                0U);
        /* Select Bandgap Voltage as the reference */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_CBIASSEL,
                1U);
        /* Lift the CLRz for the VBGAPS */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_CLRZ,
                0U);
        /* Turn on the temp sensor */
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                VTM_CFG1_TMPSENS_CTRL_TMPSOFF,
                1U);
    }
    else
    {
        status = PM_BADARGS;
    }
    return (int32_t)status;
}

int32_t PMLIBThermalGetCurrTemperature(uint8_t   sensId,
                                       int32_t  *tempInMilliDegree)
{
    pmErrCode_t status = PM_SUCCESS;
    uint32_t adcCode = 0U;
    uint32_t timeout;
    uint32_t tryAgain = 0U;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    uint64_t startTime, endTime;

    if (sensId < PMLIBThermalGetNumSensors() && \
        (tempInMilliDegree != NULL))
    {
        do
        {
            /* Start the conversion */
            CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                    VTM_CFG1_TMPSENS_CTRL_SOC,
                    1U);
            startTime = TimerP_getTimeInUsecs();
            do
            {
                endTime = TimerP_getTimeInUsecs();
            }
            while ((endTime - startTime) < (uint64_t)2U);
            CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                    VTM_CFG1_TMPSENS_CTRL_SOC,
                    0U);
            /* Waiting for ADC EOC */
            timeout = PM_TIMEOUT_INFINITE;
            while (CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].STAT,
                            VTM_CFG1_TMPSENS_STAT_EOCZ) != 0U)
            {
                timeout--;
                if(timeout == 0U)
                {
                    break;
                }
            }
            if(timeout > 0U)
            {
                adcCode = CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].STAT,
                            VTM_CFG1_TMPSENS_STAT_DTEMP);
            }
            else
            {
                status = PM_TIMEOUT;
            }

            if( (PmlibThermalIsADCCodeValid(adcCode) == PM_SUCCESS) &&
                (status == PM_SUCCESS))
            {
                *tempInMilliDegree = PmlibThermalADCCodeToMilliDegree(adcCode);
                tryAgain = 0U;
            }
            else
            {
                if (tryAgain == 1U)
                {
                    status = PM_FAIL;
                    tryAgain = 0U;
                }
                else
                {
                    tryAgain = 1U;
                }
            }
        }while (tryAgain == 1U);
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalEnableOutRangeAlert(uint8_t sensId)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                 VTM_CFG1_TMPSENS_CTRL_MAXT_OUTRG_EN,
                 1U);
        CSL_FINS(vtmCfgRegs->MISC_CTRL,
                 VTM_CFG1_MISC_CTRL_ANY_MAXT_OUTRG_ALERT_EN,
                 1U);
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalDisableOutRangeAlert(uint8_t sensId)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                 VTM_CFG1_TMPSENS_CTRL_MAXT_OUTRG_EN,
                 0U);
        CSL_FINS(vtmCfgRegs->MISC_CTRL,
                 VTM_CFG1_MISC_CTRL_ANY_MAXT_OUTRG_ALERT_EN,
                 0U);
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
int32_t PMLIBThermalSetMaxThermalOutRange(uint32_t maxTempMilliDegree,
                                          uint32_t safeTempMilliDegree)
{
    pmErrCode_t status = PM_SUCCESS;
    uint32_t th1Val = PmlibThermalMilliDegreeToADCCode(maxTempMilliDegree);
    uint32_t th2Val = PmlibThermalMilliDegreeToADCCode(safeTempMilliDegree);
    if ((PmlibThermalIsADCCodeValid(th1Val) == PM_SUCCESS) &&
        (PmlibThermalIsADCCodeValid(th2Val) == PM_SUCCESS))
    {
        /* Needs support from the CSL for this */
        CSL_vtm_cfg1Regs * vtmCfgRegs =
            (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
        CSL_FINS(vtmCfgRegs->MISC_CTRL2,
                 VTM_CFG1_MAXT_OUTRG_ALERT_THR,
                 th1Val);
        CSL_FINS(vtmCfgRegs->MISC_CTRL2,
                 VTM_CFG1_MAXT_OUTRG_ALERT_THR0,
                 th2Val);
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}
#endif

int32_t PMLIBThermalEnableHotEvent(uint8_t sensId)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                 VTM_CFG1_TMPSENS_CTRL_GT_TH1_EN,
                 1U);
        CSL_FINS(vtmCfgRegs->VD[sensId].EVT_SET,
                 VTM_CFG1_EVT_SET_TSENS_EVT_SEL,
                 (1U << sensId));
        CSL_REG_WR(&(vtmCfgRegs->GT_TH1_INT_EN_SET),
                 (1U << sensId));
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalDisableHotEvent(uint8_t sensId)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                 VTM_CFG1_TMPSENS_CTRL_GT_TH1_EN,
                 0U);
        CSL_REG_WR(&(vtmCfgRegs->GT_TH1_INT_EN_CLR),
                 (1U << sensId));
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalEnableColdEvent(uint8_t sensId)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                 VTM_CFG1_TMPSENS_CTRL_LT_TH0_EN,
                 1U);
        CSL_FINS(vtmCfgRegs->VD[sensId].EVT_SET,
                 VTM_CFG1_EVT_SET_TSENS_EVT_SEL,
                 (1U << sensId));
        CSL_REG_WR(&(vtmCfgRegs->LT_TH0_INT_EN_SET),
                 (1U << sensId));
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalDisableColdEvent(uint8_t sensId)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if (sensId < PMLIBThermalGetNumSensors())
    {
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                 VTM_CFG1_TMPSENS_CTRL_LT_TH0_EN,
                 0U);
        CSL_REG_WR(&(vtmCfgRegs->LT_TH0_INT_EN_CLR),
                 (1U << sensId));
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalSetHotThreshold(uint8_t sensId,
                                 int32_t tempInMilliDegree)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    uint32_t th1Val = PmlibThermalMilliDegreeToADCCode(tempInMilliDegree);
    if ((sensId < PMLIBThermalGetNumSensors()) &&
        (PmlibThermalIsADCCodeValid(th1Val) == PM_SUCCESS))
    {
        CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                 VTM_CFG1_TMPSENS_CTRL_TH1_VAL,
                 th1Val);
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalSetColdThreshold(uint8_t sensId,
                                     int32_t tempInMilliDegree)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    uint32_t th0Val = PmlibThermalMilliDegreeToADCCode(tempInMilliDegree);
    if ((sensId < PMLIBThermalGetNumSensors()) &&
       (PmlibThermalIsADCCodeValid(th0Val) == PM_SUCCESS))
    {
        /* Check first if the TH1 is already set and then program only the
         * difference */
        uint32_t th1Val = CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].CTRL,
                          VTM_CFG1_TMPSENS_CTRL_TH1_VAL);
        uint32_t th0DecVal = ((th1Val - th0Val)/2U) + 1U;
        if (th0DecVal <= CSL_VTM_CFG1_TMPSENS_CTRL_TH0_DEC_VAL_MAX)
        {
            CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                     VTM_CFG1_TMPSENS_CTRL_TH0_DEC_VAL,
                     th0DecVal);
        }
        else
        {
            /* This will change the TH1 value. This needs to be done
             * carefully and the API assumes that the user of this API is aware
             * of the changes to the TH1 value if already set.
             */
            th0DecVal = CSL_VTM_CFG1_TMPSENS_CTRL_TH0_DEC_VAL_MAX;
            th1Val = (th0DecVal - 1U)*2U + th0Val;
            if (PmlibThermalIsADCCodeValid(th1Val) == PM_SUCCESS)
            {
                CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                     VTM_CFG1_TMPSENS_CTRL_TH1_VAL,
                     th1Val);
                CSL_FINS(vtmCfgRegs->TMPSENS[sensId].CTRL,
                     VTM_CFG1_TMPSENS_CTRL_TH0_DEC_VAL,
                     th0DecVal);
            }
            else
            {
                status = PM_FAIL;
            }
        }
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalGetHotThreshold(uint8_t sensId,
                                    int32_t *tempInMilliDegree)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if ((sensId < PMLIBThermalGetNumSensors()) && \
        (tempInMilliDegree != NULL))
    {
        int32_t hotThresholdAdcCode =
            CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].CTRL,
                          VTM_CFG1_TMPSENS_CTRL_TH1_VAL);
        if(PmlibThermalIsADCCodeValid(hotThresholdAdcCode) == PM_SUCCESS)
        {
            *tempInMilliDegree = PmlibThermalADCCodeToMilliDegree(
                                hotThresholdAdcCode);
        }
        else
        {
            status = PM_FAIL;
        }
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalGetColdThreshold(uint8_t sensId,
                                     int32_t *tempInMilliDegree)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    if ((sensId < PMLIBThermalGetNumSensors()) && \
        (tempInMilliDegree != NULL))
    {
        int32_t coldThresholdAdcCode =
            CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].CTRL,
                          VTM_CFG1_TMPSENS_CTRL_TH1_VAL) -
                (2 * (CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].CTRL,
                          VTM_CFG1_TMPSENS_CTRL_TH0_DEC_VAL) - 1));
        if(PmlibThermalIsADCCodeValid(coldThresholdAdcCode) == PM_SUCCESS)
        {
            *tempInMilliDegree = PmlibThermalADCCodeToMilliDegree(
                                        coldThresholdAdcCode);
        }
        else
        {
            status = PM_FAIL;
        }
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalGetHotAlertStatus(uint8_t sensId, uint32_t *hotStatus)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    int32_t currTempAdcCode = 0;
    int32_t hotThresholdAdcCode = 0;

    if (sensId < PMLIBThermalGetNumSensors() && \
        (hotStatus != NULL))
    {
        *hotStatus =
            CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].STAT,
            VTM_CFG1_TMPSENS_STAT_GT_TH1_ALERT);

        /* Once the hot event is confirmed. Reconfirm by comparing the current
         * temperature to the Hot Threshold programmed. This makes sure that
         * no spurious hot events are reported.
         */
        currTempAdcCode = CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].STAT,
                          VTM_CFG1_TMPSENS_STAT_DTEMP);
        hotThresholdAdcCode = CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].CTRL,
                          VTM_CFG1_TMPSENS_CTRL_TH1_VAL);
        /* Case : Hot event is false positive */
        if ((hotThresholdAdcCode > currTempAdcCode) && (*hotStatus == 1U))
        {
            *hotStatus = 0U;
        }
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

int32_t PMLIBThermalGetColdAlertStatus(uint8_t sensId, uint32_t *coldStatus)
{
    pmErrCode_t status = PM_SUCCESS;
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *) CSL_WKUP_VTM0_CFG1_BASE;
    int32_t currTempAdcCode = 0;
    int32_t coldThresholdAdcCode = 0;

    if ((sensId < PMLIBThermalGetNumSensors()) && \
        (coldStatus != NULL))
    {
        *coldStatus =
            CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].STAT,
            VTM_CFG1_TMPSENS_STAT_LT_TH0_ALERT);
        /* Once the cold event is confirmed. Reconfirm by comparing the current
         * temperature to the cold Threshold programmed. This makes sure that
         * no spurious cold events are reported.
         */
        currTempAdcCode = CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].STAT,
                          VTM_CFG1_TMPSENS_STAT_DTEMP);
        coldThresholdAdcCode =
            CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].CTRL,
                          VTM_CFG1_TMPSENS_CTRL_TH1_VAL) -
                (2 * (CSL_FEXT(vtmCfgRegs->TMPSENS[sensId].CTRL,
                          VTM_CFG1_TMPSENS_CTRL_TH0_DEC_VAL) - 1));
        /* Case : Cold event is false positive */
        if ((coldThresholdAdcCode < currTempAdcCode) && (*coldStatus == 1U))
        {
            *coldStatus = 0U;
        }
    }
    else
    {
        status = PM_BADARGS;
    }
    return status;
}

uint8_t PMLIBThermalGetNumSensors(void)
{
    CSL_vtm_cfg1Regs * vtmCfgRegs =
        (CSL_vtm_cfg1Regs *)CSL_WKUP_VTM0_CFG1_BASE;
    uint8_t numSens = CSL_FEXT(vtmCfgRegs->DEVINFO_PWR0,
                                     VTM_CFG1_DEVINFO_PWR0_TMPSENS_CT);
    return numSens;
}


/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

static int32_t PmlibThermalADCCodeToMilliDegree(uint32_t adcCode)
{
    /* Formula from the Band gap specification.
     *
     * -40 to 0.2C    =(0.42240*(Code-544.177))-39.8
     * 0.6 to 40.2C   =(0.41820*(Code-639.875))+0.6
     * 40.6 to 80.2C  =(0.41196*(Code-735.721))+40.6
     * 80.6 to 125C   =(0.40470*(Code-833.000))+80.6
     */
    Float32 tempInMilliDegree = 0.0;
    if ((Float32)adcCode > 833)
    {
        tempInMilliDegree = (0.40470 * ((Float32)adcCode - 833.0)) + 80.6;
    }
    else if ((Float32)adcCode > 735.721)
    {
        tempInMilliDegree = (0.41196 * ((Float32)adcCode - 735.721)) + 40.6;
    }
    else if ((Float32)adcCode > 639.875)
    {
        tempInMilliDegree = (0.41820 * ((Float32)adcCode - 639.875)) + 0.6;
    }
    else
    {
        tempInMilliDegree = (0.42240 * ((Float32)adcCode - 544.177)) - 39.8;
    }
    return (int32_t) (tempInMilliDegree * 1000.0);
}

static uint32_t PmlibThermalMilliDegreeToADCCode(int32_t tempInMilliDegree)
{
    int32_t adcValue;

    /* Formula from the Band gap specification. Calculation using temperature
     * ranges is better in this case as the exact temperature search in the
     * LUT may not match, but a best case ADC value can be derived using the
     * formula.
     *
     * -40 to 0.2C    =(0.42240*(Code-544.177))-39.8
     * 0.6 to 40.2C   =(0.41820*(Code-639.875))+0.6
     * 40.6 to 80.2C  =(0.41196*(Code-735.721))+40.6
     * 80.6 to 125C   =(0.40470*(Code-833.000))+80.6
     */
    if ((tempInMilliDegree >= -40000) && (tempInMilliDegree < 600))
    {
        adcValue =
            (((tempInMilliDegree + 39800) * 100) / 42240) + (544177 / 1000);
    }
    else if ((tempInMilliDegree >= 600) && (tempInMilliDegree < 40600))
    {
        adcValue =
            (((tempInMilliDegree - 600) * 100) / 41820) + (639875 / 1000);
    }
    else if ((tempInMilliDegree >= 40600) && (tempInMilliDegree < 80600))
    {
        adcValue =
            (((tempInMilliDegree - 40600) * 100) / 41196) + (735721 / 1000);
    }
    else if ((tempInMilliDegree >= 80600) && (tempInMilliDegree <= 125000))
    {
        adcValue = (((tempInMilliDegree - 80600) * 100) / 40470) + 833;
    }
    else
    {
        adcValue = 0;
    }

    return (uint32_t) adcValue;
}

static int32_t PmlibThermalIsADCCodeValid(uint32_t adcCode)
{
    pmErrCode_t status = PM_BADARGS;
    if((adcCode > PMLIB_VTM_ADC_CODE_MIN_VAL) && \
        (adcCode < PMLIB_VTM_ADC_CODE_MAX_VAL))
    {
        status = PM_SUCCESS;
    }
    return status;
}
