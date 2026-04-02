/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
  *  \file     pm_checkers_app.c
  *
  *  \brief    This file contains PM safety checkers app code.
  *
  */

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/

#include <sciclient.h>
#include <safety_checkers_pm.h>
#include <safety_checkers_soc.h>
#include <safety_checkers_common.h>

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

void SafetyCheckersApp_pmRun(void *arg0);
extern uint64_t SafetyCheckersApp_getTimeUsec(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static int32_t SafetyCheckersApp_pmClockIdCheck(uint32_t offset);
static int32_t SafetyCheckersApp_pmClockStateCheck(uint32_t offset);
static int32_t SafetyCheckersApp_pmDevGrpCheck(void);
static int32_t SafetyCheckersApp_pmDevIdCheck(uint32_t offset);
static int32_t SafetyCheckersApp_pmDevResetCheck(uint32_t offset);
static int32_t SafetyCheckersApp_pmDevStateCheck(uint32_t offset);
static int32_t SafetyCheckersApp_pmParentClkCheck(uint32_t offset);
static int32_t SafetyCheckersApp_pmPllLockVerify(int32_t deviceID);
static int32_t SafetyCheckersApp_pmRegCfgVerify(int32_t deviceID);
static int32_t SafetyCheckersApp_pmErrCheck(void);
static int32_t SafetyCheckersApp_pmPerfTest(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void SafetyCheckersApp_pmRun(void *arg0)
{
	uint32_t    status = SAFETY_CHECKERS_SOK;

	status = SafetyCheckersApp_pmClockIdCheck(SAFETY_CHECKERS_PM_INVAL_PARAM_OFFSET_RANGE);
    if(SAFETY_CHECKERS_SOK == status)
    {
   	    status = SafetyCheckersApp_pmClockStateCheck(SAFETY_CHECKERS_PM_INVAL_PARAM_OFFSET_RANGE);
    }

    if(SAFETY_CHECKERS_SOK == status)
   	{
   	    status = SafetyCheckersApp_pmDevIdCheck(SAFETY_CHECKERS_PM_INVAL_PARAM_OFFSET_RANGE);
    }

    if(SAFETY_CHECKERS_SOK == status)
   	{
   	    status = SafetyCheckersApp_pmDevResetCheck(SAFETY_CHECKERS_PM_INVAL_PARAM_OFFSET_RANGE);
    }

    if(SAFETY_CHECKERS_SOK == status)
   	{
   	    status = SafetyCheckersApp_pmDevStateCheck(SAFETY_CHECKERS_PM_INVAL_PARAM_OFFSET_RANGE);
    }

    if(SAFETY_CHECKERS_SOK == status)
   	{
   	    status = SafetyCheckersApp_pmParentClkCheck(SAFETY_CHECKERS_PM_INVAL_PARAM_OFFSET_RANGE);
    }

   	if(SAFETY_CHECKERS_SOK == status)
    {
        status = SafetyCheckersApp_pmRegCfgVerify(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID);
   	}

   	if(SAFETY_CHECKERS_SOK == status)
    {
        status = SafetyCheckersApp_pmPllLockVerify(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID);
   	}

    if(SAFETY_CHECKERS_SOK == status)
   	{
   	    status = SafetyCheckersApp_pmDevGrpCheck();
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        status = SafetyCheckersApp_pmErrCheck();
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        SAFETY_CHECKERS_log("\n*****  Starting negative test cases  *****\r\n");

        status = SafetyCheckersApp_pmClockIdCheck(SAFETY_CHECKERS_PM_VAL_PARAM_OFFSET_RANGE);
        if(SAFETY_CHECKERS_SOK != status)
        {
   	        status = SafetyCheckersApp_pmClockStateCheck(SAFETY_CHECKERS_PM_VAL_PARAM_OFFSET_RANGE);
        }

        if(SAFETY_CHECKERS_SOK != status)
   	    {
   	        status = SafetyCheckersApp_pmDevIdCheck(SAFETY_CHECKERS_PM_VAL_PARAM_OFFSET_RANGE);
        }

        if(SAFETY_CHECKERS_SOK != status)
   	    {
   	        status = SafetyCheckersApp_pmDevResetCheck(SAFETY_CHECKERS_PM_VAL_PARAM_OFFSET_RANGE);
        }

        if(SAFETY_CHECKERS_SOK != status)
   	    {
   	        status = SafetyCheckersApp_pmDevStateCheck(SAFETY_CHECKERS_PM_VAL_PARAM_OFFSET_RANGE);
        }

        if(SAFETY_CHECKERS_SOK != status)
   	    {
   	        status = SafetyCheckersApp_pmParentClkCheck(SAFETY_CHECKERS_PM_VAL_PARAM_OFFSET_RANGE);
        }

   	    if(SAFETY_CHECKERS_SOK != status)
        {
            status = SafetyCheckersApp_pmRegCfgVerify(SAFETY_CHECKERS_PM_INVAL_DEV_ID);
   	    }

   	    if(SAFETY_CHECKERS_SOK != status)
        {
            status = SafetyCheckersApp_pmPllLockVerify(SAFETY_CHECKERS_PM_INVAL_DEV_ID);
   	    }

        SAFETY_CHECKERS_log("\nEnding negative test cases\r\n");
		
		if(SAFETY_CHECKERS_SOK != status)
        {
            status = SafetyCheckersApp_pmPerfTest();
   	    }
    }

#if defined LDRA_DYN_COVERAGE_EXIT
    SAFETY_CHECKERS_log("\n LDRA ENTRY... \r\n");
    upload_execution_history();
    SAFETY_CHECKERS_log("\n LDRA EXIT... \r\n");
#endif

    if(SAFETY_CHECKERS_SOK == status)
    {
        SAFETY_CHECKERS_log("\nAll tests have PASSED.\r\n");
    }

	return;
}

/* ========================================================================== */
/*                       Internal Function Definitions                        */
/* ========================================================================== */

static int32_t SafetyCheckersApp_pmClockIdCheck(uint32_t offset)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    uint32_t     clockStatus = 0U;
    uint32_t     clockParent = 0U;
    uint32_t     clockNumParent = 0U;
    uint64_t     reqFreqHz = 0U;
    uint64_t     freqHz = 0U;
    int32_t      clockIdRange;

    /* Valid Clock ID Range is 0 - 500 */
    int32_t clockId[] = {SAFETY_CHECKERS_PM_VALID_CLOCK_ID, -1, 500};
    for (clockIdRange = offset; clockIdRange < (sizeof(clockId) / sizeof(clockId[0])); clockIdRange++)
    {
        if(SAFETY_CHECKERS_SOK != status)
        {
            /* TISCI_MSG_SET_CLOCK PM message */
            status = Sciclient_pmModuleClkRequest(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                  clockId[clockIdRange],
                                                  SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                  SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                  SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_SET_CLOCK_PARENT PM message */
                status = Sciclient_pmSetModuleClkParent(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                        clockId[clockIdRange],
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_SET_FREQ PM message */
                status = Sciclient_pmSetModuleClkFreq(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                       clockId[clockIdRange],
                                                       SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                       SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                       SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_CLOCK PM message */
                status = Sciclient_pmModuleGetClkStatus(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                        clockId[clockIdRange],
                                                        &clockStatus,
                                                        SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_CLOCK_PARENT PM message */
                status = Sciclient_pmGetModuleClkParent(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                        clockId[clockIdRange],
                                                        &clockParent,
														SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_NUM_CLOCK_PARENTS PM message */
                status = Sciclient_pmGetModuleClkNumParent(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                           clockId[clockIdRange],
                                                           &clockNumParent,
                                                           SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_QUERY_FREQ PM message */
                status = Sciclient_pmQueryModuleClkFreq(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                        clockId[clockIdRange],
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        &reqFreqHz,
                                                        SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_FREQ PM message */
                status = Sciclient_pmGetModuleClkFreq(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                      clockId[clockIdRange],
                                                      &freqHz,
                                                      SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }
        }
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nClock ID boundary validation passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: Clock ID boundary validation failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmClockStateCheck(uint32_t offset)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    int32_t      clockStateRange;

    /* Valid Clock State Range is 0 - 2 */
    int32_t clockStates[] = {SAFETY_CHECKERS_PM_DEFAULT_ID, -1, 3};

    for (clockStateRange = offset; clockStateRange < (sizeof(clockStates) / sizeof(clockStates[0])); clockStateRange++)
    {
        if(SAFETY_CHECKERS_SOK != status)
        {
            /* TISCI_MSG_SET_CLOCK PM message */
            status = Sciclient_pmModuleClkRequest(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                  SAFETY_CHECKERS_PM_VALID_CLOCK_ID,
                                                  clockStates[clockStateRange],
                                                  SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                  SAFETY_CHECKERS_DEFAULT_TIMEOUT);
        }
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nClock State boundary validation passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: Clock State boundary validation failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmDevGrpCheck(void)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    int32_t      domainGrpRange;

    /* Valid Device Group Range is [0 - ((0x01U) << 7U)] */
    int32_t domainGrp[] = {-1, 0x81};

    for (domainGrpRange = 0; domainGrpRange < (sizeof(domainGrp) / sizeof(domainGrp[0])); domainGrpRange++)
    {
        if(SAFETY_CHECKERS_SOK != status)
        {
            /* TISCI_MSG_SYS_RESET PM message */
            status = Sciclient_pmDomainReset(domainGrp[domainGrpRange], SAFETY_CHECKERS_DEFAULT_TIMEOUT);
        }
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nDevice Group boundary validation passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmDevIdCheck(uint32_t offset)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    uint32_t     moduleState = 0U;
    uint32_t     resetState = 0U;
    uint32_t     contextLossState = 0U;
    uint32_t     clockStatus = 0U;
    uint32_t     clockParent = 0U;
    uint32_t     clockNumParent = 0U;
    uint64_t     reqFreqHz = 0U;
    uint64_t     freqHz = 0U;
    int32_t      deviceIdRange;

    /* Valid Device ID Range is 0 - 433 */
    int32_t deviceId[] = {SAFETY_CHECKERS_PM_VALID_DEVICE_ID, -1, 434};

    for (deviceIdRange = offset; deviceIdRange < (sizeof(deviceId) / sizeof(deviceId[0])); deviceIdRange++)
    {
        if(SAFETY_CHECKERS_SOK != status)
        {
            /* TISCI_MSG_SET_DEVICE PM message */
            status = Sciclient_pmSetModuleState(deviceId[deviceIdRange],
                                                SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                SAFETY_CHECKERS_PM_DFT_VAL_REQUEST_FLAG,
                                                SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_DEVICE PM message */
                status = Sciclient_pmGetModuleState(deviceId[deviceIdRange],
                                                    &moduleState,
                                                    &resetState,
                                                    &contextLossState,
                                                    SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_SET_DEVICE_RESETS PM message */
                status = Sciclient_pmSetModuleRst(deviceId[deviceIdRange],
												  SAFETY_CHECKERS_PM_DEFAULT_ID,
												  SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_SET_CLOCK PM message */
                status = Sciclient_pmModuleClkRequest(deviceId[deviceIdRange],
                                                      SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                      SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                      SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                      SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_SET_CLOCK_PARENT PM message */
                status = Sciclient_pmSetModuleClkParent(deviceId[deviceIdRange],
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_SET_FREQ PM message */
                status = Sciclient_pmSetModuleClkFreq(deviceId[deviceIdRange],
                                                       SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                       SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                       SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                       SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_CLOCK PM message */
                status = Sciclient_pmModuleGetClkStatus(deviceId[deviceIdRange],
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        &clockStatus,
                                                        SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_CLOCK_PARENT PM message */
                status = Sciclient_pmGetModuleClkParent(deviceId[deviceIdRange],
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        &clockParent,
                                                        SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_NUM_CLOCK_PARENTS PM message */
                status = Sciclient_pmGetModuleClkNumParent(deviceId[deviceIdRange],
                                                          SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                          &clockNumParent,
                                                          SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_QUERY_FREQ PM message */
                status = Sciclient_pmQueryModuleClkFreq(deviceId[deviceIdRange],
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                        &reqFreqHz,
                                                        SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }

            if(SAFETY_CHECKERS_SOK != status)
            {
                /* TISCI_MSG_GET_FREQ PM message */
                status = Sciclient_pmGetModuleClkFreq(deviceId[deviceIdRange],
                                                      SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                      &freqHz,
                                                      SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }
        }
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nDevice ID boundary validation passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: Device ID boundary validation failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmDevResetCheck(uint32_t offset)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    int32_t      deviceRstRange;

    /* Valid Device resets Range is 0 - 3 */
    int32_t deviceResets[] = {SAFETY_CHECKERS_PM_DEFAULT_ID, -1, 4};

    for (deviceRstRange = offset; deviceRstRange < (sizeof(deviceResets) / sizeof(deviceResets[0])); deviceRstRange++)
    {
        if(SAFETY_CHECKERS_SOK != status)
        {
            /* TISCI_MSG_SET_DEVICE_RESETS PM message */
            status = Sciclient_pmSetModuleRst (SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                               deviceResets[deviceRstRange],
                                               SAFETY_CHECKERS_DEFAULT_TIMEOUT);
        }
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nDevice Reset boundary validation passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: Device Reset boundary validation failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmDevStateCheck(uint32_t offset)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    int32_t      deviceStateRange;

    /* Valid  Device State Range is 0 - 2 */
    int32_t deviceState[] = {SAFETY_CHECKERS_PM_DEFAULT_ID, -1, 3};

    for (deviceStateRange = offset; deviceStateRange < (sizeof(deviceState) / sizeof(deviceState[0])); deviceStateRange++)
    {
        if(SAFETY_CHECKERS_SOK != status)
        {
            /* TISCI_MSG_SET_DEVICE PM message */
            status = Sciclient_pmSetModuleState(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                deviceState[deviceStateRange],
                                                SAFETY_CHECKERS_PM_DFT_VAL_REQUEST_FLAG,
                                                SAFETY_CHECKERS_DEFAULT_TIMEOUT);
        }
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nDevice State boundary validation passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: Device State boundary validation failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmParentClkCheck(uint32_t offset)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    int32_t      parentClkRange;

    /* Valid Parent Clock Range is 0 - 500 */
    int32_t parentClk[] = {SAFETY_CHECKERS_PM_VALID_PARENT_CLOCK_ID, -1, 500};

    for (parentClkRange = offset; parentClkRange < (sizeof(parentClk) / sizeof(parentClk[0])); parentClkRange++)
    {
        if(SAFETY_CHECKERS_SOK != status)
        {
            /* TISCI_MSG_SET_CLOCK_PARENT PM message */
            status = Sciclient_pmSetModuleClkParent(SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_ID,
                                                    SAFETY_CHECKERS_PM_VALID_CLOCK_ID,
                                                    parentClk[parentClkRange],
                                                    SAFETY_CHECKERS_DEFAULT_TIMEOUT);
        }
    }

    if(SAFETY_CHECKERS_SOK != status)
    {
        SAFETY_CHECKERS_log("\nParent Clock boundary validation passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: Parent Clock boundary validation failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmPllLockVerify(int32_t deviceID)
{
    int32_t      status = SAFETY_CHECKERS_SOK;
    uintptr_t    registerData[SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE];

    status = SafetyCheckers_pmGetPllRegCfg (registerData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);

    if(SAFETY_CHECKERS_SOK == status)
    {
        status = SafetyCheckers_pmRegisterLock();
        if(SAFETY_CHECKERS_SOK == status)
        {
            /* Call the API which changes PLL register values */
            status = Sciclient_pmSetModuleClkFreq(deviceID,
                                                  SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_ID,
                                                  SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_FREQ,
                                                  SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                  SAFETY_CHECKERS_DEFAULT_TIMEOUT);
        }

        if(SAFETY_CHECKERS_SOK == status)
        {
            /* Safety checker API should return success */
            status = SafetyCheckers_pmVerifyPllRegCfg (registerData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);
        }
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        SAFETY_CHECKERS_log("\nPLL register Lock test Passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: PLL register Lock test failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmRegCfgVerify(int32_t deviceID)
{
    int32_t      status = SAFETY_CHECKERS_SOK;
    uintptr_t    pscRegisterData[SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE];
    uintptr_t    pllRegisterData[SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE];

    /* Get the PSC register dump */
    status = SafetyCheckers_pmGetPscRegCfg (pscRegisterData, SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE);

    if(SAFETY_CHECKERS_SOK == status)
    {
        /* validate register dump with current value */
        status = SafetyCheckers_pmVerifyPscRegCfg (pscRegisterData, SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE);
        if(SAFETY_CHECKERS_SOK == status)
        {

            /* Call the API which changes PSC register values */
            status = Sciclient_pmSetModuleState(deviceID,
                                                SAFETY_CHECKERS_PM_DFT_VAL_DEVICE_STATE,
                                                SAFETY_CHECKERS_PM_DFT_VAL_REQUEST_FLAG,
                                                SAFETY_CHECKERS_DEFAULT_TIMEOUT);
        }
        if(SAFETY_CHECKERS_SOK == status)
        {
            /* Safety checker API should return failure */
            status = SafetyCheckers_pmVerifyPscRegCfg (pscRegisterData, SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE);

            if(SAFETY_CHECKERS_REG_DATA_MISMATCH == status)
            {
                SAFETY_CHECKERS_log("\nPSC register dump and readback test passed\r\n\n");
            }
        }
    }

    if(SAFETY_CHECKERS_REG_DATA_MISMATCH == status)
    {
        /* Get the PLL register dump */
        status = SafetyCheckers_pmGetPllRegCfg (pllRegisterData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);

        if(SAFETY_CHECKERS_SOK == status)
        {
            /* validate register dump with current value */
            status = SafetyCheckers_pmVerifyPllRegCfg (pllRegisterData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);
            if(SAFETY_CHECKERS_SOK == status)
            {
                /* Call the API which changes PLL register values */
                status = Sciclient_pmSetModuleClkFreq(deviceID,
                                                      SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_ID,
                                                      SAFETY_CHECKERS_PM_DFT_VAL_CLOCK_FREQ,
                                                      SAFETY_CHECKERS_PM_DEFAULT_ID,
                                                      SAFETY_CHECKERS_DEFAULT_TIMEOUT);
            }
            if(SAFETY_CHECKERS_SOK == status)
            {
                /* Safety checker API should return failure */
                status = SafetyCheckers_pmVerifyPllRegCfg (pllRegisterData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);

                if(SAFETY_CHECKERS_REG_DATA_MISMATCH == status)
                {
                    SAFETY_CHECKERS_log("\nPLL register dump and readback test passed\r\n\n");
                }
            }
        }
    }

    if(SAFETY_CHECKERS_REG_DATA_MISMATCH == status)
    {
        SAFETY_CHECKERS_log("\nPSC and PLL register dump test passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        SAFETY_CHECKERS_log("\nError: PSC and PLL register dump test failed !!!\r\n\n");
        status = SAFETY_CHECKERS_FAIL;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmErrCheck(void)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    uint32_t     size = 10;
    uintptr_t    pmRegisterData[size];

    status = SafetyCheckers_pmGetPscRegCfg (NULL, size);
    if(SAFETY_CHECKERS_FAIL == status)
    {
        status = SafetyCheckers_pmVerifyPscRegCfg (NULL, size);
        if(SAFETY_CHECKERS_FAIL == status)
        {
            status = SafetyCheckers_pmGetPllRegCfg (NULL, size);
            if(SAFETY_CHECKERS_FAIL == status)
            {
                status = SafetyCheckers_pmVerifyPllRegCfg (NULL, size);
            }
        }
    }

    if(SAFETY_CHECKERS_FAIL == status)
    {
        status = SafetyCheckers_pmGetPscRegCfg (pmRegisterData, size);
        if(SAFETY_CHECKERS_INSUFFICIENT_BUFF == status)
        {
            status = SafetyCheckers_pmVerifyPscRegCfg (pmRegisterData, size);
            if(SAFETY_CHECKERS_INSUFFICIENT_BUFF == status)
            {
                status = SafetyCheckers_pmGetPllRegCfg (pmRegisterData, size);
                if(SAFETY_CHECKERS_INSUFFICIENT_BUFF == status)
                {
                    status = SafetyCheckers_pmVerifyPllRegCfg (pmRegisterData, size);
                }
            }
        }
    }

    if(SAFETY_CHECKERS_INSUFFICIENT_BUFF == status)
    {
        SAFETY_CHECKERS_log("\nPM error condition check test passed\r\n\n");
        status = SAFETY_CHECKERS_SOK;
    }

    return (status);
}

static int32_t SafetyCheckersApp_pmPerfTest(void)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    uint64_t     startTime = 0U;
    uint64_t     endTime = 0U;
    uint32_t     timeDiff = 0U;
    uintptr_t    pscRegisterData[SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE];
    uintptr_t    pllRegisterData[SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE];

    /* Get the PSC register dump */
    status = SafetyCheckers_pmGetPscRegCfg (pscRegisterData, SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE);

    if(SAFETY_CHECKERS_SOK == status)
    {
        startTime = SafetyCheckersApp_getTimeUsec();

        /* validate register dump with current value */
        status = SafetyCheckers_pmVerifyPscRegCfg (pscRegisterData, SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE);

        endTime = SafetyCheckersApp_getTimeUsec();

        if (endTime < startTime)
        {
            /* Counter overflow occured */
            timeDiff = (0xFFFFFFFFU - startTime) + endTime + 1U;
        }
        timeDiff = endTime - startTime;

        SAFETY_CHECKERS_log("\nTime taken for the execution of PSC register dump and readback : %d usecs\r\n", timeDiff);
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        /* Get the PLL register dump */
        status = SafetyCheckers_pmGetPllRegCfg (pllRegisterData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);

        if(SAFETY_CHECKERS_SOK == status)
        {
            startTime = SafetyCheckersApp_getTimeUsec();

            /* validate register dump with current value */
            status = SafetyCheckers_pmVerifyPllRegCfg (pllRegisterData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);

            endTime = SafetyCheckersApp_getTimeUsec();
            if (endTime < startTime)
            {
                /* Counter overflow occured */
                timeDiff = (0xFFFFFFFFU - startTime) + endTime + 1U;
            }
            timeDiff = endTime - startTime;

            SAFETY_CHECKERS_log("\nTime taken for the execution of PLL register dump and readback : %d usecs\r\n", timeDiff);
        }
    }

    if(SAFETY_CHECKERS_SOK == status)
    {
        startTime = SafetyCheckersApp_getTimeUsec();

        /* validate PLL register lock */
        status = SafetyCheckers_pmRegisterLock();

        endTime = SafetyCheckersApp_getTimeUsec();
        if (endTime < startTime)
        {
            /* Counter overflow occured */
            timeDiff = (0xFFFFFFFFU - startTime) + endTime + 1U;
        }
        timeDiff = endTime - startTime;

        SAFETY_CHECKERS_log("\nTime taken for the execution of PLL register lock : %d usecs\r\n", timeDiff);
    }

    return (status);
}
