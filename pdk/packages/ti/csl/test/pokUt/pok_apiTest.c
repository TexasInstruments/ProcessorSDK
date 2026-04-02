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
 *  \file     pok_apiTest.c
 *
 *  \brief    This file contains POK API test code.
 *
 *  \details  POK API tests
 **/
/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/
#include <pok_test.h>

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

int32_t cslPOK_apiTest(void)
{
    int32_t                     testStatus, cslRet = CSL_PASS;
    CSL_wkupCtrlRegsBase_t      *pBaseAddr = (CSL_wkupCtrlRegsBase_t *) CSL_TEST_POK_MMR_BASE;
    CSL_pokCfg_t                 pokCfg;
    CSL_pokPorStat_t             porStat;
    CSL_pokPorCfg_t              porGetCfg =
    {
        .override[CSL_PORHV_OVERRIDE_INDEX]  = CSL_POR_GET_OVERRIDE_VALUE,
        .override[CSL_BGAP_OVERRIDE_INDEX]   = CSL_POR_GET_OVERRIDE_VALUE,
        .override[CSL_POKHV_OVERRIDE_INDEX]  = CSL_POR_GET_OVERRIDE_VALUE,
        .override[CSL_POKLVA_OVERRIDE_INDEX] = CSL_POR_GET_OVERRIDE_VALUE,
        .override[CSL_POKLVB_OVERRIDE_INDEX] = CSL_POR_GET_OVERRIDE_VALUE,
        .maskHHVOutputEnable = TRUE,
        .trim_select         = CSL_POR_TRIM_SELECTION_GET_VALUE
    };
    CSL_pokCfg_t                 pokGetCfg =
    {
        .hystCtrl       = CSL_PWRSS_GET_HYSTERESIS_VALUE,
        .voltDetMode    = CSL_PWRSS_GET_VOLTAGE_DET_MODE,
        .trim           = CSL_PWRSS_GET_TRIM_VALUE,
        .detectionCtrl  = CSL_POK_GET_DETECTION_VALUE,
        .pokEnSelSrcCtrl= CSL_POK_GET_ENSEL_VALUE
    };
    CSL_pok_id                   pokId;
    /** hysteresis control */
    CSL_pwrss_hysteresis            hystCtrl;
    /** Voltage Detection Mode control */
    CSL_pwrss_vd_mode               voltDetMode;
    /** POK Trim bits 7 bits wide */
    CSL_pwrss_trim                  trim;
    /** POK Detection Enable */
    CSL_POK_detection               detectionCtrl;
    /** POK Enable Source control */
    CSL_POK_enSelSrc                pokEnSelSrcCtrl;
    /** POK value */
    CSL_pokVal_t                    pokVal;
    /** POK in POR value */
    CSL_pokPorVal_t                 pokPorVal;


    if (cslRet == CSL_PASS)
    {
        for (pokId = CSL_FIRST_POK_ID; pokId <= CSL_LAST_POK_ID; pokId++)
        {
            for (hystCtrl = CSL_PWRSS_SET_HYSTERESIS_DISABLE; \
                 hystCtrl <= CSL_PWRSS_HYSTERESIS_NO_ACTION; \
                 hystCtrl++ )
            {
                for (voltDetMode = CSL_PWRSS_SET_UNDER_VOLTAGE_DET_ENABLE; \
                     voltDetMode <= CSL_PWRSS_VOLTAGE_DET_NO_ACTION; \
                     voltDetMode++ )
                {
                    for (trim = CSL_PWRSS_MAX_TRIM_VALUE; \
                         trim <= CSL_PWRSS_GET_TRIM_VALUE; \
                         trim++ )
                    {
                        for (detectionCtrl = CSL_POK_DETECTION_DISABLE; \
                             detectionCtrl <= CSL_POK_GET_DETECTION_VALUE; \
                             detectionCtrl++ )
                        {
                            for (pokEnSelSrcCtrl= CSL_POK_ENSEL_HWTIEOFFS; \
                                 pokEnSelSrcCtrl <= CSL_POK_GET_ENSEL_VALUE; \
                                 pokEnSelSrcCtrl++ )
                            {
                                pokCfg.hystCtrl         = hystCtrl;
                                pokCfg.voltDetMode      = voltDetMode;
                                pokCfg.trim             = trim;
                                pokCfg.detectionCtrl    = detectionCtrl;
                                pokCfg.pokEnSelSrcCtrl  = pokEnSelSrcCtrl;
                                cslRet = CSL_pokSetControl   (pBaseAddr,
                                                              &pokCfg,
                                                              pokId);
                                if (cslRet != CSL_PASS)
                                {
                                    /* Check if the CSL API failure is expected failure */
                                    if ( (pokEnSelSrcCtrl == CSL_POK_GET_ENSEL_VALUE) ||
                                         (detectionCtrl   == CSL_POK_GET_DETECTION_VALUE) ||
                                         (trim            == CSL_PWRSS_GET_TRIM_VALUE) ||
                                         (voltDetMode     == CSL_PWRSS_GET_VOLTAGE_DET_MODE) ||
                                         (hystCtrl        == CSL_PWRSS_GET_HYSTERESIS_VALUE))
                                    {
                                        /* no break, expected result */
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                cslRet = CSL_pokGetControl   (pBaseAddr,
                                                              &pokGetCfg,
                                                              &pokVal,
                                                              pokId);
                                if (cslRet != CSL_PASS)
                                {
                                    break;
                                }
                             }
                             if (cslRet != CSL_PASS)
                             {
                                 break;
                             }
                        }
                        if (cslRet != CSL_PASS)
                        {
                            break;
                        }
                    }
                    if (cslRet != CSL_PASS)
                    {
                        break;
                    }
                }
                if (cslRet != CSL_PASS)
                {
                    break;
                }
            }
            if (cslRet != CSL_PASS)
            {
                break;
            }
        }
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        cslRet =  CSL_porGetStat (pBaseAddr, &porStat);
        UART_printf ("porStat.porBGapOK = %d and porStat.porModuleStatus = %d \n", porStat.porBGapOK, porStat.porModuleStatus);
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        cslRet =  CSL_porGetControl(pBaseAddr,&porGetCfg, &pokPorVal);
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    if (cslRet == CSL_PASS)
    {
        cslRet =  CSL_porSetControl(pBaseAddr,(CSL_pokPorCfg_t *)&pokPorVal);
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
