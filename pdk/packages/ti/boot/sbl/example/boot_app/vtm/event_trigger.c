/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \file     event_trigger.c
 *
 *  \brief    This file contains functions that provide input event triggers
 *            for the Voltage and Thermal Monitor (VTM) application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/sciclient/sciclient.h>
#include <include/soc.h>
#include <sdl_esm.h>
#include "vtm.h"
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <src/ip/sdl_ip_vtm.h>
#include <sdl_vtm.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define LT_THR0_DEFAULT          (95000)
#define GT_THR1_DEFAULT          (105000)
#define GT_THR2_DEFAULT          (115000)

/* ========================================================================== */
/*                    Internal Function Declarations                          */
/* ========================================================================== */

static int32_t BootApp_vtmTriggerTh(int32_t lt_thr0_offset,
                            int32_t gt_thr1_offset,
                            int32_t gt_thr2_offset);
static int32_t BootApp_vtmConfig(void);
static void BootApp_setAllVTMTempThr(SDL_VTM_adc_code lt_thr0_adc_code,
                             SDL_VTM_adc_code gt_thr1_adc_code,
                             SDL_VTM_adc_code gt_thr2_adc_code);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Completion of Use Case from Input trigger perspective updates these flags */
extern volatile uint32_t    vtmEventInputTrig[2];
/* Current Use case being run */
extern volatile uint8_t     currTestCase;
uint32_t                    pStatus;
SDL_ESM_Inst                currEsmInstance;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
* This function modifies all the VTM thresholds in a single function to ensure
* appropriate number of ESM events are triggered by the change in thresholds.
*/
static void BootApp_setAllVTMTempThr(SDL_VTM_adc_code lt_thr0_adc_code,
                             SDL_VTM_adc_code gt_thr1_adc_code,
                             SDL_VTM_adc_code gt_thr2_adc_code)
{

    SDL_VTM_tsThrVal        thr_val;
    SDL_VTM_intrCtrl        ctrl;
    SDL_VTM_configTs cfgTs;
    thr_val.thrValidMap = SDL_VTM_GT_TH1_VALID | \
                          SDL_VTM_GT_TH2_VALID | \
                          SDL_VTM_LT_TH0_VALID;
    thr_val.gtTh2En     = TRUE;
    thr_val.gtTh2       = gt_thr2_adc_code;
    thr_val.gtTh1En     = TRUE;
    thr_val.gtTh1       = gt_thr1_adc_code;
    thr_val.ltTh0En     = TRUE;
    thr_val.ltTh0       = lt_thr0_adc_code;
    cfgTs.thr_val       = thr_val;
    cfgTs.tsCtrl_cfg.valid_map = 0;
    cfgTs.tsCtrl_cfg.maxt_outrg_alert_en = 0;
    cfgTs.tsCtrl_cfg.tsReset = 0;
    cfgTs.tsCtrl_cfg.adc_stat = 0;
    cfgTs.tsCtrl_cfg.mode = 0;
    cfgTs.configTsCtrl = SDL_VTM_VD_CONFIG_CTRL_SET_THR;

    ctrl    = SDL_VTM_VD_GT_THR2_INTR_EN_SET | \
              SDL_VTM_VD_GT_THR1_INTR_EN_SET | \
              SDL_VTM_VD_LT_THR0_INTR_EN_CLR;

    SDL_VTM_configVd cfgVd;
    cfgVd.configVdCtrl = SDL_VTM_VD_CONFIG_CTRL_EVT_SEL;
    cfgVd.vd_temp_evts = SDL_VTM_VD_EVT_SELECT_TEMP_SENSOR_0;
    SDL_VTM_tsGlobal_cfg   tsGlobalCfg;
    tsGlobalCfg.validMap = 0;
    tsGlobalCfg.clkSel = 0;
    tsGlobalCfg.clkDiv = 0;
    tsGlobalCfg.any_maxt_outrg_alert_en = 0;
    tsGlobalCfg.maxt_outrg_alert_thr0 = 0;
    tsGlobalCfg.maxt_outrg_alert_thr = 0;
    tsGlobalCfg.samplesPerCnt = 0;
    cfgVd.tsGlobal_cfg = tsGlobalCfg;
    /* Set the temperature thresholds */
    SDL_VTM_initTs(SDL_VTM_INSTANCE_TS_0 , &cfgTs);

    /* enable the threshold interrupts */
    SDL_VTM_intrCntrl(SDL_VTM_INSTANCE_VD_DOMAIN_1, ctrl);

    /* enable the tracking of temperature events on this VD */
    SDL_VTM_initVd(SDL_VTM_INSTANCE_VD_DOMAIN_1, &cfgVd);
}

/*
* This function reads the current temperature in the VTM module
* and then modifies the VTM thresholds to cause events to trigger.
* Note that in a typical system, the thresholds would be kept the static, and
* changes in temperature would cause the events to occur.
*/
static int32_t BootApp_vtmTriggerTh(int32_t lt_thr0_offset,
                            int32_t gt_thr1_offset,
                            int32_t gt_thr2_offset)
{
    int32_t             retVal = 0;
    int32_t             temp_milli_degrees_read;
    SDL_VTM_InstTs      insTs = SDL_VTM_INSTANCE_TS_0;
    SDL_VTM_adc_code    adc_code_read;
    SDL_VTM_adc_code    adc_code_lt_thr0 = 0, adc_code_gt_thr1 = 0, adc_code_gt_thr2 = 0;
    int32_t             gt_thr2_val, gt_thr1_val, lt_thr0_val;
    SDL_VTM_intrCtrl  ctrl;
    SDL_VTM_Stat_val statusVal;
    SDL_VTM_Stat_read_ctrl readCtrl;
    SDL_VTM_configTs cfgTs;
    readCtrl = SDL_VTM_TS_READ_DATA_OUT_VAL;

    /* Set temp sensor for continuous mode */
    cfgTs.configTsCtrl = SDL_VTM_VD_CONFIG_CTRL_SET_CTL;
    cfgTs.tsCtrl_cfg.valid_map = SDL_VTM_TS_CTRL_MODE_VALID;
    cfgTs.tsCtrl_cfg.mode = SDL_VTM_TS_CTRL_CONTINUOUS_MODE;
    SDL_VTM_initTs(SDL_VTM_INSTANCE_TS_0 , &cfgTs);

    /* Get current temperature value */
    SDL_VTM_getSensorStatus(insTs, &readCtrl, &statusVal);
    adc_code_read = statusVal.data_out;
    (void) SDL_VTM_tsConvADCToTemp (adc_code_read, insTs, \
                                   &temp_milli_degrees_read);

    /* Disable interrupts while changing thresholds */
    ctrl = (SDL_VTM_VD_GT_THR2_INTR_EN_CLR | \
            SDL_VTM_VD_GT_THR2_INTR_RAW_CLR | \
            SDL_VTM_VD_GT_THR1_INTR_EN_CLR | \
            SDL_VTM_VD_GT_THR1_INTR_RAW_CLR | \
            SDL_VTM_VD_LT_THR0_INTR_EN_CLR | \
            SDL_VTM_VD_LT_THR0_INTR_RAW_CLR);
    SDL_VTM_intrCntrl(SDL_VTM_INSTANCE_VD_DOMAIN_1, ctrl);

    /* Change all 3 thresholds relative to the current temperature */
    lt_thr0_val = temp_milli_degrees_read + lt_thr0_offset;
    gt_thr1_val = temp_milli_degrees_read + gt_thr1_offset;
    gt_thr2_val = temp_milli_degrees_read + gt_thr2_offset;

    SDL_VTM_tsConvTempToAdc(lt_thr0_val, insTs,  &adc_code_lt_thr0);
    SDL_VTM_tsConvTempToAdc(gt_thr1_val, insTs,  &adc_code_gt_thr1);
    SDL_VTM_tsConvTempToAdc(gt_thr2_val, insTs,  &adc_code_gt_thr2);

    BootApp_setAllVTMTempThr(adc_code_lt_thr0, adc_code_gt_thr1, adc_code_gt_thr2);

    return (retVal);
}

static int32_t BootApp_vtmConfig(void)
{
    int32_t retVal = 0;

    retVal = BootApp_vtmTriggerTh(-4000, -2000, 5000);

    return (retVal);
}

/*
* This function clears VTM THR0 interrupt (Less Than Temp event)
* for VTM input events to the ESM.
* It resets the VTM module to look for high temperature events again.
*/
void BootApp_vtmIntrruptLtThr0(void)
{
    SDL_VTM_intrCtrl ctrl;

    /* Ack the interrupt, by clearing the pending bit */
    ctrl = (SDL_VTM_VD_LT_THR0_INTR_EN_CLR | \
            SDL_VTM_VD_GT_THR1_INTR_EN_SET | \
            SDL_VTM_VD_GT_THR2_INTR_EN_SET | \
            SDL_VTM_VD_LT_THR0_INTR_RAW_CLR | \
            SDL_VTM_VD_GT_THR1_INTR_RAW_CLR | \
            SDL_VTM_VD_GT_THR2_INTR_RAW_CLR);
    SDL_VTM_intrCntrl(SDL_VTM_INSTANCE_VD_DOMAIN_1, ctrl);
    /* Print output ESM event to the screen */
    UART_printf ("\n    Got ltThr0 interrupt through ESM module\n");
    UART_printf ("\n    System at a temperature below the threshold of lt_thr0 \n"
                 "    System running at a safe temperature \n");
}

/*
* This function clears VTM THR1 interrupts for VTM input event to the ESM.
*/
void BootApp_vtmIntrruptGtThr1(void)
{
    SDL_VTM_intrCtrl ctrl;
    /*
    - disable the gt1 interrupt
    - clear the gt1 interrupt
    - clear the lt0 interrupt
    - enable the lt0 intterupt
    */
    ctrl = (SDL_VTM_VD_GT_THR1_INTR_EN_CLR  |  \
            SDL_VTM_VD_GT_THR1_INTR_RAW_CLR |  \
            SDL_VTM_VD_LT_THR0_INTR_EN_SET  |  \
            SDL_VTM_VD_LT_THR0_INTR_RAW_CLR);

    /* Ack and Re-enable the LT_THR0 interrupt to let system know if cooling
     * took place */
    SDL_VTM_intrCntrl(SDL_VTM_INSTANCE_VD_DOMAIN_1, ctrl);

    /* Print output ESM event to the screen */
    UART_printf ("\n    Got gtThr1 interrupt through ESM module\n");
    UART_printf ("\n    Crossed early warning threshold of gt_thr1 \n"
                 "    System should take action to implement system cooling \n");
}

/*
* This function clears VTM THR2 interrupts for VTM input event to the ESM.
*/
void BootApp_vtmIntrruptGtThr2(void)
{
    SDL_VTM_intrCtrl ctrl;
    /* Ack the interrupt, by clearing the pending bit */
    ctrl = (SDL_VTM_VD_GT_THR2_INTR_EN_CLR | \
            SDL_VTM_VD_GT_THR2_INTR_RAW_CLR);
    SDL_VTM_intrCntrl(SDL_VTM_INSTANCE_VD_DOMAIN_1, ctrl);

    /* Print output ESM event to the screen */
    UART_printf ("\n    Got gtThr2 interrupt through ESM module\n");
    UART_printf ("\n    Crossed critical threshold of gt_thr2 \n"
                 "    System should take critical action to implement system cooling \n");
}


int32_t BootApp_vtmSetThresholdsForCriticalTrigger(void)
{
    int32_t retVal = 0;

    retVal = BootApp_vtmTriggerTh(-12000, -8000, -3000);

    return (retVal);
}

/*
* This function resets the VTM thresholds back to some typical default
* values.
*/
int32_t BootApp_vtmSetNormalThresholds(void)
{
    int32_t          retVal = 0;
    SDL_VTM_InstTs insTs = SDL_VTM_INSTANCE_TS_0;
    SDL_VTM_adc_code adc_code_lt_thr0 = 0, adc_code_gt_thr1 = 0, adc_code_gt_thr2 = 0;
    int32_t          gt_thr2_val, gt_thr1_val, lt_thr0_val;
    SDL_VTM_intrCtrl  ctrl;

    /* Disable interrupts while changing thresholds */
    ctrl = (SDL_VTM_VD_GT_THR2_INTR_EN_CLR | \
            SDL_VTM_VD_GT_THR2_INTR_RAW_CLR | \
            SDL_VTM_VD_GT_THR1_INTR_EN_CLR | \
            SDL_VTM_VD_GT_THR1_INTR_EN_CLR | \
            SDL_VTM_VD_LT_THR0_INTR_EN_CLR | \
            SDL_VTM_VD_LT_THR0_INTR_RAW_CLR);

    SDL_VTM_intrCntrl(SDL_VTM_INSTANCE_VD_DOMAIN_1, ctrl);

    /* Set to default values */
    lt_thr0_val = LT_THR0_DEFAULT;
    gt_thr1_val = GT_THR1_DEFAULT;
    gt_thr2_val = GT_THR2_DEFAULT;

    SDL_VTM_tsConvTempToAdc(lt_thr0_val, insTs, &adc_code_lt_thr0);
    SDL_VTM_tsConvTempToAdc(gt_thr1_val, insTs, &adc_code_gt_thr1);
    SDL_VTM_tsConvTempToAdc(gt_thr2_val, insTs, &adc_code_gt_thr2);

    BootApp_setAllVTMTempThr(adc_code_lt_thr0, adc_code_gt_thr1, adc_code_gt_thr2);

    return (retVal);
}

/* USE CASE FUNCTIONS */

/*
* This function initiates the input trigger event for each use case
*/
int32_t BootApp_vtmRunTestCaseTrigger(uint8_t useCaseId)
{
    int32_t       retVal = 0;
    UART_printf("\nStarting Use Case %d \n", useCaseId);
    switch(useCaseId)
    {
        case 0:
            /* UC-1: Low Priority interrupt on WKUP ESM -
             * VTM less than THR1 */
            currEsmInstance = SDL_ESM_INST_WKUP_ESM0;
            retVal = BootApp_vtmConfig();
            if (retVal == 0) {
                UART_printf("case 0 success\n");
                vtmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            } else{
                UART_printf("case 0 failure\n");
                vtmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
            }
            break;

        case 1:
            /* UC-2: High Priority interrupt on WKUP ESM -
             * VTM greater than THR2 with clearing
             * of MCU_SAFETY_ERRORn pin */

            /* Start the Pin Control and Measurement Timer */
            currEsmInstance = SDL_ESM_INST_WKUP_ESM0;
            retVal = BootApp_vtmConfig();
            if (retVal == 0) {
                vtmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            } else {
                vtmEventInputTrig[useCaseId] = USE_CASE_STATUS_COMPLETED_FAILURE;
            }
            break;

        default:
            UART_printf("ERR: Invalid Test Case ID %d \n", useCaseId);
            retVal = -1;
            break;
    }

    return (retVal);
}

/* Nothing past this point */