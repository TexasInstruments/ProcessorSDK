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
 *  \file     ccmr5_test_set_get_op_mode.c
 *
 *  \brief    CCM Sub Module get and set mode testcases.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/csl/test/ccmr5/ccmr5_test_app/ccmr5_test_main.h>
#include <ti/csl/test/ccmr5/ccmr5_test_app/ccmr5_test_defs.h>
#include <ti/csl/test/ccmr5/ccmr5_test_app/ccmr5_test_set_get_op_mode.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                   Internal Function Declarations                           */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Function Definitions                                */
/* ========================================================================== */

int32_t CSLApp_getSetModeTest(void)
{
    int32_t testResult = CSL_PASS, metaInfo, iter;
    CSL_McuArmssCcmR5OpModeKey modeVal;
    uint32_t testOpModeVals[4][2] = 
    {
        {
            CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID, CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE
        },
        {
            CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID, CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_MODE
        },
        /* The error forcing mode takes one cycle to complete. Hence, the 
         * failing signature is presented for one clock cycle and the mode
         * is automatically switched to lock step mode and the key register will
         *  show the lock step key. */
        {
            CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID, CSL_MCU_ARMSS_CCMR5_MKEY_ERR_FORCE_MODE
        },
        /* In self test error forcing mode an error is forced at the self 
         * test error signal, selftest_error_o. The compare unit is still running 
         * in lock step mode and the key is switched to lock step after one clock cycle. */
        {
            CSL_MCU_ARMSS_CCMR5_CPU_MODULE_ID, CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_ERR_FORCE_MODE
        }
    };

    for (iter=0; iter<sizeof(testOpModeVals)/sizeof(testOpModeVals[0]); iter++)
    {
        testResult = CSL_armR5CCMSetOperationModeKey(CSL_TEST_CCM_BASE, testOpModeVals[iter][0], testOpModeVals[iter][1], &metaInfo);
        if (CSL_PASS == testResult)
        {
            testResult = CSL_armR5CCMGetOperationModeKey(CSL_TEST_CCM_BASE, testOpModeVals[iter][0], &modeVal, &metaInfo);
            if (CSL_PASS == testResult)
            {
                /* modeVal will not match expected mode val for ERR_FORCE and SELF_TEST_ERR_FORCE.
                 * Hence, neglect them.
                 */
                if(testOpModeVals[iter][1] != CSL_MCU_ARMSS_CCMR5_MKEY_ERR_FORCE_MODE &&
                    testOpModeVals[iter][1] != CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_ERR_FORCE_MODE &&
                    testOpModeVals[iter][1] != modeVal)
                {
                    testResult = CSL_EFAIL;
                }
            }
        }
        if (CSL_PASS != testResult)
        {
            break;
        }
    }
    
    return testResult;
}
