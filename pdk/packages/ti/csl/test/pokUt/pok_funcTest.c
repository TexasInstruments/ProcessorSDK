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
 *  \file     pok_funcTest.c
 *
 *  \brief    This file contains POK API functionality test code.
 *
 *  \details  POK functionality tests
 **/
/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/
#include <pok_test.h>

/*===========================================================================*/
/*                         Declarations                                      */
/*===========================================================================*/
#define WKUP_ESM_INSTANCE                   (CSL_WKUP_ESM0_CFG_BASE)
#define WKUP_ESM_INTID                      (CSLR_MCU_R5FSS0_CORE0_INTR_WKUP_ESM0_ESM_INT_HI_LVL_0)
#define WKUP_ESM_ERR_SIG_POKHV_UV           (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU_3POKS0_POK_PGOOD_OUT_N_TO_ESM_0)
#define WKUP_ESM_ERR_SIG_POKHV_OV           (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU_3POKS0_POK_PGOOD_OUT_N_TO_ESM_1)
#define WKUP_ESM_ERR_SIG_PORHV_UV           (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU_3POKS0_POK_PGOOD_OUT_N_TO_ESM_0)
#define WKUP_ESM_ERR_SIG_POKLV_UV           (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU_3POKS0_POK_PGOOD_OUT_N_TO_ESM_2)
#define WKUP_ESM_ERR_SIG_VDDA_IN            (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU0_POK_PGOOD_OUT_N_TO_ESM_0)
#define WKUP_ESM_ERR_SIG_VDDSHV_WKUP_GEN_UV (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU0_POK_PGOOD_OUT_N_TO_ESM_1)
#define WKUP_ESM_ERR_SIG_VDDR_MCU_UV        (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU0_POK_PGOOD_OUT_N_TO_ESM_2)
#define WKUP_ESM_ERR_SIG_VDD_MCU_OV         (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU0_POK_PGOOD_OUT_N_TO_ESM_3)
#define WKUP_ESM_ERR_SIG_VDDSHV_WKUP_GEN_OV (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU0_POK_PGOOD_OUT_N_TO_ESM_4)
#define WKUP_ESM_ERR_SIG_VDDR_MCU_OV        (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG_MCU0_POK_PGOOD_OUT_N_TO_ESM_5)
#define WKUP_ESM_ERR_SIG_VDD_CORE_UV        (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_0)
#define WKUP_ESM_ERR_SIG_VDD_CPU_UV         (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_1)
#define WKUP_ESM_ERR_SIG_VMON_EXT_UV        (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_2)
#define WKUP_ESM_ERR_SIG_VDDR_CORE_UV       (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_3)
#define WKUP_ESM_ERR_SIG_VDD_CORE_OV        (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_4)
#define WKUP_ESM_ERR_SIG_VDD_CPU_OV         (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_5)
#define WKUP_ESM_ERR_SIG_VMON_EXT_OV        (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_6)
#define WKUP_ESM_ERR_SIG_VDDR_CORE_OV       (CSLR_WKUP_ESM0_ESM_PLS_EVENT0_WKUP_PRG0_POK_PGOOD_OUT_N_TO_ESM_7)


#define  ESM_INSTANCE                      WKUP_ESM_INSTANCE
#define  ESM_INTID                         WKUP_ESM_INTID
#define  ESM_ERR_SIG_POKHV_UV              WKUP_ESM_ERR_SIG_POKHV_UV
#define  ESM_ERR_SIG_POKHV_OV              WKUP_ESM_ERR_SIG_POKHV_OV
#define  ESM_ERR_SIG_POKLV_UV              WKUP_ESM_ERR_SIG_POKLV_UV
#define  ESM_ERR_SIG_PORHV_UV              WKUP_ESM_ERR_SIG_PORHV_UV
#define  ESM_ERR_SIG_VDDA_IN               WKUP_ESM_ERR_SIG_VDDA_IN
#define  ESM_ERR_SIG_VDD_CORE_UV           WKUP_ESM_ERR_SIG_VDD_CORE_UV
#define  ESM_ERR_SIG_VDDSHV_WKUP_GEN_UV    WKUP_ESM_ERR_SIG_VDDSHV_WKUP_GEN_UV
#define  ESM_ERR_SIG_VDD_CPU_UV            WKUP_ESM_ERR_SIG_VDD_CPU_UV
#define  ESM_ERR_SIG_VDDR_MCU_UV           WKUP_ESM_ERR_SIG_VDDR_MCU_UV
#define  ESM_ERR_SIG_VMON_EXT_UV           WKUP_ESM_ERR_SIG_VMON_EXT_UV
#define  ESM_ERR_SIG_VDD_MCU_OV            WKUP_ESM_ERR_SIG_VDD_MCU_OV
#define  ESM_ERR_SIG_VDDR_CORE_UV          WKUP_ESM_ERR_SIG_VDDR_CORE_UV
#define  ESM_ERR_SIG_VDDSHV_WKUP_GEN_OV    WKUP_ESM_ERR_SIG_VDDSHV_WKUP_GEN_OV
#define  ESM_ERR_SIG_VDD_CORE_OV           WKUP_ESM_ERR_SIG_VDD_CORE_OV
#define  ESM_ERR_SIG_VDDR_MCU_OV           WKUP_ESM_ERR_SIG_VDDR_MCU_OV
#define  ESM_ERR_SIG_VDD_CPU_OV            WKUP_ESM_ERR_SIG_VDD_CPU_OV
#define  ESM_ERR_SIG_VDDR_CORE_OV          WKUP_ESM_ERR_SIG_VDDR_CORE_OV
#define  ESM_ERR_SIG_VMON_EXT_OV           WKUP_ESM_ERR_SIG_VMON_EXT_OV


/*===========================================================================*/
/*                         Macros                                            */
/*===========================================================================*/
/* None */

/* Global variables */
volatile Bool ESM_Error = false;
HwiP_Handle gHwiHandle;

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/
static int32_t cslPOKInPORTest(CSL_pok_id pokId, uint32_t intr_src, CSL_pwrss_trim trim_val, CSL_pwrss_vd_mode voltDetMode);
static int32_t cslPOKTest(CSL_pok_id pokId, uint32_t intr_src, CSL_pwrss_trim trim_val, CSL_pwrss_vd_mode voltDetMode);
static void    cslEsmSetupForPOK(uint32_t esm_err_sig);
static int32_t ESMApp_registerIsr(uintptr_t arg);
static int32_t ESMApp_unregisterIsr(uintptr_t arg);
static void    ESMApp_intrISR(uintptr_t handle);
static void cslGetErrSig(uint32_t id, CSL_pok_id *pokId, uint32_t  *esm_err_sig, Bool *usePorCfgFlag);

/*===========================================================================*/
/*                         Function definitions                              */
/*===========================================================================*/
static void cslGetErrSig(uint32_t id, CSL_pok_id *pokId, uint32_t  *esm_err_sig, Bool *usePorCfgFlag)
{
    switch (id)
    {
        case CSL_POR_POKHV_UV_ID:
            *pokId         = CSL_POR_POKHV_UV_ID;
            *usePorCfgFlag = TRUE;
            *esm_err_sig   = ESM_ERR_SIG_POKHV_UV;
            break;
        case CSL_POR_POKHV_OV_ID:
            *pokId         = CSL_POR_POKHV_OV_ID;
            *usePorCfgFlag = TRUE;
            *esm_err_sig   = ESM_ERR_SIG_POKHV_OV;
            break;
        case CSL_POR_POKLV_UV_ID:
            *pokId         = CSL_POR_POKLV_UV_ID;
            *usePorCfgFlag = TRUE;
            *esm_err_sig   = ESM_ERR_SIG_POKLV_UV;
            break;
        case CSL_POK_VDDA_PMIC_IN_ID:
            *pokId = CSL_POK_VDDA_PMIC_IN_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDDA_IN;
            break;
        case CSL_POK_VDD_CORE_UV_ID:
            *pokId = CSL_POK_VDD_CORE_UV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDD_CORE_UV;
            break;
        case CSL_POK_VDDSHV_WKUP_GEN_UV_ID:
            *pokId = CSL_POK_VDDSHV_WKUP_GEN_UV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDDSHV_WKUP_GEN_UV;
            break;
        case CSL_POK_VDD_CPU_UV_ID:
            *pokId = CSL_POK_VDD_CPU_UV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig     = ESM_ERR_SIG_VDD_CPU_UV;
            break;
        case CSL_POK_VDDR_MCU_UV_ID:
            *pokId = CSL_POK_VDDR_MCU_UV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig     = ESM_ERR_SIG_VDDR_MCU_UV;
            break;
        case CSL_POK_VMON_EXT_UV_ID:
            *pokId = CSL_POK_VMON_EXT_UV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig     = ESM_ERR_SIG_VMON_EXT_UV;
            break;
        case CSL_POK_VDD_MCU_OV_ID:
            *pokId = CSL_POK_VDD_MCU_OV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig     = ESM_ERR_SIG_VDD_MCU_OV;
            break;
        case CSL_POK_VDDR_CORE_UV_ID:
            *pokId = CSL_POK_VDDR_CORE_UV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDDR_CORE_UV;
            break;
        case CSL_POK_VDDSHV_WKUP_GEN_OV_ID:
            *pokId = CSL_POK_VDDSHV_WKUP_GEN_OV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDDSHV_WKUP_GEN_OV;
            break;
        case CSL_POK_VDD_CORE_OV_ID:
            *pokId = CSL_POK_VDD_CORE_OV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDD_CORE_OV;
            break;
        case CSL_POK_VDDR_MCU_OV_ID:
            *pokId = CSL_POK_VDDR_MCU_OV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDDR_MCU_OV;
            break;
        case CSL_POK_VDD_CPU_OV_ID:
            *pokId = CSL_POK_VDD_CPU_OV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDD_CPU_OV;
            break;
        case CSL_POK_VDDR_CORE_OV_ID:
            *pokId = CSL_POK_VDDR_CORE_OV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VDDR_CORE_OV;
            break;
        case CSL_POK_VMON_EXT_OV_ID:
        default:
            *pokId = CSL_POK_VMON_EXT_OV_ID;
            *usePorCfgFlag   = FALSE;
            *esm_err_sig   = ESM_ERR_SIG_VMON_EXT_OV;
            break;
    }
    return;
}

static int32_t ESMApp_unregisterIsr(uintptr_t arg)
{
    int32_t retVal = CSL_EFAIL;
    OsalInterruptRetCode_e osalRetVal;

    osalRetVal = Osal_DeleteInterrupt(&gHwiHandle, (int32_t)NULL);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        retVal = CSL_EFAIL;
        UART_printf("Error Could not un-register ISR !!!\n");
    }
    return (retVal);
}


static int32_t ESMApp_registerIsr(uintptr_t arg)
{
    int32_t retVal = CSL_EFAIL;
    uint32_t EsmInt;
    OsalRegisterIntrParams_t intrPrms;
    OsalInterruptRetCode_e osalRetVal;

    Osal_RegisterInterrupt_initParams(&intrPrms);
    intrPrms.corepacConfig.arg          = (uintptr_t) arg;
    intrPrms.corepacConfig.priority     = 1U;
    intrPrms.corepacConfig.corepacEventNum = 0U; /* NOT USED ? */

    EsmInt = ESM_INTID;
    intrPrms.corepacConfig.isrRoutine   = &ESMApp_intrISR;
    retVal = CSL_PASS;
    intrPrms.corepacConfig.intVecNum = EsmInt;
    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &gHwiHandle);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        retVal = CSL_EFAIL;
        UART_printf("Error Could not register ISR !!!\n");
    }
    return (retVal);
}


static void ESMApp_intrISR(uintptr_t handle)
{
    uint32_t esm_err_sig = (uint32_t) handle;
    ESM_Error = true;
    /* Disable the ESM Interrupt */
    ESMDisableIntr(ESM_INSTANCE, esm_err_sig);
    ESMClearIntrStatus(ESM_INSTANCE, esm_err_sig);
    ESMResetErrPin(ESM_INSTANCE);
}

void cslEsmSetupForPOK(uint32_t esm_err_sig)
{
    /* ESM Variables */
    esmInfo_t   appEsmInfo;


    /* Check INFO register for ESM last reset cause */
    ESMGetInfo(ESM_INSTANCE, &appEsmInfo);
    ESMReset(ESM_INSTANCE);

    /* The below function can be changed to force an error for diagnostic
     * reasons. */
    /* make sure we're not in force error mode */
    ESMSetMode(ESM_INSTANCE, ESM_OPERATION_MODE_NORMAL);

    /* Enable this ESM Error Signal */
    ESMEnableIntr(ESM_INSTANCE, esm_err_sig);

    /* Set the output interrupt priority level */
    ESMSetIntrPriorityLvl(ESM_INSTANCE, esm_err_sig, ESM_INTR_PRIORITY_LEVEL_HIGH);

    /* Enable Error Pin on this ESM Error Signal */
    ESMSetInfluenceOnErrPin(ESM_INSTANCE, esm_err_sig, TRUE);

    /* Enable for all ESM Error Signals */
    ESMEnableGlobalIntr(ESM_INSTANCE);
}


static int32_t cslPOKTest(CSL_pok_id pokId, uint32_t intr_src, CSL_pwrss_trim trim_val, CSL_pwrss_vd_mode voltDetMode)
{
    CSL_wkupCtrlRegsBase_t      *pBaseAddr = (CSL_wkupCtrlRegsBase_t *) CSL_TEST_POK_MMR_BASE;
    CSL_pokCfg_t                 pokCfg;
    CSL_pokVal_t                 pokVal;
    int32_t                      cslRet;

    /* POK configuration */
    /* Step 1: Mask POK event propogation by programming ESM_INTR_EN_CLR reg */
    cslRet = ESMDisableIntr(ESM_INSTANCE, intr_src);

    /* Step 2: Is POK Disabled, if not disable it */
    if (cslRet == CSL_PASS)
    {
        pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
        pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
        pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
        pokCfg.trim          = CSL_PWRSS_TRIM_NO_ACTION;
        pokCfg.detectionCtrl = CSL_POK_GET_DETECTION_VALUE;
        cslRet = CSL_pokGetControl(pBaseAddr, &pokCfg, &pokVal, pokId);
    }

    if (cslRet == CSL_PASS)
    {
        if (pokCfg.detectionCtrl == CSL_POK_DETECTION_ENABLE)
        {
            /* Disable the detection control */
                pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
                pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
                pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
                pokCfg.trim          = CSL_PWRSS_TRIM_NO_ACTION;
                pokCfg.detectionCtrl = CSL_POK_DETECTION_DISABLE;
                cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);
        }
    }

    /* Step 3: Program the appropriate threshold settings in POK_CTRL reg */
    if (cslRet == CSL_PASS)
    {
        pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
        pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
        pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
        pokCfg.detectionCtrl = CSL_POK_DETECTION_NO_ACTION;
        pokCfg.trim          = trim_val;
        cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);
    }

    /* Step 4: Enable the desired POK */
    if (cslRet == CSL_PASS)
    {    
        pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
        pokCfg.voltDetMode   = voltDetMode;
        pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
        pokCfg.trim          = CSL_PWRSS_TRIM_NO_ACTION;
        pokCfg.detectionCtrl = CSL_POK_DETECTION_ENABLE;
        cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);
    }
    /* Step 5: Wait for 100 us for the POK to settle */
    Osal_delay(1);

    /* Step 6: Read the POK out status to confirm voltage is within limits */
    if (cslRet == CSL_PASS)
    {   
        pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
        pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
        pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
        pokCfg.trim          = CSL_PWRSS_TRIM_NO_ACTION;
        pokCfg.detectionCtrl = CSL_POK_DETECTION_NO_ACTION;
        /* below will get voltage threshold status */
        cslRet = CSL_pokGetControl(pBaseAddr, &pokCfg, &pokVal, pokId);
    }
    /* Step 7: If Power Good == Yes, unmask POK to ESM event propagation by
                    programming ESM_INTR_EN_SET register(s) else report to WKUP_ESM0*/
    if (cslRet == CSL_PASS)
    {   
        cslRet = ESMEnableIntr(ESM_INSTANCE, intr_src);
    }

    if (cslRet == CSL_PASS)
    {   
        if (pokVal.voltageThrStatus == CSL_VOLTAGE_GOOD)
        {
            ESMClearIntrStatus(ESM_INSTANCE, intr_src);
            UART_printf("Voltage Good detected \n");
        }
        else
        {
            volatile int32_t i = 0;
            UART_printf("Voltage Not Good detected \n");
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
#if defined(BUILD_UART_SCANFMT)
                UART_printf(" Update the voltage threshold the test \
                             (Note: If threshold is below or above \
                             tracking threshold, interrupt may be triggered again \n");
                UART_scanFmt ("%d \n", &trim_val);
#else
                /* This should make no interrupt as the voltage would not be above this 
                   threshold value */
                trim_val = 127;
#endif
                pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
                pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
                pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
                pokCfg.detectionCtrl = CSL_POK_DETECTION_NO_ACTION;
                
                pokCfg.trim          = trim_val;
                cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);

                /* Re-enable the interrupt */
                ESMEnableIntr(ESM_INSTANCE, intr_src);

                cslRet  = CSL_PASS;
                ESM_Error = false;
            }
            else
            {
                cslRet = CSL_EFAIL;
            }
        }
    }
    return (cslRet);
}


static int32_t cslPOKInPORTest(CSL_pok_id pokId, uint32_t intr_src, CSL_pwrss_trim trim_val, CSL_pwrss_vd_mode voltDetMode)
{
    CSL_wkupCtrlRegsBase_t      *pBaseAddr = (CSL_wkupCtrlRegsBase_t *) CSL_TEST_POK_MMR_BASE;
    CSL_pokPorCfg_t              porCfg;
    CSL_pokCfg_t                 pokCfg;
    CSL_pokVal_t                 pokVal;
    int32_t                      cslRet;

    /* POR configuration */
    /* Step 1: MASKHHV set to 1 */
    porCfg.maskHHVOutputEnable                 = TRUE;

    /* Override is not needed to be changed for SW */
    porCfg.override[CSL_POKHV_OVERRIDE_INDEX]  = CSL_POR_SET_OVERRIDE_NO_CHANGE;
    porCfg.override[CSL_BGAP_OVERRIDE_INDEX]   = CSL_POR_SET_OVERRIDE_NO_CHANGE;
    porCfg.override[CSL_PORHV_OVERRIDE_INDEX]  = CSL_POR_SET_OVERRIDE_NO_CHANGE;
    porCfg.override[CSL_POKLVA_OVERRIDE_INDEX] = CSL_POR_SET_OVERRIDE_NO_CHANGE;
    porCfg.override[CSL_POKLVB_OVERRIDE_INDEX] = CSL_POR_SET_OVERRIDE_NO_CHANGE;

    /* Step 2: TRIM Mux selection is set to 0 */
    porCfg.trim_select                         = CSL_POR_TRIM_SELECTION_FROM_HHV_DEFAULT;

    cslRet = CSL_porSetControl (pBaseAddr,&porCfg);

    /* Step 3: Is POK Disabled, if not disable it */
    pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
    pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
    pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
    pokCfg.trim          = CSL_PWRSS_TRIM_NO_ACTION;
    pokCfg.detectionCtrl = CSL_POK_GET_DETECTION_VALUE;
    cslRet = CSL_pokGetControl(pBaseAddr, &pokCfg, &pokVal, pokId);

    if (cslRet == CSL_PASS)
    {
        if (pokCfg.detectionCtrl == CSL_POK_DETECTION_ENABLE)
        {
            /* Disable the detection control */
                pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
                pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
                pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
                pokCfg.trim          = CSL_PWRSS_TRIM_NO_ACTION;
                pokCfg.detectionCtrl = CSL_POK_DETECTION_DISABLE;
                cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);
        }
    }

    /* Step 4: Pending: Mask POK to ESM event propagation by
                    programming ESM_INTR_EN_CLR register(s)
     */
    if (cslRet == CSL_PASS)
    {
        cslRet = ESMDisableIntr(ESM_INSTANCE, intr_src);
    }

    /* Step 5: Program the appropriate threshold settings in POK_CTRL reg */
    if (cslRet == CSL_PASS)
    {    
        pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
        pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
        pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
        pokCfg.detectionCtrl = CSL_POK_DETECTION_NO_ACTION;

        pokCfg.trim          = trim_val;
        cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);
    }

    /* Step 6: Program the trim mux selection to 1, to select the new register/
               efuse values
     */
    if (cslRet == CSL_PASS)
    {
        porCfg.trim_select                         = CSL_POR_TRIM_SELECTION_FROM_CTRL_REGS;
        cslRet = CSL_porSetControl (pBaseAddr,&porCfg);

        /* Step 7: Set Mask HHV control to 0 */
        porCfg.trim_select                         = CSL_POR_TRIM_SELECTION_NO_CHANGE;
        porCfg.maskHHVOutputEnable                 = FALSE;
        cslRet = CSL_porSetControl (pBaseAddr,&porCfg);
    }
    /* Step 8: Enable the desired POK */
    if (cslRet == CSL_PASS)
    {
        pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
        pokCfg.voltDetMode   = voltDetMode;
        pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
        pokCfg.trim          = CSL_PWRSS_TRIM_NO_ACTION;
        pokCfg.detectionCtrl = CSL_POK_DETECTION_ENABLE;
        cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);
    }
    /* Step 9: Wait for 100 us for the POK to settle */
    Osal_delay(1); /* has 1000 micro seconds delay, more than needed */

    /* Step 10: Read the POK out status to confirm voltage is within limits */
    if (cslRet == CSL_PASS)
    {
        cslRet = CSL_pokGetControl(pBaseAddr, &pokCfg, &pokVal, pokId);
    }
    /* Step 11: If Power Good == Yes, unmask POK to ESM event propagation by
                    programming ESM_INTR_EN_SET register(s) else report to WKUP_ESM0*/
    if (cslRet == CSL_PASS)
    {
        cslRet = ESMEnableIntr(ESM_INSTANCE, intr_src);
    }

    if (cslRet == CSL_PASS)
    {
        if (pokVal.voltageThrStatus == CSL_VOLTAGE_GOOD)
        {
            ESMClearIntrStatus(ESM_INSTANCE, intr_src);
            UART_printf("Voltage Good detected \n");
        }
        else
        {
            volatile int32_t i = 0;
            UART_printf("Voltage Not Good detected \n");
           
            /* Wait for the ESM interrupt to report the error */
            do {
                i++;
                if (i > 0x0FFFFFFF)
                {
                    /* Timeout for the wait */
                    break;
                }
            } while (ESM_Error == false);

#if defined(BUILD_UART_SCANFMT)
            UART_printf(" Got the ESM Error Interrupt \n");
            UART_printf(" Update the voltage threshold the test \
                         (Note: If threshold is below or above \
                         tracking threshold, interrupt may be triggered again \n");
            UART_scanFmt ("%d \n", &trim_val);
#else
            /* This should make no interrupt as the voltage would not be above this 
               threshold value */
            trim_val = 127;
#endif
            pokCfg.hystCtrl      = CSL_PWRSS_HYSTERESIS_NO_ACTION;
            pokCfg.voltDetMode   = CSL_PWRSS_VOLTAGE_DET_NO_ACTION;
            pokCfg.pokEnSelSrcCtrl = CSL_POK_ENSEL_NO_ACTION;
            pokCfg.detectionCtrl = CSL_POK_DETECTION_NO_ACTION;
            pokCfg.trim          = trim_val;
            cslRet = CSL_pokSetControl(pBaseAddr, &pokCfg, pokId);

            if ((ESM_Error == true) && (cslRet == CSL_PASS))
            {
                cslRet  = CSL_PASS;
                ESM_Error = false;
            }
            else
            {
                cslRet = CSL_EFAIL;
            }
        }
    }
    return (cslRet);
}

int32_t cslPOKInPor_funcTest(void)
{
    int32_t                     testStatus, cslRet = CSL_PASS;
    CSL_pok_id                   pokId;
    uint32_t                     esm_err_sig;
    CSL_pwrss_trim               trim_val;
    uint32_t                     a;
#if defined(BUILD_UART_SCANFMT)
    uint32_t                     vd_mode;
#endif
    CSL_pwrss_vd_mode            voltDetMode;
    Bool                         usePorCfgFlag;

    UART_printf(" Below are the POK In POR ID values for the test\n");
    UART_printf("  CSL_POR_POKHV_UV_ID is:           14 \n");
    UART_printf("  CSL_POR_POKLV_UV_ID is:           15 \n");
    UART_printf("  CSL_POR_POKHV_OV_ID is:           16 \n");

#if defined (BUILD_UART_SCANFMT)
    UART_printf("\n\n Enter the POK ID for the test  \n");
    UART_scanFmt ("%d \n", &a);
    UART_printf(" Enter the voltage threshold the test  \n");
    UART_scanFmt ("%d \n", &trim_val);
    UART_printf(" Enter the Voltage Detection (0: UV, 1: OV) for the POK ID to monitor for the test  \n");
    UART_scanFmt ("%d \n", &vd_mode);
    if (vd_mode == 0U)
    {
        voltDetMode = CSL_PWRSS_SET_UNDER_VOLTAGE_DET_ENABLE;
    }
    else
    {
        voltDetMode = CSL_PWRSS_SET_OVER_VOLTAGE_DET_ENABLE;
    }
#else
    a = CSL_POR_POKHV_OV_ID;
    UART_printf ("\n\nDefault test with POK ID = %d , monitoring set to OV \n", a);
    voltDetMode = CSL_PWRSS_SET_OVER_VOLTAGE_DET_ENABLE;
    trim_val = 0;
#endif

    cslGetErrSig(a, &pokId, &esm_err_sig, &usePorCfgFlag);

    /* ESM Setup for POK tests */
    esm_err_sig--;
    cslEsmSetupForPOK(esm_err_sig);
    cslRet = ESMApp_registerIsr((uintptr_t)esm_err_sig);
    if (cslRet == CSL_PASS)
    {
        if (usePorCfgFlag == TRUE)
        {
            cslRet = cslPOKInPORTest(pokId, esm_err_sig, trim_val, voltDetMode);
        }
        else
        {
            cslRet = cslPOKTest(pokId, esm_err_sig, trim_val, voltDetMode);
        }
    }

    if (cslRet == CSL_PASS)
    {
        /* Un register the Interrupt */
        ESMApp_unregisterIsr((uintptr_t)esm_err_sig);
        testStatus = CSL_APP_TEST_PASS;
    }
    else
    {
        testStatus = CSL_APP_TEST_FAILED;
    }
    return (testStatus);
}


int32_t cslPOK_funcTest(void)
{
    int32_t                     testStatus, cslRet = CSL_PASS;
    CSL_pok_id                   pokId;
    uint32_t                     esm_err_sig;
    CSL_pwrss_trim               trim_val;
    uint32_t                     a;
#if defined(BUILD_UART_SCANFMT)
    uint32_t                     vd_mode;
#endif
    CSL_pwrss_vd_mode            voltDetMode;
    Bool                         usePorCfgFlag;

    UART_printf(" Below are the POK ID values \n");
    UART_printf("  CSL_POK_VDDA_PMIC_IN_ID is:       0 \n");
    UART_printf("  CSL_POK_VDD_CORE_UV_ID is:        1 \n");
    UART_printf("  CSL_POK_VDDSHV_WKUP_GEN_UV_ID is: 2 \n");
    UART_printf("  CSL_POK_VDD_CPU_UV_ID is:         3 \n");
    UART_printf("  CSL_POK_VDDR_MCU_UV_ID is:        4 \n");
    UART_printf("  CSL_POK_VMON_EXT_UV_ID is:        5 \n");
    UART_printf("  CSL_POK_VDD_MCU_OV_ID is:         6 \n");
    UART_printf("  CSL_POK_VDDR_CORE_UV_ID is:       7 \n");
    UART_printf("  CSL_POK_VDDSHV_WKUP_GEN_OV_ID is: 8 \n");
    UART_printf("  CSL_POK_VDD_CORE_OV_ID is:        9 \n");
    UART_printf("  CSL_POK_VDDR_MCU_OV_ID is:        10 \n");
    UART_printf("  CSL_POK_VDD_CPU_OV_ID is:         11 \n");
    UART_printf("  CSL_POK_VDDR_CORE_OV_ID is:       12 \n");
    UART_printf("  CSL_POK_VMON_EXT_OV_ID is:        13 \n");
    UART_printf("  CSL_POR_POKHV_UV_ID is:           14 \n");
    UART_printf("  CSL_POR_POKLV_UV_ID is:           15 \n");
    UART_printf("  CSL_POR_POKHV_OV_ID is:           16 \n");

#if defined (BUILD_UART_SCANFMT)
    UART_printf("\n\n Enter the POK ID for the test  \n");
    UART_scanFmt ("%d \n", &a);
    UART_printf(" Enter the voltage threshold the test  \n");
    UART_scanFmt ("%d \n", &trim_val);
    UART_printf(" Enter the Voltage Detection (0: UV, 1: OV) for the POK ID to monitor for the test  \n");
    UART_scanFmt ("%d \n", &vd_mode);
    if (vd_mode == 0U)
    {
        voltDetMode = CSL_PWRSS_SET_UNDER_VOLTAGE_DET_ENABLE;
    }
    else
    {
        voltDetMode = CSL_PWRSS_SET_OVER_VOLTAGE_DET_ENABLE;
    }
#else
    a = CSL_POK_VDD_CORE_OV_ID;
    UART_printf ("\n\nDefault test with POK ID = %d , monitoring set to OV \n", a);
    voltDetMode = CSL_PWRSS_SET_OVER_VOLTAGE_DET_ENABLE;
    trim_val = 0;
#endif

    cslGetErrSig(a, &pokId, &esm_err_sig, &usePorCfgFlag);

    /* ESM Setup for POK tests */
    esm_err_sig--;
    cslEsmSetupForPOK(esm_err_sig);
    cslRet = ESMApp_registerIsr((uintptr_t)esm_err_sig);
    if (cslRet == CSL_PASS)
    {
        if (usePorCfgFlag == TRUE)
        {
            cslRet = cslPOKInPORTest(pokId, esm_err_sig, trim_val, voltDetMode);
        }
        else
        {
            cslRet = cslPOKTest(pokId, esm_err_sig, trim_val, voltDetMode);
        }
    }

    if (cslRet == CSL_PASS)
    {
        /* Un register the Interrupt */
        ESMApp_unregisterIsr((uintptr_t)esm_err_sig);
        testStatus = CSL_APP_TEST_PASS;
    }
    else
    {
        testStatus = CSL_APP_TEST_FAILED;
    }
    return (testStatus);
}

/* Nothing past this point */
