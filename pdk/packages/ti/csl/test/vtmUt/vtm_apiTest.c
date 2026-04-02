/* Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file     vtm_apiTest.c
 *
 *  \brief    This file contains VTM API test code.
 *
 *  \details  VTM API tests
 **/
/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/
#include <vtm_test.h>

/*===========================================================================*/
/*                         Declarations                                      */
/*===========================================================================*/
/* None */

/*===========================================================================*/
/*                         Macros                                            */
/*===========================================================================*/
/* None */

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/

/*===========================================================================*/
/*                         Function definitions                              */
/*===========================================================================*/

int32_t cslVTM_apiTest(void)
{
    CSL_vtm_cfg1Regs            *p_cfg1;
    CSL_vtm_cfg2Regs            *p_cfg2;
    int32_t                     i, testStatus, cslRet = CSL_PASS;
    uint8_t                     val8_t;
    CSL_vtm_tsThrVal            tsThrVal;
    CSL_vtm_tsGlobal_cfg        globalCfg;
    CSL_vtm_ts_ctrl_cfg         tsCtrl;
    CSL_vtm_adc_code            adc_code;
    int32_t                     temp_milli_degree_c;
    CSL_vtm_tsStat_read_ctrl    tsStatReadCtrl;
    CSL_vtm_tsStat_val          tsStatReadVal;
    CSL_vtm_info                vtm_info;
    CSL_vtm_staticRegs          vtm_static_regs;

    p_cfg1 = (CSL_vtm_cfg1Regs *) CSL_VTM_TEST_CFG1_BASE;
    p_cfg2 = (CSL_vtm_cfg2Regs *) CSL_VTM_TEST_CFG2_BASE;
   
    if (cslRet == CSL_PASS)
    {
        for (i = CSL_VTM_VID_OPP_0_CODE; i < CSL_VTM_VID_OPP_MAX_NUM; i++ )
        {
            cslRet = CSL_vtmVdGetOppVid(p_cfg1,  
                                        CSL_VTM_VD_DOMAIN_1,
                                        i, 
                                        &val8_t);
            if (cslRet != CSL_PASS)
            {
                break;
            }
        }

    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        for (i = CSL_VTM_VID_OPP_0_CODE; i < CSL_VTM_VID_OPP_MAX_NUM; i++ )
        {
            cslRet = CSL_vtmVdSetOppVid(p_cfg1,  
                                        CSL_VTM_VD_DOMAIN_1,
                                        i, 
                                        val8_t,
                                        TRUE);
            if (cslRet != CSL_PASS)
            {
                break;
            }
        }
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        cslRet = CSL_vtmVdEvtSelSet(p_cfg1, 
                                    CSL_VTM_VD_DOMAIN_1,
                                    CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_0,
                                    TRUE);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        cslRet =  CSL_vtmVdEvtSelClr(p_cfg1, 
                                     CSL_VTM_VD_DOMAIN_1,
                                     CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_0,
                                     TRUE);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        cslRet = CSL_vtmVdGetEvtStat(p_cfg1,
                                     CSL_VTM_VD_DOMAIN_1,
                                     &val8_t);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        for (i=CSL_VTM_VD_LT_THR0_INTR_RAW_SET; i<=CSL_VTM_VD_GT_THR2_INTR_EN_CLR; i*=2)
        {
            cslRet = CSL_vtmVdThrIntrCtrl(p_cfg1,
                                          CSL_VTM_VD_DOMAIN_1,
                                          i,
                                          TRUE);

            if (cslRet != CSL_PASS)
            {
                break;
            }
        }
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        tsThrVal.thrValidMap    = CSL_VTM_GT_TH1_VALID | \
                                  CSL_VTM_GT_TH2_VALID | \
                                  CSL_VTM_LT_TH0_VALID;

        cslRet = CSL_vtmTsGetThresholds (p_cfg1,
                                         CSL_VTM_TS_ID_0,
                                         &tsThrVal);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        tsThrVal.thrValidMap    = CSL_VTM_GT_TH1_VALID | \
                                  CSL_VTM_GT_TH2_VALID | \
                                  CSL_VTM_LT_TH0_VALID;

        cslRet = CSL_vtmTsSetThresholds (p_cfg1,
                                         CSL_VTM_TS_ID_0,
                                         &tsThrVal,
                                         TRUE);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        globalCfg.validMap  = CSL_VTM_TSGLOBAL_CLK_SEL_VALID |
                              CSL_VTM_TSGLOBAL_CLK_DIV_VALID |
                              CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_EN_VALID |
                              CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR0_VALID |
                              CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR_VALID |
                              CSL_VTM_TSGLOBAL_SAMPLES_PER_CNT_VALID;

        cslRet = CSL_vtmTsGetGlobalCfg (p_cfg2,
                                        &globalCfg);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        globalCfg.validMap  = CSL_VTM_TSGLOBAL_CLK_SEL_VALID |
                              CSL_VTM_TSGLOBAL_CLK_DIV_VALID |
                              CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_EN_VALID |
                              CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR0_VALID |
                              CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR_VALID |
                              CSL_VTM_TSGLOBAL_SAMPLES_PER_CNT_VALID;

        cslRet = CSL_vtmTsSetGlobalCfg (p_cfg2,
                                        &globalCfg,
                                        TRUE);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        tsCtrl.valid_map  =  CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID    | \
                             CSL_VTM_TS_CTRL_RESET_CTRL_VALID         | \
                             CSL_VTM_TS_CTRL_SOC_VALID                | \
                             CSL_VTM_TS_CTRL_MODE_VALID ;

        cslRet = CSL_vtmTsGetCtrl (p_cfg2,
                                   CSL_VTM_TS_ID_0,
                                   &tsCtrl);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        tsCtrl.valid_map  =  CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID    | \
                             CSL_VTM_TS_CTRL_RESET_CTRL_VALID         | \
                             CSL_VTM_TS_CTRL_SOC_VALID                | \
                             CSL_VTM_TS_CTRL_MODE_VALID ;

        cslRet = CSL_vtmTsSetCtrl (p_cfg2,
                                   CSL_VTM_TS_ID_0,
                                   &tsCtrl,
                                   TRUE);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        for (adc_code = 0; adc_code < 1023; adc_code++)
        {
            cslRet = CSL_vtmTsConvADCToTemp(adc_code,
                                            CSL_VTM_TS_ID_0,
                                            &temp_milli_degree_c);
            if (cslRet != CSL_PASS)
            {
                break;
            }
        }
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        for (temp_milli_degree_c = -40000; temp_milli_degree_c < 150000; temp_milli_degree_c+=1000)
        {
            cslRet = CSL_vtmTsConvTempToAdc(temp_milli_degree_c,
                                            CSL_VTM_TS_ID_0,
                                            &adc_code);
            if (cslRet != CSL_PASS)
            {
                break;
            }
        }
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        tsStatReadCtrl =    \
                         (CSL_VTM_TS_READ_VD_MAP_VAL              | 
                         CSL_VTM_TS_READ_ALL_THRESHOLD_ALERTS     |
                         CSL_VTM_TS_READ_FIRST_TIME_EOC_BIT       |
                         CSL_VTM_TS_READ_DATA_VALID_BIT           |
                         CSL_VTM_TS_READ_DATA_OUT_VAL);
                        
        cslRet = CSL_vtmTsGetSensorStat(p_cfg1,
                                        &tsStatReadCtrl,
                                        CSL_VTM_TS_ID_0,
                                        &tsStatReadVal);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }   
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        cslRet = CSL_vtmTsSetMaxTOutRgAlertThr(p_cfg2,
                                        CSL_VTM_TS_ID_0,
                                        150000,
                                        115000);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }     
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        cslRet = CSL_vtmGetVTMInfo(p_cfg1,
                                   &vtm_info);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    }     
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        cslRet = CSL_vtmReadBackStaticRegisters(p_cfg1,
                                                p_cfg2,
                                                &vtm_static_regs);
    }
    else
    {
        UART_printf("cslVTM_apiTest: failure on line no. %d \n", __LINE__);
    } 
    
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif
    
    if (cslRet == CSL_PASS)
    {
        testStatus = CSL_APP_TEST_PASS;
    }
    else
    {
        testStatus = CSL_APP_TEST_FAILED;
    }
    return (testStatus);
}

/* Nothing past this point */
