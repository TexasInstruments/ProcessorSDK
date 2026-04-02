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
 *  \file   osal_extended_testapp_archutils.c
 *
 *  \brief  OSAL archutils Sub Module testcase for c7x file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_IRQ_INT_NUM           (28U)
#if defined(BUILD_C7X)
#define OSAL_APP_HWIP_BLOCK_SIZE       (1U * OSAL_SAFERTOS_HWIP_C7X_SIZE_BYTES)
#else
#define OSAL_APP_HWIP_BLOCK_SIZE       (1U * OSAL_NONOS_HWIP_SIZE_BYTES)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint8_t gOsalApphwiPMemBlock[OSAL_APP_HWIP_BLOCK_SIZE];

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description: Testing Arch utils API
 */
static int32_t OsalApp_archUtilsGeneralTest(void);

/*
 * Description: Testing External block test
 */
static int32_t OsalApp_archUtilsExtBlockTest(void);

#if defined(BUILD_C7X)
/*
 * Description: Testing Max creation test for c7x
 */
static int32_t OsalApp_archUtilsC7xMaxTest(void);
#endif

#if defined(BUILD_C66X)
/*
 * Description: Testing Negative checks for Hwi delete isused parameter for c66x
 */
static int32_t OsalApp_archUtilsIsUsedTest(void);

/*
 * Description: Testing Max creation test for c66x
 */
static int32_t OsalApp_archUtilsC66xMaxTest(void);

/*
 * Description: Testing Negative check for Hwi create with invalid parameters for c66x
 */
static int32_t OsalApp_archUtilsNegativeTest(void);
#endif
/* ========================================================================== */
/*                    Internal Function Definitions                           */
/* ========================================================================== */

static void OsalApp_hwiIRQ(uintptr_t arg)
{
    /* Do Nothing */
}

static int32_t OsalApp_archUtilsGeneralTest(void)
{
    HwiP_Params     hwiParams;
    HwiP_Handle     hwiHandle;
    uintptr_t       val = NULL;
    uint32_t        intNum = OSAL_APP_IRQ_INT_NUM;
    int32_t         result = osal_OK;

    HwiP_Params_init(&hwiParams);

    if(CSL_INVALID_EVENT_ID != OsalArch_getEventId(intNum))
    {
        result = osal_FAILURE;
    }

    if(NULL_PTR != OsalArch_HwiPCreate(intNum, OsalApp_hwiIRQ, NULL_PTR))
    {
        result = osal_FAILURE;
    }

    hwiHandle = OsalArch_HwiPCreate(intNum, OsalApp_hwiIRQ, &hwiParams);
    if(NULL_PTR == hwiHandle)
    {
        result = osal_FAILURE;
    }

#if defined(BUILD_C7X)
    if(NULL_PTR != OsalArch_getHandle(intNum))
#else
    if(NULL_PTR == OsalArch_getHandle(intNum))
#endif
    {
        result = osal_FAILURE;
    }

    osalArch_TimestampCcntAutoRefresh(val);

    OsalArch_enableInterrupt(intNum);

    OsalArch_globalDisableInterrupt();

    OsalArch_globalRestoreInterrupt(val);
    
    if(CSL_PASS == OsalArch_postInterrupt(intNum))
    {
        result = osal_FAILURE;
    }

    OsalArch_clearInterrupt(intNum);

    OsalArch_disableInterrupt(intNum);

    if(HwiP_OK != OsalArch_HwiPDelete(hwiHandle))
    {
        result = osal_FAILURE;
    }
    /* handle is deleted already, called to check the negative condition for used parameter of the handle */
    if(HwiP_OK == OsalArch_HwiPDelete(hwiHandle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Arch utils general test have failed!\n");
    }

    return result;
}

static int32_t OsalApp_archUtilsExtBlockTest(void)
{
    HwiP_Params       hwiParams;
    Osal_HwAttrs      hwAttrs;
    uint32_t          ctrlBitMap = OSAL_HWATTR_SET_HWIP_EXT_BASE;
    HwiP_Handle       hwiHandle;
    int32_t           result = osal_OK;

    /* set the extended block base and size */
    hwAttrs.extHwiPBlock.size       = OSAL_APP_HWIP_BLOCK_SIZE;
    hwAttrs.extHwiPBlock.base       = (uintptr_t) &gOsalApphwiPMemBlock[0];
    if(osal_OK != Osal_setHwAttrs(ctrlBitMap, &hwAttrs))
    {
        result = osal_FAILURE;
    }
    else
    {
        /* Default parameter initialization */
        HwiP_Params_init(&hwiParams);

        /* create the hwi block */
        hwiHandle = OsalArch_HwiPCreate(OSAL_APP_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams);
        if(NULL_PTR == hwiHandle)
        {
            result = osal_FAILURE;
        }

#if defined(BUILD_C7X)
        if(NULL_PTR != OsalArch_getHandle(OSAL_APP_IRQ_INT_NUM))
#else
        if(NULL_PTR == OsalArch_getHandle(OSAL_APP_IRQ_INT_NUM))
#endif
        {
            result = osal_FAILURE;
        }

        /* Verify the block created is in the extended memory block range */
        if((osal_OK == result) && (hwiHandle != (HwiP_Handle) &gOsalApphwiPMemBlock[0]))
        {
            result = osal_FAILURE;
        }
        if((osal_OK == result) && (HwiP_OK != OsalArch_HwiPDelete(hwiHandle)))
        {
            result = osal_FAILURE;
        }
        if(osal_OK == result)
        {
            /* Clear the extended block base for next tests */
            hwAttrs.extHwiPBlock.size       = 0U;
            hwAttrs.extHwiPBlock.base       = (uintptr_t) NULL_PTR;
            if (osal_OK != Osal_setHwAttrs(ctrlBitMap, &hwAttrs))
            {
                result = osal_FAILURE;
            }
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Extended block test have failed!\n");
    }

    return result;
}

#if defined(BUILD_C7X)
static int32_t OsalApp_archUtilsC7xMaxTest(void)
{
    HwiP_Params     hwiParams;
    HwiP_Handle     hwiHandle[OSAL_NONOS_CONFIGNUM_HWI];
    uint32_t        hwiIndex = 0U;
    int32_t         result = osal_OK;

    HwiP_Params_init(&hwiParams);

    /* Testing Maximum Heap creation */
    for(hwiIndex = 0U; hwiIndex < OSAL_NONOS_CONFIGNUM_HWI; hwiIndex++)
    {
        hwiHandle[hwiIndex] = OsalArch_HwiPCreate(hwiIndex, OsalApp_hwiIRQ, &hwiParams);
        if(NULL_PTR == hwiHandle[hwiIndex])
        {
            result = osal_FAILURE;
            break;
        }
    }

    if(osal_OK == result)
    {
        if(NULL_PTR != OsalArch_HwiPCreate(hwiIndex, OsalApp_hwiIRQ, &hwiParams))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        for(hwiIndex = 0U; hwiIndex < OSAL_NONOS_CONFIGNUM_HWI; hwiIndex++)
        {
            if(HwiP_OK != OsalArch_HwiPDelete(hwiHandle[hwiIndex]))
            {
                result = osal_FAILURE;
                break;
            }
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Multiple hwi create test failed! \n");
    }

    return result;
}
#endif

#if defined(BUILD_C66X)
static int32_t OsalApp_archUtilsIsUsedTest(void)
{
    HwiP_Params     hwiParams;
    HwiP_Handle     hwiHandle;
    uint32_t        intNum = OSAL_APP_IRQ_INT_NUM;
    int32_t         result = osal_OK;

    HwiP_Params_init(&hwiParams);
    
    hwiParams.enableIntr = BFALSE;

    hwiHandle = OsalArch_HwiPCreate(intNum, OsalApp_hwiIRQ, &hwiParams);
    if(NULL_PTR == hwiHandle)
    {
        result = osal_FAILURE;
    }

    /* Here handleAddr is used to get the memory location of the handle
    * we are corrupting the content of the handle and passing in a corrupt handle to the driver
    * to test negative condition for HeapP_alloc API
    */
    uint32_t *handleAddr = (uint32_t *)hwiHandle;

    if(NULL_PTR == OsalArch_getHandle(intNum))
    {
        result = osal_FAILURE;
    }

    if(HwiP_OK != OsalArch_HwiPDelete(hwiHandle))
    {
        result = osal_FAILURE;
    }
    /* handle is deleted already, 
     * enabling the is used parameter, and clearing the VecID and handle
     * to check the negative condition for OsalArch_HwiPDelete API */
    (*handleAddr) = 1U;
    *(handleAddr + 3U) = 0U;
    *(handleAddr + 4U) = 0U;
    if(HwiP_OK == OsalArch_HwiPDelete(hwiHandle))
    {
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Hwi delete is used parameter negative test have failed!\n");
    }

    return result;
}

static int32_t OsalApp_archUtilsNegativeTest(void)
{
    HwiP_Params     hwiParams;
    HwiP_Handle     hwiHandle;
    int32_t         result = osal_OK;

    HwiP_Params_init(&hwiParams);

    hwiParams.enableIntr = BFALSE;
    hwiParams.evtId = CSL_INVALID_EVENT_ID;

    hwiHandle = OsalArch_HwiPCreate(0U, OsalApp_hwiIRQ, &hwiParams);
    /* Reconfiguring the same interrupt to trigger negative scenario */
    hwiHandle = OsalArch_HwiPCreate(0U, OsalApp_hwiIRQ, &hwiParams);
    if(NULL_PTR != hwiHandle)
    {
        OSAL_log("hwiHandle = %x\n",hwiHandle);
        OsalArch_HwiPDelete(hwiHandle);
        result = osal_FAILURE;
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Hwi Create negative test have failed!\n");
    }

    return result;
}

static int32_t OsalApp_archUtilsC66xMaxTest(void)
{
    HwiP_Params     hwiParams;
    HwiP_Handle     hwiHandle[OSAL_NONOS_CONFIGNUM_HWI];
    uint32_t        hwiIndex = 0U, maxIndex = 0U;
    int32_t         result = osal_OK;

    HwiP_Params_init(&hwiParams);

    /* Testing Maximum Hwi creation */
    for(hwiIndex = 0U; hwiIndex < (OSAL_NONOS_CONFIGNUM_HWI + 4U); hwiIndex++)
    {
        if((21U != hwiIndex) && (16U != hwiIndex) && (14U != hwiIndex) && (15U != hwiIndex) && (20U != hwiIndex))
        {
            hwiParams.evtId = hwiIndex;
            hwiHandle[hwiIndex] = OsalArch_HwiPCreate(hwiIndex, OsalApp_hwiIRQ, &hwiParams);
            if(NULL_PTR == hwiHandle[hwiIndex])
            {
                result = osal_FAILURE;
                break;
            }
        }
    }

    if(osal_OK == result)
    {
        hwiParams.evtId = hwiIndex;
        if(NULL_PTR != OsalArch_HwiPCreate(hwiIndex, OsalApp_hwiIRQ, &hwiParams))
        {
            result = osal_FAILURE;
        }
    }

    maxIndex = hwiIndex;
    if(osal_OK == result)
    {
        for(hwiIndex = 0U; hwiIndex < maxIndex; hwiIndex++)
        {
            if((21U != hwiIndex) && (16U != hwiIndex) && (14U != hwiIndex) && (15U != hwiIndex) && (20U != hwiIndex))
            {
                if(HwiP_OK != OsalArch_HwiPDelete(hwiHandle[hwiIndex]))
                {
                    result = osal_FAILURE;
                    break;
                }
            }
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Multiple hwi create test failed! \n");
    }

    return result;
}
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_ArchutilsTests(void)
{
    int32_t result = osal_OK;
    result += OsalApp_archUtilsGeneralTest();
    result += OsalApp_archUtilsExtBlockTest();
#if defined(BUILD_C7X)
    result += OsalApp_archUtilsC7xMaxTest();
#elif defined(BUILD_C66X)
    result += OsalApp_archUtilsC66xMaxTest();
    result += OsalApp_archUtilsIsUsedTest();
    result += OsalApp_archUtilsNegativeTest();
#endif

    if(osal_OK == result)
    {
        OSAL_log("\n All Arch utils tests have passed!!\n");
    }
    else
    {
        OSAL_log("\n Some or All Arch utils tests have failed!!\n");
    }
    
    return result;
}
