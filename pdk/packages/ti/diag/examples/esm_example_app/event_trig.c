/*
 * ESM Example
 *
 * Error Signaling Module (ESM) Example Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file event_trig.c
 *
 *  \brief This file contains functions that provide input event triggers
 *         for the Error Signaling Module (ESM) application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* For Timer functions */
#include <ti/osal/osal.h>

#include <ti/drv/sciclient/sciclient.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>

#include <ti/csl/soc.h>
#include <ti/csl/csl_vtm.h>
#include <ti/csl/csl_esm.h>

#include <sdr_esm.h>
#include "event_trig.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define PIN_TIMING_TIMER_ID      (1)
#define HIGH_PRIO_TRIG_TIMER_ID  (2)
#define LOW_PRIO_TRIG_TIMER_ID   (3)

#define CSL_VTM_TEST_CFG1_BASE   (CSL_WKUP_VTM0_MMR_VBUSP_CFG1_BASE)
#define CSL_VTM_TEST_CFG2_BASE   (CSL_WKUP_VTM0_MMR_VBUSP_CFG2_BASE)

#define LT_THR0_DEFAULT          (95000)
#define GT_THR1_DEFAULT          (105000)
#define GT_THR2_DEFAULT          (115000)

#define PIN_CLEAR_PERIOD_USEC    (10)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void timerExpTrigger(uintptr_t arg);
static int32_t vtmTriggerTh(int32_t lt_thr0_offset,
                            int32_t gt_thr1_offset,
                            int32_t gt_thr2_offset);
static int32_t vtmTriggerTh1(void);
static void setAllVTMTempThr(CSL_vtm_adc_code lt_thr0_adc_code,
                             CSL_vtm_adc_code gt_thr1_adc_code,
                             CSL_vtm_adc_code gt_thr2_adc_code);
static void timerExpPinDisable(uintptr_t arg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Completion of Use Case from Input trigger perspective updates these flags */
extern volatile uint32_t    esmEventInputTrig[5];
/* Completion of Use Case from Output Pin clearing perspective updates these flags */
extern volatile uint32_t    esmPinClearResult[5];
/* Current use case being run */
extern volatile uint8_t     currUseCase;

static uint32_t             esmPinMinIntervalCycles;
static uint32_t             esmPinMinIntervalUsec;
static uint32_t             pinClearTimeCycles;
static bool                 immediatePinClear;

static TimerP_Handle        esmHiEventTrigTimerHandle;
static TimerP_Handle        esmLoEventTrigTimerHandle;
static TimerP_Handle        esmPinTimerHandle;
static SDR_ESM_InstanceType currEsmInstance;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


/*********************************************************************
* @fn      esmPinTimeInit
*
* @brief   This checks current timings for the minimum interval time
*          on the MCU_SAFETY_ERRORn pin for eacl Error Signaling Module
*
* @param   pinClearTime: Amount of time in microseconds that the Pin control
*          timer waits before triggering clearing of the MCU_SAFETY_ERRORn pin
*
* @return    0 : Success; < 0 for failures
*/
int32_t esmPinTimeInit(uint32_t pinClearTime)
{
    int32_t retVal = 0;
    uint32_t modId;
    uint32_t clkId;
    uint64_t esmInputClk;

    ESMGetErrPinLowTimePreload(CSL_MCU_ESM0_CFG_BASE, &esmPinMinIntervalCycles);
#ifdef PRINT_DEBUG
    DIAG_printf("  MCU ESM pin minimum interval is %d cycles\n",
                esmPinMinIntervalCycles);
#endif
    ESMGetErrPinLowTimePreload(CSL_WKUP_ESM0_CFG_BASE, &esmPinMinIntervalCycles);
#ifdef PRINT_DEBUG
    DIAG_printf("  WKUP ESM pin minimum interval is %d cycles\n",
                esmPinMinIntervalCycles);
#endif

    /* WKUP ESM clock */
    modId = TISCI_DEV_WKUP_ESM0;
    clkId = TISCI_DEV_WKUP_ESM0_CLK;

    retVal = Sciclient_pmGetModuleClkFreq(modId,
                                          clkId,
                                          &esmInputClk,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (retVal == CSL_PASS) {
#ifdef PRINT_DEBUG
        DIAG_printf("  WKUP ESM input clock is %d\n", esmInputClk);
#endif
        /* MCU ESM clock */
        modId = TISCI_DEV_MCU_ESM0;
        clkId = TISCI_DEV_MCU_ESM0_CLK;

        retVal = Sciclient_pmGetModuleClkFreq(modId,
                                              clkId,
                                              &esmInputClk,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
        if (retVal == CSL_PASS) {
#ifdef PRINT_DEBUG
            DIAG_printf("  MCU ESM input clock is %d\n", esmInputClk);
#endif
        }
    }

    if (retVal == CSL_PASS) {
        /* Translate Pin Timer Time (microseconds) into number of ESM cycles */
        pinClearTimeCycles = (uint32_t)((float)pinClearTime /
                             1000000 * (float)esmInputClk);
        DIAG_printf("\n  Any clear of MCU_SAFETY_ERRORn pin will first wait " \
                    "%d usecs", pinClearTime);
        /* Translate Minimum Time Interval (cycles) into time (microseconds)*/
        esmPinMinIntervalUsec = (uint32_t)((float)esmPinMinIntervalCycles /
                                           (float)esmInputClk * 1000000);
        DIAG_printf("\n  Minimum Time Interval is %d usecs", esmPinMinIntervalUsec);
    }
    else {
        retVal = -1;
    }

    /* If desired, ESMSetErrPinLowTimePreload can be used to change
     * minimum interval time here */

    return retVal;
}

/* TIMER FUNCTIONS */

/*********************************************************************
* @fn      diag_esm_timerInit
*
* @brief   This initializes all timers for the Error Signaling Module (ESM)
*          example application.
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t diag_esm_timerInit(void)
{
    TimerP_Params   timerParams;

    /* Start Timer and register call back for periodic functions */
    /* Initialize timer parameters */
    TimerP_Params_init(&timerParams);

    /* Configure periodic timer for 0.005 sec*/
    timerParams.period = 5000;  /*  0.005s */
    timerParams.periodType = TimerP_PeriodType_MICROSECS;
    timerParams.arg = (void *)HIGH_PRIO_TRIG_TIMER_ID;
    timerParams.startMode = TimerP_StartMode_USER;
    timerParams.runMode = TimerP_RunMode_ONESHOT;

    /* Create High Priority Event Trigger Timer */
    esmHiEventTrigTimerHandle = TimerP_create(HIGH_PRIO_TRIG_TIMER_ID,  
                                              (TimerP_Fxn)&timerExpTrigger,
                                              &timerParams);
    if (esmHiEventTrigTimerHandle == NULL) {
       DIAG_printf("ERR: High Priority Trigger Timer create failed\n");
       return -1;
    }

    /* Low Priority timer has same settings as High Priority timer
     * but just different timer instance */
    timerParams.arg = (void *)LOW_PRIO_TRIG_TIMER_ID;

    /* Create Low Priority Event Trigger Timer */
    esmLoEventTrigTimerHandle = TimerP_create(LOW_PRIO_TRIG_TIMER_ID,  
                                              (TimerP_Fxn)&timerExpTrigger,
                                              &timerParams);
    if (esmLoEventTrigTimerHandle == NULL) {
       DIAG_printf("ERR: Low Priority Trigger Timer create failed\n");
       return -1;
    }

    /* Configure periodic timer for 10 microseconds */
    timerParams.period = PIN_CLEAR_PERIOD_USEC;  /*  10 usec */
    timerParams.periodType = TimerP_PeriodType_MICROSECS;
    timerParams.arg = (void *)&currEsmInstance;
    timerParams.startMode = TimerP_StartMode_USER;
    timerParams.runMode = TimerP_RunMode_ONESHOT;

    /* Create Timer for Pin Control */
    esmPinTimerHandle = TimerP_create(PIN_TIMING_TIMER_ID,
                                      (TimerP_Fxn)&timerExpPinDisable,
                                      &timerParams);
    if (esmPinTimerHandle == NULL) {
       DIAG_printf("ERR: Pin Control and Measurment Timer create failed\n");
       return -1;
    }

    if (esmPinTimeInit(PIN_CLEAR_PERIOD_USEC) != 0) {
       DIAG_printf("ERR: Pin Time Init failed\n");
       return -1;
    }

    DIAG_printf("\nESM example timer initialization complete\n");

    return 0;
}

/*********************************************************************
* @fn      diag_esm_timerDelete
*
* @brief   This deletes all timers for the Error Signaling Module (ESM)
*          example application.
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t diag_esm_timersDelete(void)
{
    int32_t retVal = 0;

    if (esmHiEventTrigTimerHandle != NULL_PTR) {
        if (TimerP_delete(esmHiEventTrigTimerHandle) != TimerP_OK) {
            retVal = -1;
        }
    }

    if (esmLoEventTrigTimerHandle != NULL_PTR) {
        if (TimerP_delete(esmLoEventTrigTimerHandle) != TimerP_OK) {
            retVal = -1;
        }
    }

    if (esmPinTimerHandle != NULL_PTR) {
        if (TimerP_delete(esmPinTimerHandle) != TimerP_OK) {
            retVal = -1;
        }
    }

    return (retVal);
}

/*********************************************************************
* This callback is triggered by expiration of the input event
* trigger timers.
*/
static void timerExpTrigger(uintptr_t arg)
{
    TimerP_Status   timerStatus;
    volatile uint32_t storeArg = arg;

    if (storeArg == HIGH_PRIO_TRIG_TIMER_ID)
    {
        esmEventInputTrig[4] = USE_CASE_STATUS_COMPLETED_SUCCESS;
#ifdef PRINT_DEBUG
        DIAG_printf("timerExpTrigger: High Priority interrupt timer expired\n");
#endif
        /* Start Timer to control when external Pin is reset */
        timerStatus = TimerP_start(esmPinTimerHandle);
        if (timerStatus != TimerP_OK) {
            DIAG_printf("ERR: Could not start the timer %d \n",
                        PIN_TIMING_TIMER_ID);
        }
    } else if (storeArg == LOW_PRIO_TRIG_TIMER_ID) {
        esmEventInputTrig[3] = USE_CASE_STATUS_COMPLETED_SUCCESS;
#ifdef PRINT_DEBUG
        DIAG_printf("timerExpTrigger: Low Priority interrupt timer expired\n");
#endif
    }
}

/*
* This callback is triggered by expiration of the pin disable timer
*/
static void timerExpPinDisable(uintptr_t arg)
{
    int32_t retVal = 0;
    bool pinStatus;
    SDR_Result sdrStatus;

    volatile SDR_ESM_InstanceType *pInstType = (SDR_ESM_InstanceType *)arg;

    pinStatus = SDR_ESM_getNErrorStatus(*pInstType);
    if (pinStatus != 0) {
        DIAG_printf("\n  timerExpPinDisable: Incorrect pin value before clearing\n");
        retVal = -1;
    } else {
#ifdef DEBUG_PRINT
    DIAG_printf("\n  timerExpPinDisable: before clear, ESM instance %d view of " \
                "MCU_SAFETY_ERRORn pin is %d\n",
                *pInstType, pinStatus);
#endif
    }

    sdrStatus = SDR_ESM_resetNError(*pInstType);

    if (sdrStatus != SDR_PASS) {
        /* If resetNError is called within minimum time interval, then the result of
         * SDR_ESM_resetNError will be failure because pin does not change until
         * minimum time interval has expired */
        if ((pinClearTimeCycles >= esmPinMinIntervalCycles) ||
            (immediatePinClear)) {
            DIAG_printf("\n  timerExpPinDisable: ERROR - Failed to clear the error " \
                        "pin from ESM Instance %d\n",
                        *pInstType);
            retVal = -1;
        } else {
            /* Delay 1msec after minimum time interval is done and check the pin
             * again */
            Osal_delay((esmPinMinIntervalUsec / 1000) + 1);
        }
    } else {
#ifdef DEBUG_PRINT
        DIAG_printf("  timerExpPinDisable: Cleared the error pin successfully from " \
                    "ESM Instance %d\n",
                    *pInstType);
#endif
    }

    pinStatus = SDR_ESM_getNErrorStatus(*pInstType);
    if (pinStatus != 1) {
        DIAG_printf("\n  timerExpPinDisable: Incorrect pin value after clearing\n");
        retVal = -1;
    }
#ifdef DEBUG_PRINT
    DIAG_printf("  timerExpPinDisable: after clear, ESM instance %d view of " \
                "MCU_SAFETY_ERRORn pin is %d\n\n",
                *pInstType, pinStatus);
#endif

    if (retVal == 0) {
        esmPinClearResult[currUseCase] = USE_CASE_STATUS_COMPLETED_SUCCESS;
    } else {
        esmPinClearResult[currUseCase] = USE_CASE_STATUS_COMPLETED_FAILURE;
    }
}

/*
* This function clears timer interrupts for Timer input events to the ESM.
*/
int32_t diag_esm_timerUpdateInt(uint32_t intEsmSrc)
{
    int32_t retVal = -1;
    TimerP_Status status;

    /* Stop/Clear timer interrupt based on which timer triggered it */
    if (intEsmSrc == CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_TIMER2_INTR_PEND_0)
    {
        status = TimerP_stop(esmHiEventTrigTimerHandle);
        if (status == TimerP_OK) {
#ifdef PRINT_DEBUG
            DIAG_printf("Cleared interrupt for Timer Triggering High " \
                        "Priority Event\n");
#endif
            retVal = 0;
        } else {
            DIAG_printf("Failed to Clear interrupt for Timer Triggering High " \
                        "Priority Event\n");
        }
    } else if (intEsmSrc == CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_TIMER3_INTR_PEND_0)
    {
        status = TimerP_stop(esmLoEventTrigTimerHandle);
        if (status == TimerP_OK) {
#ifdef PRINT_DEBUG
            DIAG_printf("Cleared interrupt for Timer Triggering Low Priority Event\n");
#endif
            retVal = 0;
        } else {
            DIAG_printf("Failed to Clear interrupt for Timer Triggering " \
                        "Low Priority Event\n");
        }
    }

    return retVal;
}

/* VTM FUNCTIONS */

/*
* This function modifies all the VTM thresholds in a single function to ensure
* appropriate number of ESM events are triggered by the change in thresholds.
*/
static void setAllVTMTempThr(CSL_vtm_adc_code lt_thr0_adc_code,
                             CSL_vtm_adc_code gt_thr1_adc_code,
                             CSL_vtm_adc_code gt_thr2_adc_code)
{
    CSL_vtm_cfg1Regs         *p_cfg = (CSL_vtm_cfg1Regs *) CSL_VTM_TEST_CFG1_BASE;
    CSL_vtm_tsThrVal         thr_val;
    CSL_vtm_vdThr_interrupt_ctrl  ctrl;
    CSL_vtm_vdEvt_sel_set    vd_temp_evts;
    CSL_vtm_tmpSens_id       ts_id = 0;

    vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_0;

    thr_val.thrValidMap = CSL_VTM_GT_TH2_VALID | \
                          CSL_VTM_GT_TH1_VALID | \
                          CSL_VTM_LT_TH0_VALID;
    ctrl                = CSL_VTM_VD_GT_THR2_INTR_EN_SET | \
                          CSL_VTM_VD_GT_THR1_INTR_EN_SET | \
                          CSL_VTM_VD_LT_THR0_INTR_EN_CLR;
    thr_val.gtTh2En     = TRUE;
    thr_val.gtTh2       = gt_thr2_adc_code;
    thr_val.gtTh1En     = TRUE;
    thr_val.gtTh1       = gt_thr1_adc_code;
    thr_val.ltTh0En     = TRUE;
    thr_val.ltTh0       = lt_thr0_adc_code;

    /* Set the temperature thresholds */
    CSL_vtmTsSetThresholds (p_cfg,  ts_id, &thr_val, FALSE);
    /* enable the threshold interrupts */
    CSL_vtmVdThrIntrCtrl(p_cfg, CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);
    /* enable the tracking of temperature events on this VD */
    CSL_vtmVdEvtSelSet (p_cfg, CSL_VTM_VD_DOMAIN_1, vd_temp_evts, FALSE);
}

/*
* This function reads the current temperature in the VTM module
* and then modifies the VTM thresholds to cause events to trigger.
* Note that in a typical system, the thresholds would be kept the static, and
* changes in temperature would cause the events to occur.
*/
static int32_t vtmTriggerTh(int32_t lt_thr0_offset,
                            int32_t gt_thr1_offset,
                            int32_t gt_thr2_offset)
{
    int32_t             retVal = 0;
    int32_t             temp_milli_degrees_read, ts_id = 0;
    CSL_vtm_adc_code    adc_code_read;
    CSL_vtm_adc_code    adc_code_lt_thr0, adc_code_gt_thr1, adc_code_gt_thr2;
    CSL_vtm_cfg1Regs   *p_vtm_cfg1_regs;
    int32_t             gt_thr2_val, gt_thr1_val, lt_thr0_val;
    CSL_vtm_tsStat_val  ts_stat_val;
    CSL_vtm_tsStat_read_ctrl tsStat_read_ctrl = CSL_VTM_TS_READ_DATA_OUT_VAL;
    CSL_vtm_vdThr_interrupt_ctrl  ctrl;

    p_vtm_cfg1_regs = (CSL_vtm_cfg1Regs *) CSL_VTM_TEST_CFG1_BASE;

    /* Get current temperature value */
    CSL_vtmTsGetSensorStat (p_vtm_cfg1_regs, &tsStat_read_ctrl, \
                                    ts_id, &ts_stat_val);
    adc_code_read = ts_stat_val.data_out;
    (void) CSL_vtmTsConvADCToTemp (adc_code_read,
                                   (CSL_vtm_tmpSens_id)     ts_id,
                                   &temp_milli_degrees_read);
#ifdef DEBUG_PRINT
    DIAG_printf ("sensor id                       : %d \n" \
                 "adc_code                        : %d \n" \
                 "temp in milli degree celcius    : %d \n", \
                 ts_id, adc_code_read, temp_milli_degrees_read);
#endif

    /* Disable interrupts while changing thresholds */
    ctrl = (CSL_VTM_VD_GT_THR2_INTR_EN_CLR | \
            CSL_VTM_VD_GT_THR2_INTR_RAW_CLR | \
            CSL_VTM_VD_GT_THR1_INTR_EN_CLR | \
            CSL_VTM_VD_GT_THR1_INTR_RAW_CLR | \
            CSL_VTM_VD_LT_THR0_INTR_EN_CLR | \
            CSL_VTM_VD_LT_THR0_INTR_RAW_CLR);
    CSL_vtmVdThrIntrCtrl((CSL_vtm_cfg1Regs *)CSL_VTM_TEST_CFG1_BASE, \
                              CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);

    /* Change all 3 thresholds relative to the current temperature */
    lt_thr0_val = temp_milli_degrees_read + lt_thr0_offset;
    gt_thr1_val = temp_milli_degrees_read + gt_thr1_offset;
    gt_thr2_val = temp_milli_degrees_read + gt_thr2_offset;

    CSL_vtmTsConvTempToAdc(lt_thr0_val, ts_id,  &adc_code_lt_thr0);
    CSL_vtmTsConvTempToAdc(gt_thr1_val, ts_id,  &adc_code_gt_thr1);
    CSL_vtmTsConvTempToAdc(gt_thr2_val, ts_id,  &adc_code_gt_thr2);

#ifdef DEBUG_PRINT
    DIAG_printf ("vtmTriggerTh: Setting lt_thr0_val temp to " \
                 "%d millidegrees Celsius, and adc_code_lt_thr0 = %d\n",
                 lt_thr0_val,
                 adc_code_lt_thr0);
    DIAG_printf ("vtmTriggerTh: Setting gt_thr1_val temp to " \
                 "%d millidegrees Celsius, and adc_code_gt_thr1 = %d\n",
                 gt_thr1_val,
                 adc_code_gt_thr1);
    DIAG_printf ("vtmTriggerTh: Setting gt_thr2_val temp to " \
                 "%d millidegrees Celsius, and adc_code_gt_thr2 = %d\n",
                 gt_thr2_val,
                 adc_code_gt_thr2);
#endif

    setAllVTMTempThr(adc_code_lt_thr0, adc_code_gt_thr1, adc_code_gt_thr2);

#ifdef DEBUG_PRINT
    DIAG_printf("Finished VTM threshold setting\n");
#endif
    return (retVal);
}

static int32_t vtmTriggerTh1(void)
{
    int32_t retVal = 0;
#ifdef DEBUG_PRINT
    DIAG_printf("vtmTriggerTh1: starting test with threshold change\n");
#endif
    retVal = vtmTriggerTh(-4000, -2000, 5000);

    return (retVal);
}

/*
* This function clears VTM THR0 interrupt (Less Than Temp event)
* for VTM input events to the ESM.
* It resets the VTM module to look for high temperature events again.
*/
void diag_esm_vtmLtThr0CrossedUpdateInt(void)
{
    CSL_vtm_vdThr_interrupt_ctrl ctrl;

    /* Ack the interrupt, by clearing the pending bit */
    ctrl = (CSL_VTM_VD_LT_THR0_INTR_EN_CLR | \
            CSL_VTM_VD_GT_THR1_INTR_EN_SET | \
            CSL_VTM_VD_GT_THR2_INTR_EN_SET | \
            CSL_VTM_VD_LT_THR0_INTR_RAW_CLR | \
            CSL_VTM_VD_GT_THR1_INTR_RAW_CLR | \
            CSL_VTM_VD_GT_THR2_INTR_RAW_CLR);

    CSL_vtmVdThrIntrCtrl((CSL_vtm_cfg1Regs *)CSL_VTM_TEST_CFG1_BASE, \
                          CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);

    /* Print output ESM event to the screen */
#ifdef PRINT_EVENTS
    DIAG_printf ("\n    Got ltThr0 interrupt through ESM module\n");
    DIAG_printf ("\n    System at a temperature below the threshold of lt_thr0 \n"
                 "    System running at a safe temperature \n");
#endif
}

/*
* This function clears VTM THR1 interrupts for VTM input event to the ESM.
*/
void diag_esm_vtmGtThr1CrossedUpdateInt(void)
{
    CSL_vtm_vdThr_interrupt_ctrl ctrl;
    /*
    - disable the gt1 interrupt
    - clear the gt1 interrupt
    - clear the lt0 interrupt
    - enable the lt0 intterupt
    */
    ctrl = (CSL_VTM_VD_GT_THR1_INTR_EN_CLR  |  \
            CSL_VTM_VD_GT_THR1_INTR_RAW_CLR |  \
            CSL_VTM_VD_LT_THR0_INTR_EN_SET  |  \
            CSL_VTM_VD_LT_THR0_INTR_RAW_CLR);

    /* Ack and Re-enable the LT_THR0 interrupt to let system know if cooling
     * took place */
    CSL_vtmVdThrIntrCtrl((CSL_vtm_cfg1Regs *)CSL_VTM_TEST_CFG1_BASE, \
                          CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);

    /* Print output ESM event to the screen */

#ifdef PRINT_EVENTS
    DIAG_printf ("\n    Got gtThr1 interrupt through ESM module\n");
    DIAG_printf ("\n    Crossed early warning threshold of gt_thr1 \n"
                 "    System should take action to implement system cooling \n");
#endif
}

/*
* This function clears VTM THR2 interrupts for VTM input event to the ESM.
*/
void diag_esm_vtmGtThr2CrossedUpdateInt(void)
{
    CSL_vtm_vdThr_interrupt_ctrl ctrl;
    /* Ack the interrupt, by clearing the pending bit */
    ctrl = (CSL_VTM_VD_GT_THR2_INTR_EN_CLR | \
            CSL_VTM_VD_GT_THR2_INTR_RAW_CLR);
    CSL_vtmVdThrIntrCtrl((CSL_vtm_cfg1Regs *)CSL_VTM_TEST_CFG1_BASE, \
                              CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);

#ifdef PRINT_EVENTS
    /* Print output ESM event to the screen */
    DIAG_printf ("\n    Got gtThr2 interrupt through ESM module\n");
    DIAG_printf ("\n    Crossed critical threshold of gt_thr2 \n"
                 "    System should take critical action to implement system cooling \n");
#endif
}

/*
* This function starts timer for clearing the MCU_SAFETY_ERRORn pin.
* When this timer expires, the pin clearing function will be initiated.
*/
void diag_esm_startPinClearTimer(bool forcePinClear)
{
    /* Start timer to clear the pin at a certain time */
    TimerP_Status   timerStatus;

    immediatePinClear = forcePinClear;

    timerStatus = TimerP_start(esmPinTimerHandle);
    if (timerStatus != TimerP_OK) {
        DIAG_printf("ERR: Could not start the timer %d \n",
                    PIN_TIMING_TIMER_ID);
    } else {
#ifdef PRINT_DEBUG
        DIAG_printf("Started timer to simulate MCU addressing error\n");
#endif
    }

}

/*
* This function prints the current VTM thresholds.
*/
int32_t diag_esm_printCurrentThresholds(void)
{
    int32_t retVal = 0;

    CSL_vtm_cfg1Regs *p_vtm_cfg1_regs = (CSL_vtm_cfg1Regs *) CSL_VTM_TEST_CFG1_BASE;
    CSL_vtm_tsThrVal  tsThrVal;

    tsThrVal.thrValidMap = CSL_VTM_GT_TH1_VALID | \
                           CSL_VTM_GT_TH2_VALID | \
                           CSL_VTM_LT_TH0_VALID;

    retVal = CSL_vtmTsGetThresholds (p_vtm_cfg1_regs,
                                     CSL_VTM_TS_ID_0,
                                     &tsThrVal);

#ifdef PRINT_EVENTS
    DIAG_printf("Current VTM Thresholds: lt_thr0 = %d, gt_thr1 = %d, gt_thr2 = %d\n",
                tsThrVal.ltTh0, tsThrVal.gtTh1, tsThrVal.gtTh2);
#endif
    return (retVal);
}

int32_t diag_esm_setThresholdsForCriticalTrigger(void)
{
    int32_t retVal = 0;
#ifdef DEBUG_PRINT
    DIAG_printf("diag_esm_setThresholdsForCriticalTrigger: setting thresholds " \
                "to trigger high priority thermal event.\n");
#endif
    retVal = vtmTriggerTh(-12000, -8000, -3000);

    return (retVal);
}

/*
* This function resets the VTM thresholds back to some typical default
* values.
*/
int32_t diag_esm_setNormalThresholds(void)
{
    int32_t          retVal = 0;
    int32_t          ts_id  = 0;
    CSL_vtm_adc_code adc_code_lt_thr0, adc_code_gt_thr1, adc_code_gt_thr2;
    int32_t          gt_thr2_val, gt_thr1_val, lt_thr0_val;
    CSL_vtm_vdThr_interrupt_ctrl  ctrl;

#ifdef DEBUG_PRINT
    DIAG_printf("Start changing to normal VTM threshold setting\n");
#endif

    /* Disable interrupts while changing thresholds */
    ctrl = (CSL_VTM_VD_GT_THR2_INTR_EN_CLR | \
            CSL_VTM_VD_GT_THR2_INTR_RAW_CLR | \
            CSL_VTM_VD_GT_THR1_INTR_EN_CLR | \
            CSL_VTM_VD_GT_THR1_INTR_RAW_CLR | \
            CSL_VTM_VD_LT_THR0_INTR_EN_CLR | \
            CSL_VTM_VD_LT_THR0_INTR_RAW_CLR);
    CSL_vtmVdThrIntrCtrl((CSL_vtm_cfg1Regs *)CSL_VTM_TEST_CFG1_BASE, \
                              CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);

    /* Set to default values */
    lt_thr0_val = LT_THR0_DEFAULT;
    gt_thr1_val = GT_THR1_DEFAULT;
    gt_thr2_val = GT_THR2_DEFAULT;

    CSL_vtmTsConvTempToAdc(lt_thr0_val, ts_id, &adc_code_lt_thr0);
    CSL_vtmTsConvTempToAdc(gt_thr1_val, ts_id, &adc_code_gt_thr1);
    CSL_vtmTsConvTempToAdc(gt_thr2_val, ts_id, &adc_code_gt_thr2);

#ifdef DEBUG_PRINT
    DIAG_printf ("diag_esm_setNormalThresholds: Setting lt_thr0_val temp to %d " \
                 "millidegrees Celsius, and adc_code_lt_thr0 = %d\n",
                 lt_thr0_val,
                 adc_code_lt_thr0);
    DIAG_printf ("diag_esm_setNormalThresholds: Setting gt_thr1_val temp to %d " \
                 "millidegrees Celsius, and adc_code_gt_thr1 = %d\n",
                 gt_thr1_val,
                 adc_code_gt_thr1);
    DIAG_printf ("diag_esm_setNormalThresholds: Setting gt_thr2_val temp to %d " \
                 "millidegrees Celsius, and adc_code_gt_thr2 = %d\n",
                 gt_thr2_val,
                 adc_code_gt_thr2);
#endif

    setAllVTMTempThr(adc_code_lt_thr0, adc_code_gt_thr1, adc_code_gt_thr2);

#ifdef DEBUG_PRINT
    DIAG_printf("Finished normal VTM threshold setting\n");
#endif

    return (retVal);
}

/* USE CASE FUNCTIONS */

/*
* This function initiates the input trigger event for each use case
*/
int32_t diag_esm_runUseCaseTrigger(uint8_t useCaseId)
{
    TimerP_Status timerStatus;
    int32_t       retVal = 0;

    DIAG_printf("\nStarting Use Case %d \n", useCaseId);
    switch(useCaseId)
    {
        case 0:
            /* UC-1: Low Priority interrupt on WKUP ESM -
             * VTM greater than THR1 */
            currEsmInstance = SDR_ESM_INSTANCE_WKUP;
            retVal = vtmTriggerTh1();
            if (retVal == 0) {
                esmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            } else {
                esmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
            }
            /* This use case only has low priority interrupts not routed to
             * the pin, so this flag can be set at beginning of the test */
            esmPinClearResult[0] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            break;

        case 1:
            /* UC-2: High Priority interrupt on WKUP ESM -
             * VTM greater than THR2, no clearing of
             * MCU_SAFETY_ERRORn pin */
            currEsmInstance = SDR_ESM_INSTANCE_WKUP;
            retVal = vtmTriggerTh1();
            if (retVal == 0) {
                esmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            } else {
                esmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
            }
            break;

        case 2:
            /* UC-3: High Priority interrupt on WKUP ESM -
             * VTM greater than THR2 with clearing
             * of MCU_SAFETY_ERRORn pin */

            /* Start the Pin Control and Measurement Timer */
            currEsmInstance = SDR_ESM_INSTANCE_WKUP;
            retVal = vtmTriggerTh1();
            if (retVal == 0) {
                esmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            } else {
                esmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
            }
            break;

        case 3:
            /* UC-4: Low Priority interrupt on MCU ESM -
             * MCU TIMER 2 expiration */
            currEsmInstance = SDR_ESM_INSTANCE_MCU;
            /* Start the Low Priority Event Trigger Timer */
            timerStatus = TimerP_start(esmLoEventTrigTimerHandle);
            if (timerStatus != TimerP_OK) {
                DIAG_printf("ERR: Could not start the timer %d \n",
                            LOW_PRIO_TRIG_TIMER_ID);
                retVal = -1;
            }
            /* esmEventInputTrig flag is set in expiration of the timer */
            /* This use case only has low priority interrupts not routed to
             * the pin, so this flag can be set at beginning of the test */
            esmPinClearResult[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            break;
    
        case 4:
            /* UC-5: High Priority interrupt on MCU ESM -
             * MCU TIMER 1 expiration */
            /* Start the High Priority Event Trigger Timer */
            currEsmInstance = SDR_ESM_INSTANCE_MCU;
            timerStatus = TimerP_start(esmHiEventTrigTimerHandle);
            if (timerStatus != TimerP_OK) {
                DIAG_printf("ERR: Could not start the timer %d \n",
                            HIGH_PRIO_TRIG_TIMER_ID);
                esmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
                retVal = -1;
            }
            /* esmEventInputTrig flag is set in expiration of the Hi Event timer */
            /* esmPinClearResult flag is set in expiration of the Pin timer */
            break;

        default:
            DIAG_printf("ERR: Invalid Use Case ID %d \n", useCaseId);
            retVal = -1;
            break;
    }

    return (retVal);
}

