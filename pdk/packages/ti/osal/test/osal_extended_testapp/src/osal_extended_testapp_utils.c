/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_extended_testapp_utils.c
 *
 *  \brief  OSAL utils Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Function Declarations                           */
/* ========================================================================== */

/*
 * Description  : Test the following APIs for Null check :
 *                  1) Osal_getHwAttrs
 *                  2) Osal_setHwAttrs
 *                  3) Osal_getStaticMemStatus
 */
static int32_t OsalApp_utilsNullcheckTest(void);

/*
 * Description: Testing Multi control for Osal_setHwAttrs API
 *
 */
static int32_t OsalApp_utilsSetHwAttrMultiCtrlTest(void);

/* ========================================================================== */
/*                      Internal Function Definitions                         */
/* ========================================================================== */

static int32_t OsalApp_utilsNullcheckTest(void)
{
    Osal_HwAttrs          *hwAttrs = NULL_PTR;
    Osal_StaticMemStatus  *pMemStats = NULL_PTR;
    int32_t               result = osal_OK;
    uint32_t              ctrlBitMap = 0U;

    if(osal_OK == Osal_getHwAttrs(hwAttrs))
    {
        result = osal_FAILURE;
    }
    if(osal_OK == result)
    {
        if(osal_OK == Osal_setHwAttrs(ctrlBitMap, hwAttrs))
        {
            result = osal_FAILURE;
        }
    }
    if(osal_OK == result)
    {
        if(osal_OK == Osal_getStaticMemStatus(pMemStats))
        {
            result = osal_FAILURE;
        }
    }
#if defined(SAFERTOS)
    /* for safertos privilege level check */
    Osal_isInPrivilegeMode();
#endif

    if(osal_OK != result)
    {
        OSAL_log("\n Utils Null test failed! \n");
    }

    return result;
}

static int32_t OsalApp_utilsSetHwAttrMultiCtrlTest(void)
{
    Osal_HwAttrs      hwAttrs;
    int32_t           result = osal_OK;
    uint32_t          ctrlBitMap = (OSAL_HWATTR_SET_EXT_CLK |
                                    OSAL_HWATTR_SET_HWACCESS_TYPE |
                                    OSAL_HWATTR_SET_CPU_FREQ);

    if(osal_OK != Osal_getHwAttrs(&hwAttrs))
    {
        result = osal_FAILURE;
    }
    if(osal_OK == result)
    {
        if(osal_OK != Osal_setHwAttrs(ctrlBitMap, &hwAttrs))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Utils Multi HwAttribute test failed! \n");
    }
    
    return result;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_utilsTests(void)
{
    int32_t result = osal_OK;
    
    result += OsalApp_utilsNullcheckTest();
    result += OsalApp_utilsSetHwAttrMultiCtrlTest();
    
    if(osal_OK == result)
    {
        OSAL_log("\n All Utils tests have passed!!\n");
    }
    else
    {
        OSAL_log("Some or all Utils tests have failed!!\n");
    }
    
    return result;
}

