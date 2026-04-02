/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2022
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
 *  \file     vtm_sensor_temp_alert.c
 *
 *  \brief    This file contains VTM PVT sensor thermal alert example application
 *            Implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "vtm_sensor_temp_alert.h"
#include <stdint.h>

#define CSL_VTM_TEST_NUM_EFUSE_REGS                              (4u)
#define CSL_VTM_TEST_EFUSE_BASE_ADDR                             (0x43000300UL)
#define CSL_VTM_TEST_NUM_SENSORS                                 (5u)
#define CSL_VTM_TEST_THERMAL_RST_MASK                            (1 << 24)

#if defined(SOC_J721E) || defined(SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#define CSL_VTM_TEST_CFG1_BASE          (CSL_WKUP_VTM0_MMR_VBUSP_CFG1_BASE)
#define CSL_VTM_TEST_CFG2_BASE          (CSL_WKUP_VTM0_MMR_VBUSP_CFG2_BASE)
#elif defined (SOC_AM64X)
#define CSL_VTM_TEST_CFG1_BASE          (CSL_VTM0_MMR_VBUSP_CFG1_BASE)
#define CSL_VTM_TEST_CFG2_BASE          (CSL_VTM0_MMR_VBUSP_CFG2_BASE)
#else
/* Address needs to be defined */
#endif


/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
volatile uint32_t g_efuse_0, gefuse_1, gefuse_2, gefuse_3;
volatile Bool     g_all_thr_alerts_fired = FALSE, g_lt_thr0_alert_fired = FALSE;
volatile Bool     g_gt_thr1_alert_fired = FALSE, g_gt_thr2_alert_fired = FALSE;
volatile int32_t  g_current_temp =0;
Bool gAuto_test = (Bool) TEST_THR_AUTO_TEST_FOR_RTOS_NIGHTLY;
HwiP_Handle       gHwiP_handle[4];

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* Unity functions */
void test_csl_vtm_pvt_sensor_temp_alert_runner(void);
void test_csl_vtm_pvt_sensor_temp_alert (void);

/* Threshold interrupt service routines */
void test_csl_vtm_pvt_sensor_ltThr0_isr(uintptr_t arg);
void test_csl_vtm_pvt_sensor_gtThr1_isr(uintptr_t arg);
void test_csl_vtm_pvt_sensor_gtThr2_isr(uintptr_t arg);
void test_csl_register_temp_thr_isrs(uintptr_t isr, uintptr_t arg, int32_t thr_code);
void test_csl_unregister_temp_thr_isrs(int32_t thr_code);
void test_csl_set_temp_thr(CSL_vtm_adc_code adc_code, CSL_vtm_tmpSens_id ts_id, int32_t thr_code);



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void test_csl_vtm_pvt_sensor_ltThr0_isr(uintptr_t arg)
{
    g_lt_thr0_alert_fired = TRUE;
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

    UART_printf (" \n\n\ngot ltThr0 interrupt \n");
    UART_printf (" \nSystem at a temperature below the threshold of %d \nSystem running at a safe temperature \n", (int32_t) arg);
}

void test_csl_vtm_pvt_sensor_gtThr1_isr(uintptr_t arg)
{
    g_gt_thr1_alert_fired = TRUE;
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

    /* Ack and Re-enable the LT_THR0 interrupt to let system know if cooling took place */
    CSL_vtmVdThrIntrCtrl((CSL_vtm_cfg1Regs *)CSL_VTM_TEST_CFG1_BASE, \
                          CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);
    UART_printf (" \n\n\ngot gtThr1 interrupt \n");
    UART_printf (" \nCrossed threshold %d  System should take action to implement system cooling \n", (int32_t) arg);
   
}

void test_csl_vtm_pvt_sensor_gtThr2_isr(uintptr_t arg)
{
    g_gt_thr2_alert_fired = TRUE;
    CSL_vtm_vdThr_interrupt_ctrl ctrl;
    /* Ack the interrupt, by clearing the pending bit */
    ctrl = (CSL_VTM_VD_GT_THR2_INTR_EN_CLR | \
            CSL_VTM_VD_GT_THR2_INTR_RAW_CLR);
    CSL_vtmVdThrIntrCtrl((CSL_vtm_cfg1Regs *)CSL_VTM_TEST_CFG1_BASE, \
                              CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);
    UART_printf (" \n\n\ngot gtThr2 interrupt \n");
    UART_printf (" \nCrossed threshold %d  System should take critical action to implement system cooling \n", (int32_t) arg);
}

#ifdef UNITY_INCLUDE_CONFIG_H
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}
#endif

void test_csl_set_temp_thr(CSL_vtm_adc_code adc_code, CSL_vtm_tmpSens_id ts_id, int32_t thr_code)
{
    CSL_vtm_cfg1Regs         *p_cfg = (CSL_vtm_cfg1Regs *) CSL_VTM_TEST_CFG1_BASE;
    CSL_vtm_tsThrVal         thr_val;
    CSL_vtm_vdThr_interrupt_ctrl  ctrl;
    CSL_vtm_vdEvt_sel_set   vd_temp_evts;

    switch (ts_id)
    {
        case CSL_VTM_TS_ID_0:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_0;
            break;
        case CSL_VTM_TS_ID_1:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_1;
            break;
        case CSL_VTM_TS_ID_2:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_2;
            break;
        case CSL_VTM_TS_ID_3:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_3;
            break;            
        case CSL_VTM_TS_ID_4:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_4;
            break;
        case CSL_VTM_TS_ID_5:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_5;
            break;
        case CSL_VTM_TS_ID_6:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_6;
            break;          
        case CSL_VTM_TS_ID_7:
        default:
            vd_temp_evts =  CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_7;
            break;            
    }

    /* Program VTM  enabling the gt1 and gt2 interrupts, but disabling the lt0_int */
    switch (thr_code)
    {
        case TEST_CSL_VTM_GT_THR2_SET:
            thr_val.thrValidMap = CSL_VTM_GT_TH2_VALID;
            thr_val.gtTh2En     = TRUE;
            thr_val.gtTh2       = adc_code;
            ctrl                = CSL_VTM_VD_GT_THR2_INTR_EN_SET;
            break;
        case TEST_CSL_VTM_GT_THR1_SET:
            thr_val.thrValidMap = CSL_VTM_GT_TH1_VALID;
            thr_val.gtTh1En     = TRUE;
            thr_val.gtTh1       = adc_code;
            ctrl                = CSL_VTM_VD_GT_THR1_INTR_EN_SET;
            break;
        case TEST_CSL_VTM_LT_THR0_SET:
        default:
            thr_val.thrValidMap = CSL_VTM_LT_TH0_VALID;
            thr_val.ltTh0En     = TRUE;
            thr_val.ltTh0       = adc_code;
            ctrl                = CSL_VTM_VD_LT_THR0_INTR_EN_CLR;
            break;
    }

    /* Set the temperature thresholds */
    CSL_vtmTsSetThresholds (p_cfg,  ts_id, &thr_val, FALSE);
    /* enable the threshold interrupts */
    CSL_vtmVdThrIntrCtrl(p_cfg, CSL_VTM_VD_DOMAIN_1, ctrl, FALSE);
    /* enable the tracking of temperature events on this VD */
    CSL_vtmVdEvtSelSet (p_cfg, CSL_VTM_VD_DOMAIN_1, vd_temp_evts, FALSE);

}

void test_csl_register_temp_thr_isrs(uintptr_t isr, uintptr_t arg, int32_t thr_code)
{
    OsalRegisterIntrParams_t intr_params;

    Osal_RegisterInterrupt_initParams(&intr_params);

    switch (thr_code)
    {
        case TEST_CSL_VTM_GT_THR2_SET:
            intr_params.corepacConfig.isrRoutine = (Osal_IsrRoutine) isr;
            intr_params.corepacConfig.intVecNum  = TEST_CSL_VTM_GT_THR2_INTR_NUM;
            break;
        case TEST_CSL_VTM_GT_THR1_SET:
            intr_params.corepacConfig.isrRoutine = (Osal_IsrRoutine) isr;
            intr_params.corepacConfig.intVecNum  = TEST_CSL_VTM_GT_THR1_INTR_NUM;
            break;
        case TEST_CSL_VTM_LT_THR0_SET:
        default:
            thr_code = TEST_CSL_VTM_LT_THR0_SET;
            intr_params.corepacConfig.isrRoutine = (Osal_IsrRoutine) isr;
            intr_params.corepacConfig.intVecNum  = TEST_CSL_VTM_LT_THR0_INTR_NUM;
            break;
    }

#if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') /* R5F */
        intr_params.corepacConfig.priority=0x8U;
#else
#ifdef BUILD_C7X
        intr_params.corepacConfig.priority=0x01U;
#else
        intr_params.corepacConfig.priority=0x20U;
#endif
#endif

    intr_params.corepacConfig.arg = arg;
    Osal_RegisterInterrupt(&intr_params, &gHwiP_handle[thr_code]);
}

void test_csl_unregister_temp_thr_isrs(int32_t thr_code)
{
    int32_t intr_num;
    switch (thr_code)
    {
        case TEST_CSL_VTM_GT_THR1_SET:
            intr_num = TEST_CSL_VTM_GT_THR1_INTR_NUM;
            break;
        case TEST_CSL_VTM_GT_THR2_SET:
            intr_num = TEST_CSL_VTM_GT_THR2_INTR_NUM;
            break;
        case TEST_CSL_VTM_LT_THR0_SET:
        default:
            intr_num = TEST_CSL_VTM_LT_THR0_INTR_NUM;
            thr_code = TEST_CSL_VTM_LT_THR0_SET;
            break;
    }

    Osal_DeleteInterrupt(&gHwiP_handle[thr_code], intr_num);
}

void test_csl_vtm_pvt_sensor_temp_alert (void)
{
    int32_t temp_milli_degrees_read, ts_id;
    int32_t tmp_value = 0x2342;

    CSL_vtm_adc_code       adc_code;
    CSL_vtm_cfg1Regs       *p_vtm_cfg1_regs;
    CSL_vtm_cfg2Regs       *p_vtm_cfg2_regs;
    CSL_vtm_tsStat_read_ctrl tsStat_read_ctrl = CSL_VTM_TS_READ_DATA_OUT_VAL;
    CSL_vtm_tsStat_val       ts_stat_val;
    CSL_vtm_ts_ctrl_cfg     ts_ctrl_cfg;
    int32_t                 high_temp_in_milli_degree_celsius = 123000;
    int32_t                 low_temp_in_milli_degree_celsius  = 105000;
    int32_t                 lt_thr0_val, gt_thr1_val, gt_thr2_val, max_temp_read;
    int32_t                 i; 
    int32_t                 a = 1234, b =2352, c =232; /* Random val initializations */
    uint32_t               *efuse_base_addr = (uint32_t *)  CSL_VTM_TEST_EFUSE_BASE_ADDR;

    p_vtm_cfg1_regs = (CSL_vtm_cfg1Regs *) CSL_VTM_TEST_CFG1_BASE;
    p_vtm_cfg2_regs = (CSL_vtm_cfg2Regs *) CSL_VTM_TEST_CFG2_BASE;
    lt_thr0_val     = TEST_CSL_VTM_LT_THR0_VAL;
    gt_thr1_val     = TEST_CSL_VTM_GT_THR1_VAL;
    gt_thr2_val     = TEST_CSL_VTM_GT_THR2_VAL;

    /* Setup the Efuse values if needed, to simulate an efuse part */
#if defined (CSL_VTM_SS_UNIT) || defined (CSL_VTM_TT_UNIT) || defined (CSL_VTM_FF_UNIT)
    efuse_base_addr[0] = CSL_VTM_VAL_EFUSE_0;
    efuse_base_addr[1] = CSL_VTM_VAL_EFUSE_1;
    efuse_base_addr[2] = CSL_VTM_VAL_EFUSE_2;    
    efuse_base_addr[3] = CSL_VTM_VAL_EFUSE_3;
#endif

    UART_printf("\nVTM threshold test Application version 1.0.0.0 !!\n");

    UART_printf (" efuse 0  is : 0x%x \n \
               efuse 1 is : 0x%x \n \
               efuse 2  is : 0x%x \n \
               efuse 3  is : 0x%x \n", \
               efuse_base_addr[0], \
               efuse_base_addr[1], \
               efuse_base_addr[2], \
               efuse_base_addr[3]);



    for (ts_id = 0; ts_id < TEST_MAX_SENSOR_FOR_VTM_ALERT; ts_id++)
    {

        ts_ctrl_cfg.valid_map = CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID   |
                                CSL_VTM_TS_CTRL_RESET_CTRL_VALID        |
                                CSL_VTM_TS_CTRL_SOC_VALID               |
                                CSL_VTM_TS_CTRL_MODE_VALID;

        (void) CSL_vtmTsGetCtrl (p_vtm_cfg2_regs,
                         ts_id,      
                         &ts_ctrl_cfg);
        
        ts_ctrl_cfg.valid_map = CSL_VTM_TS_CTRL_RESET_CTRL_VALID        |
                                CSL_VTM_TS_CTRL_SOC_VALID               |
                                CSL_VTM_TS_CTRL_MODE_VALID;

        ts_ctrl_cfg.adc_stat   = CSL_VTM_TS_CTRL_SINGLESHOT_ADC_CONV_IN_PROGRESS;
        ts_ctrl_cfg.mode       = CSL_VTM_TS_CTRL_CONTINUOUS_MODE;
        ts_ctrl_cfg.tsReset    = CSL_VTM_TS_CTRL_SENSOR_NORM_OP;
        (void) CSL_vtmTsSetCtrl (p_vtm_cfg2_regs,
                                 ts_id,      
                                 &ts_ctrl_cfg,
                                 FALSE);

        /* setup the thermal shutdown range */
        (void) CSL_vtmTsSetMaxTOutRgAlertThr( p_vtm_cfg2_regs,
                                              ts_id, 
                                              high_temp_in_milli_degree_celsius,
                                              low_temp_in_milli_degree_celsius);
    }

    /* print current temperature for all sensors  */
    {
        extern int32_t gCSL_vtm_pvt_error[4];
        UART_printf ("\nN40 error is : %d \n \
                       P30 error is : %d \n \
                       P125 err  is : %d \n \
                       P150 err  is : %d \n ", \
                       gCSL_vtm_pvt_error[0], \
                       gCSL_vtm_pvt_error[1], \
                       gCSL_vtm_pvt_error[2], \
                       gCSL_vtm_pvt_error[3]); 
        UART_printf ("\n");
        max_temp_read = 0;
        for (ts_id = 0; ts_id < TEST_MAX_SENSOR_FOR_VTM_ALERT; ts_id++)
        {
            CSL_vtmTsGetSensorStat (p_vtm_cfg1_regs, &tsStat_read_ctrl, \
                                    ts_id, &ts_stat_val);
             
             adc_code = ts_stat_val.data_out;
             (void) CSL_vtmTsConvADCToTemp (adc_code,
                                        (CSL_vtm_tmpSens_id)     ts_id,
                                         &temp_milli_degrees_read);


             UART_printf (" \
                sensor id                       : %d \
                adc_code                        : %d \
                temp in milli degree celcius    : %d \n", \
                ts_id, adc_code, temp_milli_degrees_read);
             if (max_temp_read < temp_milli_degrees_read)
             {
                 max_temp_read = temp_milli_degrees_read;
             }
        }
        UART_printf ("\n");

        if (gAuto_test == TRUE)
        {
            /* Set the LT_THR0, GT_THR1, GT_THR2 values */
            lt_thr0_val = max_temp_read + 500;
            gt_thr1_val = lt_thr0_val + 1800;
            gt_thr2_val = gt_thr1_val + 1800;
        }

        UART_printf(" Threshold values programmed in milli degree celsius are \n");
        UART_printf (" \t \
           lt_thr0_val    : %d \
           gt_thr1_val    : %d \
           gt_thr2_val    : %d \n", \
           lt_thr0_val, gt_thr1_val, gt_thr2_val);

        for (ts_id = 0; ts_id < TEST_MAX_SENSOR_FOR_VTM_ALERT; ts_id++)
        {
            CSL_vtmTsConvTempToAdc(lt_thr0_val, ts_id,  &adc_code);
            test_csl_set_temp_thr(adc_code, ts_id, TEST_CSL_VTM_LT_THR0_SET);
            CSL_vtmTsConvTempToAdc(gt_thr1_val, ts_id,  &adc_code);
            test_csl_set_temp_thr(adc_code, ts_id, TEST_CSL_VTM_GT_THR1_SET);
            CSL_vtmTsConvTempToAdc(gt_thr2_val, ts_id,  &adc_code);
            test_csl_set_temp_thr(adc_code, ts_id, TEST_CSL_VTM_GT_THR2_SET);
        }
    
        /* Register the low, gt_thr1, gt_thr2 Interrupts,
           note same ISR is used for all temperature sensors */
        test_csl_register_temp_thr_isrs((uintptr_t) test_csl_vtm_pvt_sensor_ltThr0_isr, \
                                        (uintptr_t) lt_thr0_val,                        \
                                        TEST_CSL_VTM_LT_THR0_SET);
        test_csl_register_temp_thr_isrs((uintptr_t) test_csl_vtm_pvt_sensor_gtThr1_isr, \
                                        (uintptr_t) gt_thr1_val,                        \
                                        TEST_CSL_VTM_GT_THR1_SET);
        test_csl_register_temp_thr_isrs((uintptr_t) test_csl_vtm_pvt_sensor_gtThr2_isr, \
                                        (uintptr_t) gt_thr2_val,                        \
                                        TEST_CSL_VTM_GT_THR2_SET);

        i = 0;
        while (TRUE)
        {
            int32_t        temp[CSL_VTM_TEST_NUM_SENSORS];
            /* if gt_thr2 is fired, wait for the cooling to take place */
            while ((g_gt_thr2_alert_fired == TRUE) && \
                   (g_lt_thr0_alert_fired == FALSE))
            {
                /* No operations until cooling takes place */
                asm (" nop ");
            }
            /* have some computations to raise the temperature */
            /* Do not do any computations if temperature is above normal */
            tmp_value += a*b;
            tmp_value += b/c;
            a = b;
            b = c;
            c= tmp_value;

            for (ts_id = 0; ts_id < CSL_VTM_TEST_NUM_SENSORS; ts_id++)
            {
                CSL_vtmTsGetSensorStat (p_vtm_cfg1_regs, &tsStat_read_ctrl, \
                                        ts_id, &ts_stat_val);
                 
                adc_code = ts_stat_val.data_out;
                (void) CSL_vtmTsConvADCToTemp (adc_code,
                                               (CSL_vtm_tmpSens_id) ts_id,
                                               &temp[ts_id]);
            }

            if (i == 8096)
            {
                 i = 0;
                 UART_printf ("s0_temp: %d  s1_temp: %d s2_temp: %d  s3_temp: %d  s4_temp: %d \r", temp[0], temp[1], temp[2], temp[3], temp[4]);
            }

            i++;
            /* Check if all alerts are fired, exit if done */
            g_all_thr_alerts_fired = g_lt_thr0_alert_fired & \
                                     g_gt_thr1_alert_fired & \
                                     g_gt_thr2_alert_fired;

            if ((g_all_thr_alerts_fired == TRUE) && 
                (gAuto_test == TRUE))
            {
                break;
            }
        }
    }

    UART_printf("\nApplication is completed!!\n");
#if defined (UNITY_INCLUDE_CONFIG_H)
    UART_printf("\n All tests have passed. \n");
    TEST_PASS();
#endif

}

void test_csl_vtm_pvt_sensor_temp_alert_runner(void)
{
    /* @description:Test runner for VTM tests

       @requirements: PRSDK-6371, PRSDK-7237, PRSDK-7696, PRSDK-8209, PRSDK-7407

       @cores: mcu1_0 */
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (test_csl_vtm_pvt_sensor_temp_alert);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_vtm_pvt_sensor_temp_alert();
#endif
    return;
}

int32_t main(void)
{
    
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    
    boardCfg = BOARD_INIT_UNLOCK_MMR | 
        BOARD_INIT_MODULE_CLOCK | 
        BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_UART_STDIO;
    
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        #if defined (UNITY_INCLUDE_CONFIG_H)
            TEST_FAIL();
        #endif
    }
    else
    {
       (void) test_csl_vtm_pvt_sensor_temp_alert_runner();
        while (1)
        {
        }
    }
}
