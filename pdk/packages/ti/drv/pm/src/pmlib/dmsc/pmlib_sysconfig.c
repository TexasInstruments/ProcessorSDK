/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2014
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
 * \file    pmlib_sysconfig.c
 *
 * \brief   This file defines the function used for
 *          switching on or switching off modules or putting them in HW
 *          controlled Auto Clock Gated mode.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/sciclient/sciclient.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief This API is used to disable a given module.
 *
 * \param  modIdx       Unique ID of the module to be disabled.
 * \param  result      Structure to return to user the cause of failure.
 * \param  timeout     Time for which to wait for the module to be disabled.
 *                     PM_TIMEOUT_NOWAIT (0) - The API does not wait.
 *                     PM_TIMEOUT_INFINITE (0xFFFFFFFF) - The API waits
 *                     infinitely.
 *                     Any other value - The API waits to any one of the
 *                     events to happen first : (1) Success of operation
 *                     (2) The timeout is reached.
 *
 * \return errorStatus Status of the API call. Can be any of the following,
 *         PM_SUCCESS  Indicates the operation is success
 *         PM_FAIL     Indicates the module is not disabled (timeout)
 */
static int32_t PmlibSysConfigDisable(uint32_t        modIdx,
                                         pmlibSysConfigErrReturn_t *result,
                                         uint32_t                   timeout);

/**
 * \brief This API is used to auto clock gate a given module.
 *
 * \param  modIdx       Unique ID of the module to be auto clock gated.
 * \param  result      Structure to return to user the cause of failure.
 * \param  timeout     Time for which to wait for the module to be auto clock
 *                     gated.
 *                     PM_TIMEOUT_NOWAIT (0) - The API does not wait.
 *                     PM_TIMEOUT_INFINITE (0xFFFFFFFF) - The API waits
 *                     infinitely.
 *                     Any other value - The API waits to any one of the
 *                     events to happen first : (1) Success of operation
 *                     (2) The timeout is reached.
 *
 * \return errorStatus Status of the API call. Can be any of the following,
 *         PM_SUCCESS  Indicates the operation is success
 *         PM_FAIL     Indicates failure of API
 */
static int32_t PmlibSysConfigRetention(uint32_t        modIdx,
                                        pmlibSysConfigErrReturn_t *result,
                                        uint32_t                   timeout);

/**
 * \brief This API is used to enable a given module.
 *
 * \param  modIdx       Unique ID of the module to be enabled.
 * \param  result      Structure to return to user the cause of failure.
 * \param  timeout     Time for which to wait for the module to be enabled
 *                     PM_TIMEOUT_NOWAIT (0) - The API does not wait.
 *                     PM_TIMEOUT_INFINITE (0xFFFFFFFF) - The API waits
 *                     infinitely.
 *                     Any other value - The API waits to any one of the
 *                     events to happen first : (1) Success of operation
 *                     (2) The timeout is reached.
 *
 * \return errorStatus Status of the API call. Can be any of the following,
 *         PM_SUCCESS  Indicates the operation is success
 *         PM_FAIL     Indicates the module is not enabled (timeout)
 */
static int32_t PmlibSysConfigEnable(uint32_t        modIdx,
                                        pmlibSysConfigErrReturn_t *result,
                                        uint32_t                   timeout);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t PMLIBSysConfigSetPowerState(
    const pmlibSysConfigPowerStateParams_t *inputTable,
    uint32_t                                numConfig,
    uint32_t                                timeout,
    pmlibSysConfigErrReturn_t              *resultReturn)
{
    int32_t status   = PM_SUCCESS;
    uint32_t    modCount = 0U;
    uint32_t    logError = 0U;

    /* Check if the errors can be logged. Done only when a valid pointer
     * is passed to the API for returning the results.
     */
    if (NULL != resultReturn)
    {
        logError = 1U;
    }

    if ((NULL == inputTable) || (0U == numConfig))
    {
        status = PM_BADARGS;
    }
    else
    {
        /* Loop through the number of modules given to the API */
        for (modCount = 0U; modCount < numConfig; modCount++)
        {
            uint32_t        currMod =
                inputTable[modCount].modId;
            uint32_t desiredPwrSt = (uint32_t)
                inputTable[modCount].pwrState;
            /* Check if the input parameters are correct */
            if ((PM_SUCCESS != Sciclient_pmIsModuleValid(currMod)) ||
                (desiredPwrSt > PMLIB_SYS_CONFIG_MAX))
            {
                if (1U == logError)
                {
                    resultReturn[modCount].moduleId  = currMod;
                    resultReturn[modCount].failCause = PM_BADARGS;
                }
                status = PM_FAIL;
            }
            else
            {
                pmlibSysConfigErrReturn_t *currResult = NULL;
                if (1U == logError)
                {
                    currResult = &resultReturn[modCount];
                }
                /* Based on which power state is requested go to the
                 * corresponding internal function.
                 */
                switch (desiredPwrSt)
                {
                    case PMLIB_SYS_CONFIG_DISABLED:
                        /* Case when the module is to be disabled */
                        status += PmlibSysConfigDisable(currMod,
                                    currResult,
                                    timeout);
                        break;
                    case PMLIB_SYS_CONFIG_RETENTION:
                        /* Case when the module automatically goes to low
                         * power when the module is not active.
                         */
                        status += PmlibSysConfigRetention(
                                    currMod,
                                    currResult,
                                    timeout);
                        break;
                    case PMLIB_SYS_CONFIG_ALWAYS_ENABLED:
                        /* Case when the module is in its highest power
                         * configuration.
                         */
                        status += PmlibSysConfigEnable(
                                    currMod,
                                    currResult,
                                    timeout);
                        break;
                    default:
                        /* Invalid power state */
                        if (1U == logError)
                        {
                            currResult->moduleId  = currMod;
                            currResult->failCause = PM_BADARGS;
                        }
                        status += PM_FAIL;
                        break;
                }
            }
        }
    }

    return status;
}

int32_t PMLIBSysConfigPowerStateParams_init(
    pmlibSysConfigPowerStateParams_t *inputTable,
    uint32_t                          numConfig)
{
    int32_t status   = PM_SUCCESS;
    uint32_t    modCount = 0U;
    if ((NULL == inputTable) || (0U == numConfig))
    {
        status = PM_BADARGS;
    }
    else
    {
        /* Loop through the number of modules given. The Init API would
         * fill in the details of all the modules in a sequence of module IDs
         * and initialize the initial state of the modules to DISABLED. Based
         * on what the software wants to enable the modules corresponding to
         * that can be enabled/auto clock gated.
         */
        for (modCount = 0U; modCount < numConfig; modCount++)
        {
            inputTable[modCount].modId = (uint32_t) modCount;
            inputTable[modCount].pwrState = PMLIB_SYS_CONFIG_DISABLED;
        }
    }
    return status;
}

int32_t PMLIBSysConfigGetPowerState(
    uint32_t            moduleId,
    pmlibSysConfigPowerState_t    *currState,
    pmlibSysConfigDetailedState_t *detailedState)
{
    int32_t status = PM_SUCCESS;
    uint32_t    moduleState = 0U;
    uint32_t    resetState = 0U;
    uint32_t    contextLossState = 0U;

    if ((NULL == currState) ||
        (PM_SUCCESS != Sciclient_pmIsModuleValid(moduleId)))
    {
        status = PM_BADARGS;
    }
    else
    {
        status = Sciclient_pmGetModuleState(moduleId,
                                            &moduleState,
                                            &resetState,
                                            &contextLossState,
                                            PM_TIMEOUT_INFINITE);
        if (PM_SUCCESS == status)
        {
            switch (moduleState)
            {
                case TISCI_MSG_VALUE_DEVICE_HW_STATE_ON:
                    *currState = PMLIB_SYS_CONFIG_ALWAYS_ENABLED;
                    break;
                case TISCI_MSG_VALUE_DEVICE_HW_STATE_TRANS:
                    *currState = PMLIB_SYS_CONFIG_INTRANSITION;
                    break;
                case TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF:
                    *currState = PMLIB_SYS_CONFIG_DISABLED;
                    break;
                default:
                    /* Should not reach here. */
                    status = PM_FAIL;
                    break;
            }
            if (detailedState != NULL)
            {
                detailedState->moduleState = moduleState;
                detailedState->resetState = resetState;
                detailedState->contextLossState = contextLossState;
            }
        }
    }
    return status;
}

int32_t PMLIBSysConfigSysReset(uint32_t timeout)
{
    int32_t status = PM_SUCCESS;

    status = Sciclient_pmDeviceReset(timeout);

    return status;
}


/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

static int32_t PmlibSysConfigDisable(uint32_t        modIdx,
                                         pmlibSysConfigErrReturn_t *result,
                                         uint32_t                   timeout)
{
    int32_t     status = PM_SUCCESS;
    /* The assumption is that the module is always exclusively requested and
     * the reset isolation is enabled. For those modules for which reset
     * isolation is not present this flag is ignored.
     */
    status  = Sciclient_pmSetModuleState (modIdx,
                                          TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                          TISCI_MSG_FLAG_AOP |
                                          TISCI_MSG_FLAG_DEVICE_EXCLUSIVE |
                                          TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                          timeout);
    if (status == PM_SUCCESS)
    {
        status  = Sciclient_pmSetModuleRst (modIdx,
                                            0x1U,
                                            timeout);
    }
    if (NULL != result)
    {
        /* Save the result of the operation */
        result->moduleId  = modIdx;
        result->failCause = status;
    }

    if (PM_SUCCESS != status)
    {
        /* The system config API always returns PM_FAIL. The real cause of fail
         * is captured in the result structure.
         */
        status = PM_FAIL;
    }

    return status;
}

static int32_t PmlibSysConfigRetention(uint32_t        modIdx,
                                           pmlibSysConfigErrReturn_t *result,
                                           uint32_t                   timeout)
{
    int32_t     status = PM_SUCCESS;
    /* The assumption is that the module is always exclusively requested and
     * the reset isolation is enabled. For those modules for which reset
     * isolation is not present this flag is ignored.
     */
    status  = Sciclient_pmSetModuleState (modIdx,
                                          TISCI_MSG_VALUE_DEVICE_SW_STATE_RETENTION,
                                          TISCI_MSG_FLAG_AOP |
                                          TISCI_MSG_FLAG_DEVICE_EXCLUSIVE |
                                          TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                          timeout);

    if (NULL != result)
    {
        /* Save the result of the operation */
        result->moduleId  = modIdx;
        result->failCause = status;
    }

    if (PM_SUCCESS != status)
    {
        /* The system config API always returns PM_FAIL. The real cause of fail
         * is captured in the result structure.
         */
        status = PM_FAIL;
    }
    return status;
}

static int32_t PmlibSysConfigEnable(uint32_t        modIdx,
                                        pmlibSysConfigErrReturn_t *result,
                                        uint32_t                   timeout)
{
    int32_t                 status = PM_SUCCESS;
    /* The assumption is that the module is always exclusively requested and
     * the reset isolation is enabled. For those modules for which reset
     * isolation is not present this flag is ignored.
     */
    status  = Sciclient_pmSetModuleState (modIdx,
                                          TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                          TISCI_MSG_FLAG_AOP |
                                          TISCI_MSG_FLAG_DEVICE_EXCLUSIVE |
                                          TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                          timeout);
    if (status == PM_SUCCESS)
    {
        status  = Sciclient_pmSetModuleRst (modIdx,
                                            0x0U,
                                            timeout);
    }
    if (NULL != result)
    {
        /* Save the result of the operation */
        result->moduleId  = modIdx;
        result->failCause = status;
    }

    if (PM_SUCCESS != status)
    {
        /* The system config API always returns PM_FAIL. The real cause of fail
         * is captured in the result structure.
         */
        status = PM_FAIL;
    }

    return status;
}
