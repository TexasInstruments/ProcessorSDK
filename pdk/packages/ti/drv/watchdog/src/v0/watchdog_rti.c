/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/csl/hw_types.h>
#include <ti/osal/osal.h>

#include "watchdog_rti.h"

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/* Registered callback functions: */
static void         WatchdogRTI_clear(Watchdog_Handle handle);
static void         WatchdogRTI_close(Watchdog_Handle handle);
static int32_t      WatchdogRTI_control(Watchdog_Handle handle, uint32_t cmd, void *arg);
static void         WatchdogRTI_init(Watchdog_Handle handle);
static Watchdog_Handle   WatchdogRTI_open(Watchdog_Handle handle, Watchdog_Params *params);
/* Watchdog Driver API: */
static void         WatchdogRTI_callback (void* arg);

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   Watchdog Driver Function Table
 */
Watchdog_FxnTable gWatchdogFxnTable =
{
    &WatchdogRTI_clear,
    &WatchdogRTI_close,
    &WatchdogRTI_control,
    &WatchdogRTI_init,
    &WatchdogRTI_open
};

/**************************************************************************
 ************************* Watchdog  Driver Functions *********************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      The function is the registered ESM callback function for the Watchdog Driver.
 *      This function is used only when the reset mode is set to NMI interrupt mode.
 *
 *  @param[in]  arg
 *      Argument which is registered with the ESM module while registering the notify function.
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */

static void WatchdogRTI_callback (void* arg)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_MCB*           ptrWatchdogMCB;
    Watchdog_HwAttrs*       ptrHwCfg;

    /* Get the Watchdog Configuration */
    ptrWatchdogConfig = (Watchdog_Config*)arg;

    /* Get the Watchdog Driver Object */
    ptrWatchdogMCB = (Watchdog_MCB*)ptrWatchdogConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (Watchdog_HwAttrs*)ptrWatchdogConfig->hwAttrs;

    /* Increment the ISR counter */
    ptrWatchdogMCB->interruptsRxed++;

    /* Clear the status flags */
    RTIDwwdClearStatus(ptrHwCfg->baseAddr, WATCHDOG_CLEAR_STATUS);

    /* Call the registered callback. */
    if (ptrWatchdogMCB->params.callbackFxn != NULL)
    {
        ptrWatchdogMCB->params.callbackFxn((Watchdog_Handle)arg);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is clears the watchdog to prevent a reset signal or
 *      interrupt from being generated. Its the application's responsibilty
 *      to clear the watch dog timer.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void WatchdogRTI_clear(Watchdog_Handle handle)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_HwAttrs*       ptrHwCfg;
    Watchdog_MCB*           ptrWatchdogMCB;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if (handle != NULL)
    {
        /* Get the Watchdog Configuration: */
        ptrWatchdogConfig = (Watchdog_Config*)handle;

        /* Get the Watchdog Driver Object */
        ptrWatchdogMCB = (Watchdog_MCB*)ptrWatchdogConfig->object;

        ptrWatchdogMCB->watchdogCleared++;

        /* Get the hardware configuration: */
        ptrHwCfg = (Watchdog_HwAttrs*)ptrWatchdogConfig->hwAttrs;

        RTIDwwdService(ptrHwCfg->baseAddr);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog Driver instance is being closed
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void WatchdogRTI_close(Watchdog_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog Driver is being passed a control command.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *  @param[in]  cmd
 *      Watchdog command
 *  @param[in]  arg
 *      Opaque handle to the argument
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Watchdog Error code
 */
static int32_t WatchdogRTI_control(Watchdog_Handle handle, uint32_t cmd, void *arg)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_HwAttrs*       ptrHwCfg;
    Watchdog_MCB*           ptrWatchdogMCB;
    int32_t                 errCode;

    /* Initialize and setup the error code: */
    errCode = 0;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (arg == NULL))
    {
        errCode = WATCHDOG_EINVAL;
    }
    else
    {
        /* Get the Watchdog Configuration: */
        ptrWatchdogConfig = (Watchdog_Config*)handle;

        /* Get the Watchdog Driver Object */
        ptrWatchdogMCB = (Watchdog_MCB*)ptrWatchdogConfig->object;

        /* Get the hardware configuration: */
        ptrHwCfg = (Watchdog_HwAttrs*)ptrWatchdogConfig->hwAttrs;

        /* Processing is done on the basis of the command: */
        switch (cmd)
        {
            case WATCHDOG_CMD_RESETMODE:
            {
                int32_t*   resetMode;

                /* Get the reset mode from the argument */
                resetMode = (int32_t*)arg;

                /* Only NMI mode is supported in the DSS subsystem.
                 * Resetting the system is supported only in the MSS sysbsystem. */
#if defined (_TMS320C6X)
                 if ((Watchdog_ResetMode)*resetMode == Watchdog_RESET_ON)
                 {
                     errCode = WATCHDOG_ENOTIMPL;
                 }
#else
                /* Check if we need to change the reset mode */
                if (*resetMode != ptrWatchdogMCB->params.resetMode)
                {
                    /* Configure the reset mode */
                    RTIDwwdSetReaction(ptrHwCfg->baseAddr, *resetMode);

                    /* Configure the reset mode */
                    ptrWatchdogMCB->params.resetMode = (Watchdog_ResetMode)*resetMode;
                }
#endif
                break;
            }
            case WATCHDOG_CMD_WINDOWSIZE:
            {
                int32_t*    windowSize;

                /* Get the window size from the argument */
                windowSize = (int32_t*)arg;

                /* Check if we need to change the window size */
                if (*windowSize != ptrWatchdogMCB->params.windowSize)
                {
                    /* Configure the window size */
                    RTIDwwdSetWindowSize(ptrHwCfg->baseAddr, *windowSize);

                    /* Configure the window size */
                    ptrWatchdogMCB->params.windowSize = (Watchdog_WindowSize)*windowSize;
                }
                break;
            }
            default:
            {
                /* Error: Unsuported/Invalid command specified */
                errCode = WATCHDOG_EINVAL;
                break;
            }
        }
    }
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog driver is initialized.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver which is to be initialized.
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void WatchdogRTI_init(Watchdog_Handle handle)
{
    Watchdog_socInit(handle);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *  @param[in]  params
 *      Watchdog Parameters with which the driver is being opened
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
static Watchdog_Handle WatchdogRTI_open(Watchdog_Handle handle, Watchdog_Params* params)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_MCB*           ptrWatchdogMCB;
    Watchdog_HwAttrs*       ptrHwCfg;
    Watchdog_Handle         retHandle = NULL;
    ESM_NotifyParams        notifyParams;
    int32_t                 errCode, retVal = 0;
    uint32_t                preloadMaxValue;
    uint64_t                preloadValueTmp;

    /* Only NMI mode is supported in the DSS subsystem.
     * Resetting the system is supported only in the MSS sysbsystem. */
#if defined (_TMS320C6X)
    if (params->resetMode == Watchdog_RESET_ON)
    {
        retVal = -1;
    }
#endif

    /* Get the Watchdog Configuration: */
    ptrWatchdogConfig = (Watchdog_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (Watchdog_HwAttrs*)ptrWatchdogConfig->hwAttrs;

    /* Get the Watchdog Driver Object */
    ptrWatchdogMCB = (Watchdog_MCB*)ptrWatchdogConfig->object;

    /* Initialize the memory: */
    memset ((void *)ptrWatchdogMCB, 0, sizeof(Watchdog_MCB));

    /* Calculate preload value from the expiration time */
    /* Expiration time (in millisecond)/1000 = (preload value + 1)*(2^13)/RTICLK */
    preloadValueTmp =((uint64_t)(ptrHwCfg->rtiClkFrequency)) * ((uint64_t)params->expirationTime) / 1000;
    /* Pre load is 12 bit value in register and actual value should be multiplied by 2^13) */
    preloadMaxValue = (1 << (12 + 13)) - 1;
    if (preloadValueTmp > preloadMaxValue)
    {
        retVal = -1;
    }

    if (retVal >= 0)
    {
        /* Configure the Watchdog driver. */
        /* Copy over the Watchdog Parameters */
        memcpy ((void*)&ptrWatchdogMCB->params, (void *)params, sizeof(Watchdog_Params));

        /* Bring watchdog out of reset */
        RTI_socEnableWatchdog();

        /* Register to get a callback from the ESM module if NMI interrupt mode is configured */
        if (ptrWatchdogMCB->params.resetMode == Watchdog_RESET_OFF)
        {
            notifyParams.groupNumber = ptrHwCfg->groupNum;
            notifyParams.errorNumber = ptrHwCfg->errorNum;
            notifyParams.setIntrPriorityLvlHigh = 0;
            notifyParams.enableInfluenceOnErrPin = 0;
            notifyParams.arg = (void *)ptrWatchdogConfig;
            notifyParams.notify = WatchdogRTI_callback;

            retVal = ESM_registerNotifier (ptrHwCfg->esmHandle, &notifyParams, &errCode);
        }
        else
        {
            /* Configure the SOC moule to trigger a warm reset upon watchdog reset */
            RTI_socTriggerWatchdogWarmReset();
        }
    }

    if (retVal >= 0 )
    {
        uint32_t                preloadValue;
        /* CSL API needs shifted value but doesnot do -1 while programming.
         * So updating the preload value.
         */
        preloadValue = ((uint32_t)(preloadValueTmp) >> 13) - 1;
        preloadValue = preloadValue << 13;

        /* Clear the status flags */
        RTIDwwdClearStatus(ptrHwCfg->baseAddr, WATCHDOG_CLEAR_STATUS);

        RTIDwwdWindowConfig(ptrHwCfg->baseAddr,
                            ptrWatchdogMCB->params.resetMode,    /* Configure the reset mode    */
                            preloadValue,                        /* Configure the preload value */
                            ptrWatchdogMCB->params.windowSize);  /* Configure the window size   */

        /* Configure the stall mode */
        RTIGcSetStallMode(ptrHwCfg->baseAddr, ptrWatchdogMCB->params.debugStallMode);

        /* Enable the watchdog timer */
        RTIDwwdCounterEnable(ptrHwCfg->baseAddr);

        /* Mark the driver to be operational */
        ptrWatchdogMCB->state = Watchdog_DriverState_OPERATIONAL;

        /* Setup the return handle: */
        retHandle = handle;
    }

    return retHandle;
}

