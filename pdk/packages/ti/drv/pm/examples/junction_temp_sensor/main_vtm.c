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
 *  \file main_vtm.c
 *
 *  \brief VTM Temperature Monitoring Application
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/csl_types.h>
#include <ti/drv/pm/include/pm_utils.h>
#include <ti/drv/pm/include/pm_types.h>
#include <ti/drv/pm/include/dmsc/pmlib_thermal.h>
#include "app_utils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief HOT temperature event triggered at 20 degrees Celcius
 *          Triggering this at a lower temperature so it maybe tested without
 *          waiting for a the soc to heat up.
 */
#define HOT_EVT_TEMP_THRESH              (20000)

/** \brief COLD temperature event triggered at 80 degrees Celcius.
 *          Triggering this at a higher temperature so it maybe tested without
 *          decreasing the temperature manually.
 */
#define COLD_EVT_TEMP_THRESH             (80000)

/** \brief This is the difference between high and low threshold events
 *          converted in milli degrees.
 */
#define TH0_DEC_VAL                      (10000)

/** \brief HOT temperature event triggered at 125 degrees Celcius */
#define MAX_HOT_EVT_TEMP_THRESH          (125000)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static int32_t gPassFailStatus = PM_SUCCESS;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    int32_t          retVal        = PM_SUCCESS;
    int32_t          currTemp      = 0;
    uint8_t          numSens       = 0;
    uint32_t         i             = 0U;
    int32_t          hotThreshold  = 0;
    int32_t          coldThreshold = 0;
    uint32_t         hotStatus = 0U;
    uint32_t         coldStatus = 0U;
    gPassFailStatus = PM_SUCCESS;
    AppUtils_defaultInit();

    App_printf("\r\nPM Junction Temperature Measure Test Application");
    
    /* Enable all thermal sensors */
    numSens = PMLIBThermalGetNumSensors();
    for (i=0;i < numSens;i++)
    {
        PMLIBThermalTurnOnSensor(i);
    }
    
    App_printf("\r\n----------------------------------------");
    App_printf("\r\n Measure the Current Temperature from different sensors");
    App_printf("\r\n----------------------------------------");

    /* Get the temperature of all sensors .*/
    for (i=0;i < numSens;i++)
    {
        retVal = PMLIBThermalEnableOutRangeAlert(i);
        if (PM_SUCCESS == retVal)
        {
            App_printf("\r\nEnable out of range Alert for sensId = %u",(uint32_t)i);
        }
        else
        {
            App_printf("\r\nEnable out of range Alert for sensId = %u Failed",(uint32_t)i);
            gPassFailStatus = PM_FAIL;
        }
        retVal = PMLIBThermalGetCurrTemperature(i, &currTemp);
        if (PM_SUCCESS == retVal)
        {
            App_printf("\r\nCurrent Temperature for sensId = %u in milli Degrees C = %d ",
                (uint32_t)i,currTemp );
        }
        else
        {
            App_printf("\r\nGet Current Temperature Failed!!\n");
            gPassFailStatus = PM_FAIL;
        }
    }
    App_printf("\r\n----------------------------------------");
    App_printf("\r\n Testing Cold Event");
    App_printf("\r\n----------------------------------------");
    /* Set hot and cold thresholds for  triggering low temperature event for all sensors. */
    for (i=0;i < numSens;i++)
    {
        retVal = PMLIBThermalSetColdThreshold(i, COLD_EVT_TEMP_THRESH);
        if (PM_SUCCESS == retVal)
        {
            retVal = PMLIBThermalGetColdThreshold(i,&coldThreshold);
            App_printf(
                "\r\nCold threshold for sensId = %u is set at milli Degrees C = %d\n",
                (uint32_t)i,coldThreshold );
        }
        else
        {
            App_printf("\r\nSet cold threshold Failed!!\n");
            gPassFailStatus = PM_FAIL;
        }
    }

    /* Enable cold event for all sensors. */
    for (i=0;i < numSens;i++)
    {
        PMLIBThermalEnableColdEvent(i);
    }

    App_printf("\r\n----------------------------------------");
    App_printf("\r\n Check for Cold events of different sensors");
    App_printf("\r\n----------------------------------------");

    for (i=0;i < numSens;i++)
    {
        PMLIBThermalGetColdAlertStatus(i,&coldStatus);
        if(coldStatus == 1U)
        {
            PMLIBThermalGetCurrTemperature(i, &currTemp);
            App_printf("\r\nCold event occurred for sensId = %u and current temperature in milli Degrees C = %d",(uint32_t)i,currTemp);
            /* Disable cold event for this sensor. */
            PMLIBThermalDisableColdEvent(i);
        }
        else
        {
            App_printf("\r\nCold event failed for sensId = %u and current temperature in milli Degrees C = %d",(uint32_t)i,currTemp);
            gPassFailStatus = PM_FAIL;
        }
    }

    App_printf("\r\n----------------------------------------");
    App_printf("\r\n Testing Hot Event");
    App_printf("\r\n----------------------------------------");
    /* Set hot and cold thresholds for  triggering hot temperature event for all sensors. */
    for (i=0;i < numSens;i++)
    {
        retVal = PMLIBThermalSetHotThreshold(i, HOT_EVT_TEMP_THRESH);
        if (PM_SUCCESS == retVal)
        {
            retVal = PMLIBThermalGetHotThreshold(i,&hotThreshold);
            App_printf(
                "\r\n\nHot threshold for sensId = %u is set at milli Degrees C = %d",
                (uint32_t)i,hotThreshold );
        }
        else
        {
            App_printf("\r\nSet hot threshold Failed!!");
        }
    }

    /* Enable Hot event for all sensors. */
    for (i=0;i < numSens;i++)
    {
        PMLIBThermalEnableHotEvent(i);
    }

    App_printf("\r\n----------------------------------------");
    App_printf("\r\n Check for Hot events of different sensors");
    App_printf("\r\n----------------------------------------");

    for (i=0;i < numSens;i++)
    {
        PMLIBThermalGetHotAlertStatus(i,&hotStatus);
        if(hotStatus == 1U)
        {
            PMLIBThermalGetCurrTemperature(i, &currTemp);
            App_printf("\r\nHot event occurred for sensId = %u and current temperature in milli Degrees C = %d",(uint32_t)i,currTemp);
            /* Disable hot event for this sensor. */
            PMLIBThermalDisableHotEvent(i);
        }
        else
        {
            App_printf("\r\nHot event failed for sensId = %u and current temperature in milli Degrees C = %d",(uint32_t)i,currTemp);
            gPassFailStatus = PM_FAIL;
        }
    }
    for (i=0;i < numSens;i++)
    {
        PMLIBThermalTurnOffSensor(i);
    }
    if (gPassFailStatus == PM_SUCCESS)
    {
        App_printf("\r\nAll tests have passed.");
    }
    else
    {
        App_printf("\r\nSome tests have failed");
    }
    return 0;
}
