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
 *  \file     vtm_sensor_read.c
 *
 *  \brief    This file contains VTM PVT sensor read example application
 *            Implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "vtm_sensor_read.h"

#define CSL_VTM_TEST_NUM_EFUSE_REGS                              (4u)
#define CSL_VTM_TEST_EFUSE_BASE_ADDR                             (0x43000300UL)
#define CSL_VTM_TEST_NUM_SENSORS                                 (5u)
#define CSL_VTM_TEST_NUM_ITERATIONS                              (60u)

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#define CSL_VTM_TEST_CFG1_BASE          (CSL_WKUP_VTM0_MMR_VBUSP_CFG1_BASE)
#define CSL_VTM_TEST_CFG2_BASE          (CSL_WKUP_VTM0_MMR_VBUSP_CFG2_BASE)
#elif defined (SOC_AM64X)
#define CSL_VTM_TEST_CFG1_BASE          (CSL_VTM0_MMR_VBUSP_CFG1_BASE)
#define CSL_VTM_TEST_CFG2_BASE          (CSL_VTM0_MMR_VBUSP_CFG2_BASE)
#else
/* Address needs to be defined */
#endif
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
volatile uint32_t g_efuse_0, gefuse_1, gefuse_2, gefuse_3;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* Unity functions */
void test_csl_vtm_pvt_sensor_read_runner(void);
void test_csl_vtm_pvt_sensor_read (void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
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

void test_csl_vtm_pvt_sensor_read (void)
{
    int32_t temp_milli_degrees_read, ts_id, i, j, first_time = 1;
    int32_t is_diff_value_read[CSL_VTM_TEST_NUM_SENSORS] = {0, 0, 0, 0, 0};
    Bool    test_pass;
    CSL_vtm_adc_code       adc_code;
    CSL_vtm_cfg1Regs       *p_vtm_cfg1_regs;
    CSL_vtm_cfg2Regs       *p_vtm_cfg2_regs;
    CSL_vtm_tsStat_read_ctrl tsStat_read_ctrl = CSL_VTM_TS_READ_DATA_OUT_VAL;
    CSL_vtm_tsStat_val       ts_stat_val;
    CSL_vtm_ts_ctrl_cfg     ts_ctrl_cfg;
    uint32_t *              efuse_base_addr = (uint32_t *)  CSL_VTM_TEST_EFUSE_BASE_ADDR;

    p_vtm_cfg1_regs = (CSL_vtm_cfg1Regs *) CSL_VTM_TEST_CFG1_BASE;
    p_vtm_cfg2_regs = (CSL_vtm_cfg2Regs *) CSL_VTM_TEST_CFG2_BASE;

    /* Setup the Efuse values if needed */
#if defined (CSL_VTM_SS_UNIT) || defined (CSL_VTM_TT_UNIT) || defined (CSL_VTM_FF_UNIT)
    efuse_base_addr[0] = CSL_VTM_VAL_EFUSE_0;
    efuse_base_addr[1] = CSL_VTM_VAL_EFUSE_1;
    efuse_base_addr[2] = CSL_VTM_VAL_EFUSE_2;    
    efuse_base_addr[3] = CSL_VTM_VAL_EFUSE_3;
#endif

    UART_printf ("efuse 0  is : 0x%x \n \
               efuse 1 is : 0x%x \n \
               efuse 2  is : 0x%x \n \
               efuse 3  is : 0x%x \n", \
               efuse_base_addr[0], \
               efuse_base_addr[1], \
               efuse_base_addr[2], \
               efuse_base_addr[3]);

    for (ts_id = 0; ts_id < CSL_VTM_TEST_NUM_SENSORS; ts_id++)
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
    }

    for (i = 0 ; i < CSL_VTM_TEST_NUM_ITERATIONS; i++)
    {
        for (ts_id = 0; ts_id < CSL_VTM_TEST_NUM_SENSORS; ts_id++)
        {
            CSL_vtmTsGetSensorStat (p_vtm_cfg1_regs, &tsStat_read_ctrl, \
                                    ts_id, &ts_stat_val);
             
             adc_code = ts_stat_val.data_out;
             (void) CSL_vtmTsConvADCToTemp (adc_code,
                                        (CSL_vtm_tmpSens_id)     ts_id,
                                         &temp_milli_degrees_read);

             if (first_time == 1)
             {
                first_time = 0;
                extern int32_t gCSL_vtm_pvt_error[4];
                UART_printf (" N40 error is : %d \n \
                               P30 error is : %d \n \
                               P125 err  is : %d \n \
                               P150 err  is : %d \n ", \
                               gCSL_vtm_pvt_error[0], \
                               gCSL_vtm_pvt_error[1], \
                               gCSL_vtm_pvt_error[2], \
                               gCSL_vtm_pvt_error[3]);
             }

             is_diff_value_read[ts_id] ^= temp_milli_degrees_read; 
             UART_printf (" \
                sensor id                       : %d \
                adc_code                        : %d \
                temp in milli degree celcius    : %d \n", \
                ts_id, adc_code, temp_milli_degrees_read);
        }
        UART_printf("\n");
        /* have some computations to raise the temperature */
        for (j = 0; j < 10; j++)
        {
           int32_t a, b, c;
           for ( a=1; a<20;a++)
           {
               for ( b=1; b<20;b++)
               {
                   for ( c=1; c<20;c++)
                   {
                       first_time = a*b;
                       first_time += b/c;
                       first_time += c*a*b*first_time;
                   }
               }
           }
           /* reset back to 0 */
           first_time = 0; 
        }
    }

    UART_printf("\nApplication is completed!!\n");
    test_pass = TRUE;
    for ( i = 0; i < CSL_VTM_TEST_NUM_SENSORS; i++)
    {
        if (is_diff_value_read[i] == 0)
        {
            test_pass = FALSE;
            break;
        }
    }
#if defined (UNITY_INCLUDE_CONFIG_H)
    if (test_pass == TRUE)
    {
        UART_printf("\n All tests have passed. \n");
        TEST_PASS();
    }
    else
    {
        UART_printf("\n VTM read test is failed \n");
        TEST_FAIL();
    }
#endif

}

void test_csl_vtm_pvt_sensor_read_runner(void)
{
    /* @description:Test runner for VTM tests

       @requirements: PRSDK-6371, PRSDK-7237, PRSDK-7696, PRSDK-8209

       @cores: mcu1_0 */
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (test_csl_vtm_pvt_sensor_read);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_vtm_pvt_sensor_read();
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
       (void) test_csl_vtm_pvt_sensor_read_runner();
        while (1)
        {
        }
    }
}
