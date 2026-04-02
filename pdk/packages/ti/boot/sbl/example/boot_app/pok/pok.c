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
 *  \file     pok.c
 *
 *  \brief    This file implements POK task function
 */

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/

#include "pok.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* define the unlock values */
#define KICK0_UNLOCK_VAL 0x68EF3490
#define KICK1_UNLOCK_VAL 0xD172BC5A

/* ========================================================================== */
/*                    Internal Function Declarations                          */
/* ========================================================================== */

int32_t        BootApp_pokSetConfig(SDL_POK_Inst instance, SDL_POK_config *pPokCfg);
int32_t        BootApp_pokInPorFunc(void);
int32_t        BootApp_pokFunc(void);
int32_t        BootApp_pokEsmApplicationCallbackFunction(SDL_ESM_Inst esmInstType,
                                                   SDL_ESM_IntType esmIntType,
                                                   uint32_t grpChannel,
                                                   uint32_t index,
                                                   uint32_t intSrc,
                                                   void *arg);
volatile bool ESM_Error = false;
uint32_t BootApp_deactivateTrigger(uint32_t *esm_err_sig );
static void BootApp_getInstance(SDL_POK_Inst *instance, uint32_t *esm_err_sig);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

sdlPokTest_t  BootApp_pokTestList[] = {
    {BootApp_pokFunc,        "POK EXAMPLE UC-1" ,         SDL_APP_TEST_NOT_RUN },
    {BootApp_pokInPorFunc,   "POR EXAMPLE UC-2" ,         SDL_APP_TEST_NOT_RUN },
    {NULL,               "TERMINATING CONDITION",     SDL_APP_TEST_NOT_RUN }
};

SDL_ESM_config POK_Test_esmInitConfig_WKUP =
{
    .esmErrorConfig = {0u, 8u}, /* Self test error config */
    .enableBitmap = {0xffffffffu, 0x00180003u, 0xffffffffu,
                },
     /**< All events enable: except clkstop events for unused clocks */
    .priorityBitmap = {0xffffffffu, 0x00180003u, 0xffffffffu,
                        },
    /**< All events high priority: except clkstop events for unused clocks */
    .errorpinBitmap = {0xffffffffu, 0x00180003u, 0xffffffffu,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and selftest error events */
};

extern int32_t BootApp_pokEsmApplicationCallbackFunction(SDL_ESM_Inst esmInstType,
                                                   SDL_ESM_IntType esmIntType,
                                                   uint32_t grpChannel,
                                                   uint32_t index,
                                                   uint32_t intSrc,
                                                   void *arg);
static uint32_t arg;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/*===========================================================================*/
/*                         Function definitions                              */
/*===========================================================================*/

int32_t BootApp_pokEsmApplicationCallbackFunction(SDL_ESM_Inst esmInst,
                                            SDL_ESM_IntType esmIntrType,
                                            uint32_t grpChannel,
                                            uint32_t index,
                                            uint32_t intSrc,
                                            void *arg)
{
    int32_t retVal = SDL_PASS;
    UART_printf("\n  ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                esmInst, esmIntrType, grpChannel, index, intSrc);
    UART_printf("  Take action \n");
    /* Disable the ESM Interrupt */
    BootApp_deactivateTrigger(&intSrc);
    SDL_ESM_clrNError(esmInst);
    ESM_Error = true;
    /* Any additional customer specific actions can be added here */

    return retVal;
}

uint32_t BootApp_deactivateTrigger(uint32_t *esm_err_sig )
{
    SDL_POK_Inst               instance;
    SDL_POK_config             pPokCfg;
    SDL_pokVal_t               pPokVal;
    SDL_wkupCtrlRegsBase_t    *pBaseAddr = (SDL_wkupCtrlRegsBase_t *) SDL_POK_MMR_BASE;
    int32_t                    sdlRet = SDL_EFAIL;

    BootApp_getInstance(&instance, esm_err_sig);

    pPokCfg.hystCtrl = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.voltDetMode = SDL_PWRSS_GET_VOLTAGE_DET_MODE;
    pPokCfg.trim = SDL_PWRSS_TRIM_NO_ACTION;
    pPokCfg.detectionCtrl = SDL_POK_DETECTION_NO_ACTION;
    pPokCfg.pokEnSelSrcCtrl = SDL_POK_ENSEL_NO_ACTION;
    pPokCfg.hystCtrlOV = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.trimOV = SDL_PWRSS_TRIM_NO_ACTION;
    pPokCfg.deglitch = SDL_PWRSS_DEGLITCH_NO_ACTION;

    SDL_pokGetControl (pBaseAddr,&pPokCfg,&pPokVal,instance);
    /* Re-configure to "good" setting */
    if (pPokVal.voltDetMode == SDL_PWRSS_SET_UNDER_VOLTAGE_DET_ENABLE)
    {
        pPokCfg.trim = 0;

        pPokCfg.trimOV = SDL_PWRSS_TRIM_NO_ACTION;
    }
    else
    {
        pPokCfg.trimOV = 45;
        pPokCfg.trim = SDL_PWRSS_TRIM_NO_ACTION;
    }

    pPokCfg.hystCtrl = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.voltDetMode = pPokVal.voltDetMode;
    pPokCfg.detectionCtrl = SDL_POK_DETECTION_NO_ACTION;
    pPokCfg.pokEnSelSrcCtrl = SDL_POK_ENSEL_NO_ACTION;
    pPokCfg.hystCtrlOV = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.deglitch = SDL_PWRSS_DEGLITCH_NO_ACTION;
    sdlRet = SDL_POK_init(instance,&pPokCfg);

    return sdlRet;
}

int32_t BootApp_pokSetConfig(SDL_POK_Inst instance, SDL_POK_config *pPokCfg)
{
    int32_t sdlRet = SDL_EFAIL;
    sdlRet = SDL_POK_init(instance, pPokCfg);
    if (sdlRet != SDL_PASS)
    {
        UART_printf("SDL_POK_init failed! \n");
    }
    else
    {
        volatile int32_t i = 0;
        UART_printf("Waiting for ESM to report the error \n");
        /* Wait for the ESM interrupt to report the error */
        do {
            i++;
            if (i > 0x0FFFFFFF)
            {
                /* Timeout for the wait */
                break;
            }
        } while (ESM_Error == false);

        if (ESM_Error == true)
        {
            UART_printf(" Got the ESM Error Interrupt \n");
            UART_printf("Action taken \n");
            ESM_Error = false;
            if (sdlRet != SDL_PASS)
            {
                UART_printf("SDL_POK_init failed! \n");
            }
        }
        else
        {
            sdlRet = SDL_EFAIL;
        }
    }
    return(sdlRet);
}


int32_t BootApp_pokInPorFunc(void)
{
    int32_t                      testStatus, sdlRet = SDL_PASS, overallStatus = SDL_APP_TEST_PASS;
    SDL_POK_config               pPokCfg;
    SDL_POK_Inst                 instance;

    instance = SDL_POR_POKLVA_OV_ID;

    UART_printf ("\n\n POK ID = %d , monitoring set to OV \n", instance);
    pPokCfg.voltDetMode = SDL_PWRSS_SET_OVER_VOLTAGE_DET_ENABLE;
    pPokCfg.trim = 0;
    pPokCfg.hystCtrl = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.detectionCtrl = SDL_POK_DETECTION_NO_ACTION;
    pPokCfg.pokEnSelSrcCtrl = SDL_POK_ENSEL_NO_ACTION;

    pPokCfg.hystCtrlOV = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.trimOV = 0;
    pPokCfg.deglitch = SDL_PWRSS_DEGLITCH_NO_ACTION;

    sdlRet = BootApp_pokSetConfig(instance, &pPokCfg);
    if (sdlRet == SDL_PASS)
    {
        testStatus = SDL_APP_TEST_PASS;
    }
    else
    {
        testStatus = SDL_APP_TEST_FAILED;
        overallStatus = SDL_APP_TEST_FAILED;
    }
    UART_printf("Safety software Example UC-2 pok for instance  %d %s\n",
                instance, (testStatus == SDL_APP_TEST_PASS) ? "PASSED" : "FAILED");

    return (overallStatus);
}

int32_t BootApp_pokFunc(void)
{
    int32_t                      testStatus, sdlRet = SDL_PASS, overallStatus = SDL_APP_TEST_PASS;
    SDL_POK_config               pPokCfg;
    SDL_POK_Inst                 instance;

    instance = SDL_POK_VDD_CORE_ID;

    UART_printf ("\n\n POK ID = %d , monitoring set to UV \n", instance);
    pPokCfg.voltDetMode = SDL_PWRSS_SET_UNDER_VOLTAGE_DET_ENABLE;
    pPokCfg.trim = 127;
    pPokCfg.hystCtrl = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.detectionCtrl = SDL_POK_DETECTION_NO_ACTION;
    pPokCfg.pokEnSelSrcCtrl = SDL_POK_ENSEL_NO_ACTION;

    pPokCfg.hystCtrlOV = SDL_PWRSS_HYSTERESIS_NO_ACTION;
    pPokCfg.trimOV = SDL_PWRSS_TRIM_NO_ACTION;
    pPokCfg.deglitch = SDL_PWRSS_DEGLITCH_NO_ACTION;

    sdlRet = BootApp_pokSetConfig(instance, &pPokCfg);
    if (sdlRet == SDL_PASS)
    {
        testStatus = SDL_APP_TEST_PASS;
    }
    else
    {
        testStatus = SDL_APP_TEST_FAILED;
        overallStatus = SDL_APP_TEST_FAILED;
    }
    UART_printf("Safety software Example UC-1 pok for instance %d %s\n",
                instance, (testStatus == SDL_APP_TEST_PASS) ? "PASSED" : "FAILED");

    return (overallStatus);
}

static void BootApp_getInstance(SDL_POK_Inst *instance, uint32_t *esm_err_sig)
{
    switch (*esm_err_sig)
    {
        case WKUP_ESM_ERR_SIG_VDD_CORE_UV:
        case WKUP_ESM_ERR_SIG_VDD_CORE_OV:
            *instance    = SDL_POK_VDD_CORE_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDDR_CORE_UV:
        case WKUP_ESM_ERR_SIG_VDDR_CORE_OV:
            *instance    = SDL_POK_VDDR_CORE_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDD_CPU_UV:
        case WKUP_ESM_ERR_SIG_VDD_CPU_OV:
            *instance    = SDL_POK_VDD_CPU_ID;
            break;
        case WKUP_ESM_ERR_SIG_VMON_EXT_UV:
        case WKUP_ESM_ERR_SIG_VMON_EXT_OV:
            *instance    = SDL_POK_VMON_EXT_ID;
            break;
        case WKUP_ESM_ERR_SIG_VMON_EXT_MAIN_1P8_OV:
        case WKUP_ESM_ERR_SIG_VMON_EXT_MAIN_1P8_UV:
            *instance    = SDL_POK_VMON_EXT_MAIN_1P8_ID;
            break;
        case WKUP_ESM_ERR_SIG_VMON_EXT_MAIN_3P3_OV:
        case WKUP_ESM_ERR_SIG_VMON_EXT_MAIN_3P3_UV:
            *instance = SDL_POK_VMON_EXT_MAIN_3P3_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDD_MCU_OV:
            *instance = SDL_POK_VDD_MCU_OV_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDD_MCU_UV:
            *instance = SDL_POR_POKLVB_UV_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDDR_MCU_UV:
        case WKUP_ESM_ERR_SIG_VDDR_MCU_OV:
            *instance = SDL_POK_VDDR_MCU_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDDSHV_WKUP_GEN_UV:
        case WKUP_ESM_ERR_SIG_VDDSHV_WKUP_GEN_OV:
            *instance = SDL_POK_VDDSHV_WKUP_GEN_ID;
            break;
        case WKUP_ESM_ERR_SIG_CAP_VDDS_MCU_GEN_UV:
        case WKUP_ESM_ERR_SIG_CAP_VDDS_MCU_GEN_OV:
            *instance = SDL_POK_CAP_VDDS_MCU_GEN_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDDA_PMIC_IN_UV:
            *instance = SDL_POK_VDDA_PMIC_IN_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDDA_MCU_UV:
            *instance = SDL_POR_POKHV_UV_ID;
            break;
        case WKUP_ESM_ERR_SIG_VDDA_MCU_OV:
            *instance = SDL_POR_POKLVA_OV_ID;
            break;
        default:
            *instance = (SDL_POK_Inst)(-1);
            break;
    }
    return;
}

void BootApp_pokFxn(void)
{
    /* Declarations of variables */
    int32_t    testResult = SDL_APP_TEST_PASS;
    int32_t    i;
    int32_t    sdlRet;
    void *ptr = (void *)&arg;
    
    UART_printf("\n POK Test Application\r\n");
    /* Unlock the MMR in order to access the POK registers */
    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK0)) = KICK0_UNLOCK_VAL;
    *((uint32_t *)(SDL_WKUP_CTRL_MMR0_CFG0_BASE + SDL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK1)) = KICK1_UNLOCK_VAL;

    /* ESM Setup for POK tests */
    /* Initialize WKUP ESM module */
    sdlRet = SDL_ESM_init(SDL_ESM_INST_WKUP_ESM0, &POK_Test_esmInitConfig_WKUP, BootApp_pokEsmApplicationCallbackFunction,ptr);
    if (sdlRet != SDL_PASS) {
        /* print error and quit */
        UART_printf("sdlEsmSetupForPOK init: Error initializing WKUP ESM: sdlRet = SDL_EFAIL \n");
         sdlRet = -1;
    } else {
         UART_printf("\nsdlEsmSetupForPOK init: Init WKUP ESM complete \n");
    }

    for ( i = 0; BootApp_pokTestList[i].testFunction != NULL; i++)
    {
        testResult = BootApp_pokTestList[i].testFunction();
        BootApp_pokTestList[i].testStatus = testResult;
    }

    testResult = SDL_APP_TEST_PASS;
    for ( i = 0; BootApp_pokTestList[i].testFunction != NULL; i++)
    {
        if (BootApp_pokTestList[i].testStatus != SDL_APP_TEST_PASS)
        {
            UART_printf("Test Name: %s  FAILED \n", BootApp_pokTestList[i].name);
            testResult = SDL_APP_TEST_FAILED;
            break;
        }
        else
        {
            UART_printf("Test Name: %s  PASSED \n", BootApp_pokTestList[i].name);
        }
    }

    if (testResult == SDL_APP_TEST_PASS)
    {
        UART_printStatus("\n All tests have passed. \n");
    }
    else
    {
        UART_printStatus("\n Few/all tests Failed \n");
    }
}