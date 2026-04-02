/*
 *   Copyright (c) Texas Instruments Incorporated 2024
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
 *  \file     ccm_neg_testapp.c
 *
 *  \brief    CCM Sub Module Negative testcases file.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/csl/test/ccmr5/ccmr5_test_app/ccmr5_test_main.h>
#include <ti/csl/test/ccmr5/ccmr5_test_app/ccmr5_test_defs.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* To cover static API CSL_armR5GetRegOffset */
#define CSL_CCMR5_INVALID_REGID        (7U)
#define CSL_CCMR5_INVALID_BASEADDRESS  (uintptr_t)(NULL)
#define CSL_CCMR5_VALID_BASEADDRESS    (uintptr_t)(CSL_TEST_CCM_BASE)
#define CSL_CCMR5_INVALID_OPMODEKEY    (1U)

/* ========================================================================== */
/*                   Internal Function Declarations                           */
/* ========================================================================== */

static int32_t CSL_compareErrorTest(void);
static int32_t CSL_OperationModeKeyTest(void);
static int32_t CSL_configureCCMRegisterTest(void);
static int32_t CSL_readCCMRegisterTest(void);

/* ========================================================================== */
/*                      Internal Function Definitions                         */
/* ========================================================================== */

static int32_t CSL_compareErrorTest(void)
{
    int32_t    result = CSL_PASS;
    
    /* Testing Negative case by passing invalid base Address */  
    if(CSL_PASS == CSL_armR5CCMClearCompareError(CSL_CCMR5_INVALID_BASEADDRESS,
                                            CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID,
                                             NULL))
    {
        result += CSL_EFAIL;
    }
    
    /* Testing Negative case by passing invalid module ID */
    if(CSL_PASS == CSL_armR5CCMClearCompareError(CSL_CCMR5_VALID_BASEADDRESS,
                                          CSL_MCU_ARMSS_CCMR5_INVALID_MODULE_ID,
                                          NULL))
    {
        result += CSL_EFAIL;
    }    
    
    /* Testing Negative case by passing invalid base Address */        
    if(CSL_PASS == CSL_armR5CCMGetCompareError(CSL_CCMR5_INVALID_BASEADDRESS,
                                           CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID,
                                            (uint32_t *)0U, NULL))
    {
        result += CSL_EFAIL;
    }
    
    /* Testing Negative case by passing invalid module ID */
    if(CSL_PASS == CSL_armR5CCMGetCompareError(CSL_CCMR5_VALID_BASEADDRESS,
                                          CSL_MCU_ARMSS_CCMR5_INVALID_MODULE_ID,
                                           (uint32_t *)1U, NULL))
    {
        result += CSL_EFAIL;
    }        
   
    if(CSL_PASS != result)
    {
        CSL_CCMR5AppPrint(" CSL_CompareError Negativetest failed\r\n"); 
    }                                      
   
    return result;    
}

static int32_t CSL_configureCCMRegisterTest(void)
{
    int32_t    result = CSL_PASS;
    
    /* Testing Negative case by passing invalid base Address */ 
    if(CSL_PASS == CSL_armR5ConfigureCCMRegister(CSL_CCMR5_INVALID_BASEADDRESS,
                                             CSL_MCU_ARMSS_CCMR5_CCMSR1_REGID,
                                             0U, NULL))
    {
        result += CSL_EFAIL;
    }
        
    if(CSL_PASS  != result)
    {
        CSL_CCMR5AppPrint("CSL_configureCCMRegister Negativetest failed\r\n"); 
    }
    
    return result;
}

static int32_t CSL_readCCMRegisterTest(void)
{
    int32_t    result = CSL_PASS;
   
    /* Testing Negative case by passing invalid base Address */ 
    if(CSL_PASS == CSL_armR5ReadCCMRegister(CSL_CCMR5_INVALID_BASEADDRESS,
                                 CSL_MCU_ARMSS_CCMR5_CCMSR1_REGID, NULL, NULL))
    {
        result += CSL_EFAIL;
    }
   
    /* Testing Negative case by passing invalid Reg ID */
    if(CSL_PASS == CSL_armR5ReadCCMRegister(CSL_CCMR5_VALID_BASEADDRESS,
                                    CSL_CCMR5_INVALID_REGID, ( void *)1U, NULL))
    {
        result += CSL_EFAIL;
    }
   
    if(CSL_PASS  != result)
    {
        CSL_CCMR5AppPrint(" CSL_readCCMRegister Negativetest failed\r\n");
    }
   
    return result;
}

static int32_t CSL_OperationModeKeyTest(void)
{
    int32_t    result = CSL_PASS;
  
    /* Testing Negative case by passing invalid base Address and different opModeKey */
    if(CSL_PASS == CSL_armR5CCMSetOperationModeKey(CSL_CCMR5_INVALID_BASEADDRESS,
                                        CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID,
                                        CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_MODE,
                                        NULL))
    {
        result += CSL_EFAIL;
    }
   
    /* Testing Negative case by passing invalid base Address and different opModeKey */
    if(CSL_PASS == CSL_armR5CCMSetOperationModeKey(CSL_CCMR5_INVALID_BASEADDRESS,
                                        CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID,
                                        CSL_MCU_ARMSS_CCMR5_MKEY_ERR_FORCE_MODE,
                                        NULL))
    {
        result += CSL_EFAIL;
    }
   
    /* Testing Negative case by passing invalid base Address and different opModeKey */ 
    if(CSL_PASS == CSL_armR5CCMSetOperationModeKey(CSL_CCMR5_INVALID_BASEADDRESS,
                                       CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID,
                       CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_ERR_FORCE_MODE, NULL))
    {
        result += CSL_EFAIL;
    }
   
    /* Testing Negative case by passing invalid base Address and opModeKey */ 
    if(CSL_PASS == CSL_armR5CCMSetOperationModeKey(CSL_CCMR5_INVALID_BASEADDRESS,
                                    CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID,
                                CSL_CCMR5_INVALID_OPMODEKEY, NULL))
    {
        result += CSL_EFAIL;
    }
   
    /* Testing Negative case by passing invalid MODULE_ID */ 
    if(CSL_PASS == CSL_armR5CCMSetOperationModeKey(CSL_CCMR5_VALID_BASEADDRESS,
                                   CSL_MCU_ARMSS_CCMR5_INVALID_MODULE_ID,
                                CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE, NULL))
    {
        result += CSL_EFAIL;
    }
    
    if(CSL_PASS == CSL_armR5CCMGetOperationModeKey(CSL_CCMR5_INVALID_BASEADDRESS,
                                 CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID, NULL, NULL))
    {
        result += CSL_EFAIL;
    }
    
    if(CSL_PASS != result)
    {
        CSL_CCMR5AppPrint(" CSL_OperationModeKey Negativetest failed\r\n");
    }
  
    return result;
}

int32_t CSLApp_CCMNegativeTests(void)
{
    int32_t result = CSL_PASS;
    
    result += CSL_compareErrorTest();
    result += CSL_OperationModeKeyTest();
    result += CSL_configureCCMRegisterTest();
    result += CSL_readCCMRegisterTest();
          
    if(CSL_PASS == result)
    {
        CSL_CCMR5AppPrint("\n All ccmr5 negative tests have passed!!\n");
    }
    else
    {
        CSL_CCMR5AppPrint("Some or all ccmr5 negative tests have failed!!\n");
    }
    
    return result;

}
