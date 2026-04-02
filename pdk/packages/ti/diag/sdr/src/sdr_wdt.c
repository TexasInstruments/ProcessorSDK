/*
 * SDR WDT
 *
 * Software Diagnostics Reference module for Watchdog Timer
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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

#include <stddef.h>

#include <sdr_common.h>
#include <sdr_wdt.h>
#include <sdr_esm.h>

#include "sdr_wdt_priv.h"

/** ---------------------------------------------------------------------------
 *
 * \brief Defines the values for WDT flag used to track self test
 *
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_WDT_TEST_FLAG_NONE=0,
    /**< Error flag no action */
    SDR_WDT_TEST_FLAG_PREPARING=1,
    /**< Error flag to indicate self test in progress */
    SDR_WDT_TEST_FLAG_INPROGRESS=2,
    /**< Error flag to indicate self test in progress */
    SDR_WDT_TEST_FLAG_TRIGGERED=3,
    /**< Error flag to indicate error triggerred */
} SDR_WDT_TestFlag;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of WDT software instance
 *
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_WDT_instance_s
{
    uint64_t elapsedTime;
    /**< Recorded elapsed time in timestamp units */
    uint32_t startTimeStamp;
    /**< Recorded Start time reference */
    uint32_t lastTimeStamp;
    /**< Recorded previous timestamp */
    volatile uint32_t triggerTimeStamp;
    /**< Time stamp when WDT event triggered */
    volatile SDR_ESM_WDT_IntSrc selfTestintSrc;
    /**< Interrupt source expeced with watchdog event */
    volatile SDR_WDT_TimerId selfTestTimerId;
    /**<  Self test timer id */
    volatile SDR_WDT_TestFlag testFlag;
    /**< Test flag to track self test status */
    volatile SDR_WDT_TestType testType;
    /**< Test type */
}  SDR_WDT_Instance_t;

/* Global WDT instance */
static SDR_WDT_Instance_t SDR_WDT_instance;


/* Local function prototypes */
static uint64_t SDR_WDT_getLowTimeout(const SDR_WDT_TimerConfig_t *pTimerConfig);
static SDR_ESM_WDT_IntSrc SDR_WDT_getIntSource(SDR_WDT_TimerId timerId);
static bool SDR_ESM_WDT_callBackFunction (SDR_ESM_WDT_IntSrc intSrc);

/** ============================================================================
 *
 * \brief  Runs diagnostics on configured Watchdog Timer
 *
 * \param  pTimerConfig:  Pointer to timer configuration
 * \param  pTestConfig:   Pointer to test configuration
 *
 * \return  SDR_RETURN_PASS : Success; SDR_RETURN_FAIL for failures
 */
SDR_Result SDR_WDT_selftest (const SDR_WDT_TimerConfig_t *pTimerConfig,
                             const SDR_WDT_TestConfig_t *pTestConfig)
{

    SDR_Result result = SDR_PASS;
    uint32_t timeStamp, timeDelta, triggerDelta;
    uint64_t triggerTime, feedDeltaTime;
    uint64_t lowTimeout;

    feedDeltaTime = ((uint64_t)0ULL);

    SDR_WDT_instance.testFlag = SDR_WDT_TEST_FLAG_NONE;

    if ((pTimerConfig == NULL)
            || (pTestConfig == NULL)){
        result = SDR_BADARGS;
    }

    if ( result == SDR_PASS ) {

        /* Calculate the low time out period */
        lowTimeout = SDR_WDT_getLowTimeout(pTimerConfig);

        /* Initialize variables */
        SDR_WDT_instance.elapsedTime = ((uint64_t)0ULL);
        SDR_WDT_instance.testType = pTestConfig->testType;

        /* Record Timer Id */
        SDR_WDT_instance.selfTestTimerId = pTimerConfig->timerId;

        /* Get the interrupt source */
        SDR_WDT_instance.selfTestintSrc = SDR_WDT_getIntSource(pTimerConfig->timerId);

        /* Set flag to indicate preparing for test */
        SDR_WDT_instance.testFlag = SDR_WDT_TEST_FLAG_PREPARING;

        /* Register callback function with MCU ESM */
        (void)SDR_ESM_registerWDTHandler(SDR_ESM_INSTANCE_MCU,
                                         &SDR_ESM_WDT_callBackFunction);

        /* Feed the watchdog to start */
        SDR_WDT_feedWatchdog(pTimerConfig->timerId);

        /* Clear any initial violations */
        SDR_WDT_clearAllInterruptEvents(SDR_WDT_instance.selfTestintSrc);

        /* Get start time and record it as both start time and last time stamp */
        timeStamp = SDR_getTime();
        SDR_WDT_instance.startTimeStamp = timeStamp;
        SDR_WDT_instance.lastTimeStamp = timeStamp;

        if (pTestConfig->testType == SDR_WDT_TEST_TYPE_END_VIOLATION) {
            /* Wait for the right time to feed the watchdog */
            do
            {
                /* Get the current time */
                timeStamp = SDR_getTime();

                /* Get delta from last timestamp */
                timeDelta = timeStamp - SDR_WDT_instance.lastTimeStamp;

                /* update last time stamp */
                SDR_WDT_instance.lastTimeStamp = timeStamp;

                /* Update delta time from start time */
                SDR_WDT_instance.elapsedTime += (uint64_t)timeDelta;

            } while (SDR_WDT_instance.elapsedTime < lowTimeout);

            feedDeltaTime = SDR_WDT_instance.elapsedTime;
        }

        /* Set the Test flag to in progress */
        SDR_WDT_instance.testFlag = SDR_WDT_TEST_FLAG_INPROGRESS;

        /* Feed the watchdog
           NOTE: For test type Window violation, this will act as a purposefully wrong
           feed to trigger the Window violation event*/
        SDR_WDT_feedWatchdog(pTimerConfig->timerId);

        /* Wait in a loop for events */
        while(SDR_WDT_instance.testFlag != SDR_WDT_TEST_FLAG_TRIGGERED ) {
            /* Get time stamp */
            timeStamp = SDR_getTime();

            /* Get delta from last timestamp */
            timeDelta = timeStamp - SDR_WDT_instance.lastTimeStamp;

            /* update last time stamp */
            SDR_WDT_instance.lastTimeStamp = timeStamp;

            /* Update delta time from start time */
            SDR_WDT_instance.elapsedTime += (uint64_t)timeDelta;

            /* Check if Timeout exceeded */
            if (SDR_WDT_instance.elapsedTime  > pTestConfig->timeoutPeriod) {
                result = SDR_FAIL;
                break;
            }
        }

        /* Clear self test flag */
        SDR_WDT_instance.testFlag = SDR_WDT_TEST_FLAG_NONE;

        /* Check for timeout */
        if ( result == SDR_PASS ) {
            if ( SDR_WDT_instance.triggerTimeStamp > SDR_WDT_instance.lastTimeStamp) {
                /* Check if the WDT event occured within the window */
                triggerDelta = SDR_WDT_instance.triggerTimeStamp - SDR_WDT_instance.lastTimeStamp;
            } else {
                triggerDelta = ((uint32_t)0u);
            }

            /* Calculate Trigger time */
            triggerTime = (SDR_WDT_instance.elapsedTime - feedDeltaTime) + (uint64_t)triggerDelta ;

            if (pTestConfig->testType == SDR_WDT_TEST_TYPE_END_VIOLATION) {
                /* Make sure Trigger time is within the range */
                if ((triggerTime < lowTimeout)
                    || (triggerTime > (pTestConfig->timeoutPeriod))) {
                    result = SDR_FAIL;
                }
            }
        }
        SDR_WDT_clearAllInterruptEvents(SDR_WDT_instance.selfTestintSrc);

        /* De-register WDT handler */
        SDR_ESM_deRegisterWDTHandler(SDR_ESM_INSTANCE_MCU);
    }

    return result;
}

/** ======================================================================
 *
 * \brief   WDT call back function registered with ESM handler
 *
 *
 * \return  None
 */
static bool SDR_ESM_WDT_callBackFunction (SDR_ESM_WDT_IntSrc intSrc)
{
    bool retVal = (bool)false;

    /* If self test intercept event */
    if ((SDR_WDT_instance.testFlag == SDR_WDT_TEST_FLAG_INPROGRESS)
        && (intSrc == SDR_WDT_instance.selfTestintSrc)) {
        if (SDR_WDT_checkWatchDogEvent(intSrc, SDR_WDT_instance.testType)) {
            SDR_WDT_instance.triggerTimeStamp = SDR_getTime();
            SDR_WDT_instance.testFlag = SDR_WDT_TEST_FLAG_TRIGGERED;
        }
        retVal = (bool)true;
    } else {
        if ((SDR_WDT_instance.testFlag == SDR_WDT_TEST_FLAG_PREPARING)
            || (SDR_WDT_instance.testFlag == SDR_WDT_TEST_FLAG_TRIGGERED)){
            retVal = (bool)true;
        }
        SDR_WDT_clearAllInterruptEvents(intSrc);
    }

    return retVal;
}

/** ============================================================================
 *
 * \brief  Calculates the low timeout period for the windowed watchdog.
 *
 * \param  pTimerConfig:  Pointer to timer configuration
 *
 * \return Calculated Low timeout period
 */
static uint64_t SDR_WDT_getLowTimeout(const SDR_WDT_TimerConfig_t *pTimerConfig)
{
    uint64_t loTimeout;


    /* Calculate teh timeout period based on window size */
    switch(pTimerConfig->windowSize) {

        case SDR_WDT_WINDOWSIZE_50_PERCENT:
            loTimeout = (uint64_t)(pTimerConfig->timerPeriod >> 1);
            break;

        case SDR_WDT_WINDOWSIZE_25_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 2));
            break;

        case SDR_WDT_WINDOWSIZE_12_5_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 3));
            break;

        case SDR_WDT_WINDOWSIZE_6_25_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 4));
            break;

        case SDR_WDT_WINDOWSIZE_3_125_PERCENT:
            loTimeout = pTimerConfig->timerPeriod
                        - ((uint64_t)(pTimerConfig->timerPeriod >> 5));
            break;

        case SDR_WDT_WINDOWSIZE_100_PERCENT:
        default:
            loTimeout = ((uint64_t)0ULL);
            break;
    }

    return loTimeout;

}

/** ============================================================================
 *
 * \brief  Gets watch dog timer interrupt source for the given Timer id
 *
 * \param  timerId: Timer Id which timer to get the corresponding interrupt source
 *
 * \return Interrupt source number
 */
static SDR_ESM_WDT_IntSrc SDR_WDT_getIntSource(SDR_WDT_TimerId timerId)
{
    SDR_ESM_WDT_IntSrc intSrc;


    /* Calculate teh timeout period based on window size */
    switch(timerId) {

        case SDR_WDT_TIMER_1:
            intSrc = SDR_ESM_TIMER_ID_1;
            break;

        case SDR_WDT_TIMER_0:
        default:
            intSrc = SDR_ESM_TIMER_ID_0;
            break;
    }

    return intSrc;

}
